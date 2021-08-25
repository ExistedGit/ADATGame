#include "Level.h"
#include <sstream>
#include "tinyxml.h"
#include <iostream>
#include <Windows.h>

bool Level::debug = false;
Font Level::debugFont = Font();
using namespace std;

void Level::insertWithPriority(vector<pair<int, TileMap>>& layers, pair<int, TileMap> tmap) {
	if (layers.empty()) {
		layers.push_back(tmap);
		return;
	}

	for (int i = 0; i < layers.size() - 1; i++)
		if (layers[i].first >= layers[i + 1].first) {
			layers.insert(layers.begin() + i, tmap);
			return;
		}
	
	layers.push_back(tmap);
}

Level::Level() :InteractiveArray() {
	Level::debugFont.loadFromFile("Fonts/kongtext.ttf");
}

bool Level::bordered() const {
	return _bordered;
}

const Vector2f& Level::getSize() const noexcept {
	return size;
}


Level& Level::load(string xmlDoc, const RenderWindow* window, const string& name, map<string, function<void()>> useMap) {
	this->filename = xmlDoc;

	tileLayers.clear();
	objects.clear();
	interactives.clear();

#pragma region Генерация границ окна

	this->name = name;
	
	_bordered = window != nullptr;
	if (_bordered) {
		objects.insert(objects.begin(), shared_ptr<Object>(new Object(nullptr, Vector2f(window->getSize().x, 1), Vector2f(window->getSize().x / 2, window->getSize().y), Solid))); // Снизу окна
		objects.insert(objects.begin(), shared_ptr<Object>(new Object(nullptr, Vector2f(window->getSize().x, 1), Vector2f(window->getSize().x / 2, 0), Solid))); // Сверху
		objects.insert(objects.begin(), shared_ptr<Object>(new Object(nullptr, Vector2f(1, window->getSize().y), Vector2f(0, window->getSize().y / 2), Solid))); // Слева
		objects.insert(objects.begin(), shared_ptr<Object>(new Object(nullptr, Vector2f(1, window->getSize().y), Vector2f(window->getSize().x + 1, window->getSize().y / 2), Solid))); // Справа
	}
#pragma endregion

#pragma region Инициализация
	TiXmlDocument doc(xmlDoc.c_str());
	if (!doc.LoadFile()) {
		throw xmlDoc + u8": файл карты не найден";
		return *this;
	}
	// Загружаем карту
	TiXmlElement* map = doc.FirstChildElement("map");
	string tileset = string("TileMap/") + map->FirstChildElement("tileset")->FirstChildElement("image")->Attribute("source");;
	// Сохраняем размеры карты
	int width = atoi(map->Attribute("width")), height = atoi(map->Attribute("height"));
	Vector2u tileSize(atoi(map->Attribute("tilewidth")), atoi(map->Attribute("tileheight")));
	size = Vector2f(width * tileSize.x, height * tileSize.y);
#pragma endregion

#pragma region Обработка тайлов
	for (TiXmlElement* child = map->FirstChildElement("layer"); child != NULL && (string(child->Value())) == "layer"; child = child->NextSiblingElement())
	{
		int* tileArray = new int[width * height];
		try {
			atoi(child->Attribute("name"));
		}
		catch (...) {
			cerr << "Level.load(): неверное имя слоя тайлов(используйте 1 или -1)";
		}
		
		{
			// Берём data
			// Конвертируем в строку
			string data(child->FirstChildElement("data")->GetText());
			// Создаём поток ввода из этой строки
			istringstream ss(data);
			char comma; // буфер для запятых, ибо COMMA-separated values
			int value; // непосредственный номер тайла в тайлсете

			int i = 0;
			while (ss >> value) {
				ss >> comma;
				// Для текущего тайлсета, если значение клетки 0, ставим на это место первую клетку тайлсета(пустое пространство)
				tileArray[i] = (value ? value - 1 : 0);
				i++;
			}
			//for (int k = 0; k < width * height; k++) {
			//	if (k % width == 0) cout << endl;
			//	cout << tileArray[k] << " ";
			//}
			//cout << endl;
		}
		TileMap tmap;
		tmap.load(tileset, tileSize, tileArray, width, height);
		insertWithPriority(tileLayers, pair<int, TileMap>(atoi(child->Attribute("name")), tmap));
		delete tileArray;
		tileArray = nullptr;
	}
#pragma endregion

#pragma region Обработка объектов
	TiXmlElement* objGroup = map->FirstChildElement("objectgroup");
	if (objGroup != nullptr) {
		for (TiXmlElement* object = objGroup->FirstChildElement("object"); object != NULL; object = object->NextSiblingElement())
		{
			string objName = string(object->Attribute("name"));
			if (objName.empty())
			{
				cerr << u8"Level.load(): объект без названия" << endl;
				continue;
			}

			if (objName == "solid") {
				float x = atof(object->Attribute("x")), y = atof(object->Attribute("y"));
				float width = atof(object->Attribute("width")), height = atof(object->Attribute("height"));
				objects.push_back(shared_ptr<Object>(new Object(nullptr, Vector2f(width, height), Vector2f(x + width / 2, y + height / 2), ObjectType::Solid)));
			}
			else if (objName == "platform") {
				float x = atof(object->Attribute("x")),
					y = atof(object->Attribute("y"));
				float width = atof(object->Attribute("width")),
					height = 140; // Работае только так, нет времени фиксытъ
				objects.push_back(shared_ptr<Object>(new Object(nullptr, Vector2f(width, height), Vector2f(x + width / 2, y + height / 2), ObjectType(Platform | Solid))));
			}
			else if (objName == "spawn") {
				float x = atof(object->Attribute("x")),
					y = atof(object->Attribute("y"));
				spawn = Vector2f(x, y);
			}
			else if (objName == "respawn") {
				float x = atof(object->Attribute("x")), y = atof(object->Attribute("y"));
				float width = atof(object->Attribute("width")), height = atof(object->Attribute("height"));
				objects.push_back(shared_ptr<Object>(new Object(nullptr, Vector2f(width, height), Vector2f(x + width / 2, y + height / 2), ObjectType::Respawn)));
			}
			else {
				istringstream ss(objName);
				string specifier, interactiveName, modelName;
				ss >> specifier >> interactiveName >> modelName;

				if (interactiveName.empty()) 
					throw u8"Level.load(): интерактивный объект без имени\n";
				bool oneTime = false;
				if (object->Attribute("type") != nullptr) {
					string type;
					istringstream typestream(object->Attribute("type"));
					typestream >> type;

					oneTime = type == "oneTime";
				}

				std::string modelPath = !modelName.empty() ? 
					"Models/" + modelName+ ".xml" :
					"";
				
				Animation* anim = new Animation(modelPath);
				if (specifier == "button") {
					
					float x = atof(object->Attribute("x")),
						y = atof(object->Attribute("y"));
					float width = atof(object->Attribute("width")),
						height = atof(object->Attribute("height"));

					interactives.push_back(new InteractiveButton(anim, Vector2f(width, height), Vector2f(x + width / 2, y + height / 2), interactiveName, useMap.count(interactiveName) ? useMap[interactiveName] : []() {}, oneTime));
				}
				else if (specifier == "lever") {

					float x = atof(object->Attribute("x")),
						y = atof(object->Attribute("y"));
					float width = atof(object->Attribute("width")),
						height = atof(object->Attribute("height"));

					interactives.push_back(new InteractiveLever(anim, Vector2f(width, height), Vector2f(x + width / 2, y + height / 2), interactiveName, useMap.count(interactiveName) ? useMap[interactiveName] : []() {}, oneTime));
				}
				else if (specifier == "door") {
					float x = atof(object->Attribute("x")),
						y = atof(object->Attribute("y"));
					float width = atof(object->Attribute("width")),
						height = atof(object->Attribute("height"));

					RectangleShape hitbox;
					{
						string modelPath = "Models/Complex/" + modelName + ".xml";
						TiXmlDocument doorModel(modelPath.c_str());
						doorModel.LoadFile();
						TiXmlElement* hitboxElem = doorModel.FirstChildElement("hitbox");

						Vector2f hitboxSize = Vector2f(
							atoi(hitboxElem->Attribute("w")),
							atoi(hitboxElem->Attribute("h"))
						);
						Vector2f offset = Vector2f(
							hitboxElem->Attribute("x") != nullptr ?
							atoi(hitboxElem->Attribute("x")) :
							0,
							hitboxElem->Attribute("y") != nullptr ?
							atoi(hitboxElem->Attribute("y")) :
							0);
						hitbox = RectangleShape(hitboxSize);
						hitbox.move(offset);
					}

					interactives.push_back(new InteractiveDoor(anim, hitbox, Vector2f(width, height), Vector2f(x + width / 2, y + height / 2), interactiveName, oneTime));
				}
				else if (specifier == "moveplatform") {
					bool platformPlane;
					float platformDistance, platformSpeed;
					{
						istringstream typestream(object->Attribute("type"));
						string speed, plane, distance;
						typestream >> speed >> plane >> distance;
						platformSpeed = atof(speed.c_str());
						if (plane != "y" && plane != "x")
							throw u8"Level.load(): Неверный символ направления движущейся платформы\n";
						platformPlane = plane == "y";
						platformDistance = atof(distance.c_str());
					}

					float x = atof(object->Attribute("x")),
						y = atof(object->Attribute("y"));
					float width = atof(object->Attribute("width")),
						height = atof(object->Attribute("height"));
					
					shared_ptr<Object> moveplatform = shared_ptr<Object>(new MovingPlatform(Object(anim, Vector2f(width, height), Vector2f(x + width / 2, y + height / 2), ObjectType(Moving | Solid)), platformSpeed, platformPlane, platformDistance));

					objects.push_back(moveplatform);
					namedObjects[interactiveName] = moveplatform;
				}
				else throw u8"Level.load(): недопустимый тип объекта";
			}
		}
	}
#pragma endregion
#pragma region Перемещение объектов и тайлов к нужному месту
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i]->type & ObjectType::Solid)
			objects[i]->getCollider().Move(1, size.y - tileSize.y * height);
	}
	for (auto& it : tileLayers) {
		it.second.move(1, size.y - tileSize.y * height);
	}
	for (auto& it : interactives) {
		it->move(Vector2f(1, size.y - tileSize.y * height));
	}
