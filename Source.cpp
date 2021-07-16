#include "Animation.h"
#include "Player.h"
#include "Object.h"
#include <vector>
#include <iostream>
#include <sstream>
#include "tinyxml.h"
#include "TileMap.h"
#include <map>

const Clock cl;

using namespace std;

#define VIEW_HEIGHT 768

void insertWithPriority(vector<pair<int, TileMap>>& layers, pair<int, TileMap> tmap) {
	if (layers.empty()) {
		layers.push_back(tmap);
		return;
	}
	
	for (int i = 0; i < layers.size() - 1; i++) {
		if (layers[i].first >= layers[i + 1].first) {
			layers.insert(layers.begin() + i, tmap);
			return;
		}
	}
	layers.push_back(tmap);
}

void loadFromXML(vector<pair<int, TileMap>>& layers, vector<Object>& v, string filename, int lowerBorder = 0) {
	layers.clear();
	v.clear();
	
	TiXmlDocument doc(filename.c_str());
	doc.LoadFile();
	// Загружаем карту
	TiXmlElement *map = doc.FirstChildElement("map");


	// Сохраняем размеры карты
	int width = atoi(map->Attribute("width")), height = atoi(map->Attribute("height"));
	for (TiXmlElement* child = map->FirstChildElement("layer"); child != NULL && string(child->Value()) == "layer"; child = child->NextSiblingElement())
	{
		int* tileArray = new int[width * height];

#pragma region Обработка тайлов
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
				// Для текущего тайлсета, если значение клетки 0, ставим на это место 141 клетку тайлсета(пустое пространство)
				tileArray[i] = (value ? value - 1 : 141);
				i++;
			}
#pragma region Вывод отсканированных карт
			for (int k = 0; k < width * height; k++) {
				if (k % width == 0) cout << endl;
				cout << tileArray[k] << " ";
			}
			cout << endl;
#pragma endregion
		}
		TileMap tmap;
		tmap.load("TileMap/tileset.png", { 32, 32 }, tileArray, width, height);
		insertWithPriority(layers, pair<int, TileMap>(atoi(child->Attribute("name")), tmap));
		delete tileArray;
		tileArray = nullptr;
	}
#pragma endregion

	TiXmlElement* objGroup = map->FirstChildElement("objectgroup");
	for (TiXmlElement* child = objGroup->FirstChildElement("object"); child != NULL; child = child->NextSiblingElement())
	{
		if (string(child->Attribute("name")) == "solid") {
			int x = atoi(child->Attribute("x")), y = atoi(child->Attribute("y"));
			int width = atoi(child->Attribute("width")), height = atoi(child->Attribute("height"));
			v.push_back(Object(nullptr, Vector2f(width, height), Vector2f(x + width/2, y + height/2), true ));
		}
	}

	for (int i = 0; i < v.size(); i++) {
		if (v[i].solid)
			v[i].getCollider().Move(0, lowerBorder - 32 * height);
	}
	for (auto& it : layers) {
		it.second.move(0, lowerBorder - 32 * height);
	}
}

int main() {
#pragma region Инициализация


	vector<pair<int, TileMap>> tmap; 

	View view(Vector2f(0, 0), Vector2f(1366,768));
	RenderWindow mainWindow(sf::VideoMode(1366, 768), "SFML");
	
	Texture* playerTexture = new Texture();
	playerTexture->loadFromFile("Textures/NewRatR.png");
	//Texture* mirrorTexture = new Texture();
	//mirrorTexture->loadFromFile("Textures/NewRatL.png");

	//Texture* vanTexture = new Texture();
	//vanTexture->loadFromFile("Textures/unnamed.jpg");

	Player player(playerTexture, Vector2u(4, 2), 0.15, 600, 150, 1, Vector2f(200, 200));
	vector<Object> objects;
	Vector2u mapSize;
	loadFromXML(tmap, objects, "TileMap/untitled.tmx", 768);
	
	

	objects.insert(objects.begin(), Object(nullptr, Vector2f(mainWindow.getSize().x, 1), Vector2f(mainWindow.getSize().x / 2, mainWindow.getSize().y))); // Снизу окна
	objects.insert(objects.begin(), Object(nullptr, Vector2f(mainWindow.getSize().x, 1), Vector2f(mainWindow.getSize().x / 2, 0))); // Сверху
	objects.insert(objects.begin(), Object(nullptr, Vector2f(1, mainWindow.getSize().y), Vector2f(0, mainWindow.getSize().y / 2))); // Слева
	objects.insert(objects.begin(), Object(nullptr, Vector2f(1, mainWindow.getSize().y), Vector2f(mainWindow.getSize().x + 1, mainWindow.getSize().y / 2))); // Справа
	
	
#pragma endregion

	Clock clock;
	float deltaTime = 0;
	Event ev;
	
	bool switched = false;

	while (mainWindow.isOpen()) {
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			mainWindow.close();
			return 0;
		}
		

		
		mainWindow.clear();

		deltaTime = clock.restart().asSeconds();
		if (deltaTime > 1. / 60.) {
			deltaTime = 1. / 60.;
		}

		while (mainWindow.pollEvent(ev)) {
			switch (ev.type) {
			case Event::Closed:
				mainWindow.close();
				return 0;
				break;
			case Event::Resized:
				view.setSize(VIEW_HEIGHT * (float(mainWindow.getSize().x) / float(mainWindow.getSize().y)), VIEW_HEIGHT);
				break;
			case Event::KeyPressed:
				if (ev.key.code == Keyboard::X) {
					if (!switched) loadFromXML(tmap, objects, "TileMap/other map.tmx", 768);
					else loadFromXML(tmap, objects, "TileMap/untitled.tmx", 768);

					switched = !switched;

					objects.insert(objects.begin(), Object(nullptr, Vector2f(mainWindow.getSize().x, 1), Vector2f(mainWindow.getSize().x / 2, mainWindow.getSize().y))); // Снизу окна
					objects.insert(objects.begin(), Object(nullptr, Vector2f(mainWindow.getSize().x, 1), Vector2f(mainWindow.getSize().x / 2, 0))); // Сверху
					objects.insert(objects.begin(), Object(nullptr, Vector2f(1, mainWindow.getSize().y), Vector2f(0, mainWindow.getSize().y / 2))); // Слева
					objects.insert(objects.begin(), Object(nullptr, Vector2f(1, mainWindow.getSize().y), Vector2f(mainWindow.getSize().x + 1, mainWindow.getSize().y / 2))); // Справа
				}
				break;
			}
		}
		
		player.Update(deltaTime);

		Vector2f direction;

		bool groundObj = false;
		for (auto& p : objects) {
			if (p.getCollider().CheckCollision(player.getCollider(), direction, !p.push)) {
				player.onCollision(direction);
				if (direction.y == -1) groundObj = true;
			};
		}
		if (!groundObj) player.onCollision(direction);

		// Отрисовка белых границ окна. Можно откомментировать когда-нибудь
		for (int i = 0; i < 4; i++) {
			objects[i].Draw(mainWindow);
		}

		// Отрисовка заднего плана
		for (auto& it : tmap) {
			if (it.first < 0)
				mainWindow.draw(it.second);
		}

		player.Draw(mainWindow);

		// Отрисовка переднего плана
		for (auto& it : tmap) {
			if(it.first > 0)
				mainWindow.draw(it.second);
		}

		view.setCenter(player.getPos());
		mainWindow.setView(view);

		mainWindow.display();
	}
	return 0;
}