#pragma endregion
	return *this;
}

void Level::reload() {
	load(filename);
}

void Level::checkCollision(Player& player, float deltaTime) {

	Vector2f direction;
	Object* groundCollision = nullptr;
	for (auto& p : objects) {
		if (p->active) {
			if (p->type & Platform) {
				if (player.getPos().y + player.getRect().getSize().y < p->getRect().getPosition().y
					&&
					player.velocity.y > 0) {
					p->type = ObjectType(p->type | Solid);
					if (Keyboard::isKeyPressed(Keyboard::S))
						p->type = ObjectType(p->type ^ Solid);

				}
				else if (p->type & Solid)
					p->type = ObjectType(p->type ^ Solid);
			}

			if ((p->type & Respawn) && player.getCollider().collides(p->getCollider())) {
				player.respawn(spawn.x, spawn.y);
				return;
			}

			if (p->type & Solid) {
				if (p->getCollider().CheckCollision(player.getCollider(), direction)) {
					player.onCollision(direction);
					if (direction.y == -1)
						groundCollision = p.get();
					if ((p->type & Moving) && ((MovingPlatform*)p.get())->plane == false)
						player.move(((MovingPlatform*)p.get())->speed * deltaTime * (((MovingPlatform*)p.get())->getDirection() ? 1 : -1), 0);
				};
			}
		}
	}
	for (auto& p : interactives)
		if (p->active)
			if(p->type & Solid)
				if (p->getType() == IntObjType::Door) {
					if (((InteractiveDoor*)(p))->getHitbox().CheckCollision(player.getCollider(), direction))
						player.onCollision(direction);
				}
				else if (p->getCollider().CheckCollision(player.getCollider(), direction)) {
						player.onCollision(direction);
						if (direction.y == -1) 
							groundCollision = p;
				};
			
	if (groundCollision == nullptr) 
		player.onCollision(direction);

		

}

const string& Level::getName() const { return name; }

void Level::Draw(RenderWindow& wnd, Player* player) const {
	// Отрисовка заднего плана
	for (auto& it : tileLayers)
		if (it.first < 0)
			wnd.draw(it.second);

	// Отрисовка интерактивных объектов
	for (auto& it : interactives)
		if (it->getTexture() != nullptr) it->Draw(wnd);

	if (player != nullptr)
		player->Draw(wnd);

	// Отрисовка переднего плана
	for (auto& it : tileLayers)
		if (it.first > 0)
			wnd.draw(it.second);
	for (auto& it : objects)
		it->Draw(wnd);
	

	if (_bordered)
		for (int i = 0; i < 4; i++)
			objects[i]->Draw(wnd);

	if (debug) {
		Text text;
		text.setFont(debugFont);
		for (auto& it : objects) {
			text.setPosition(it->getRect().getPosition());
			text.setString(it->type & Solid ? "1" : "0");
			wnd.draw(text);
		}
	}
}

void Level::applyUseMap(map<string, function<void()>> map) {
	for (auto& obj : interactives) {
		if (map.count(obj->getName())) {
			obj->use = map[obj->getName()];
		}
	}
}

void Level::Update(Player& player) {
	for (auto& obj : interactives)
		if (obj->isActive())
			if (!player.getCollider().collides(obj->getCollider()))
				if (obj->getType() == IntObjType::Button)
				{
					auto& button = *(InteractiveButton*)obj;
					if (button.getCurrFrame() == 1) button.Update();
					button.pressed = false;
				}
}
void Level::Update(float deltaTime) {
	for (auto& obj : objects) {
		obj->Update(deltaTime);
	}
}