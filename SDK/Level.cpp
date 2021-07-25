#include "Level.h"
#include <sstream>
#include "tinyxml.h"
#include <iostream>

void Level::insertWithPriority(vector<pair<int, TileMap>>& layers, pair<int, TileMap> tmap) {
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

bool Level::bordered() const {
	return _bordered;
}

vector<Object>& Level::getObjects() {
	return objects;
}

Level& Level::load(const string& filename, Vector2f offset, const RenderWindow* window) {
	tileLayers.clear();
	objects.clear();

#pragma region ��������� ������ ����


	_bordered = window != nullptr;
	if (_bordered) {
		objects.insert(objects.begin(), Object(nullptr, Vector2f(window->getSize().x, 1), Vector2f(window->getSize().x / 2, window->getSize().y))); // ����� ����
		objects.insert(objects.begin(), Object(nullptr, Vector2f(window->getSize().x, 1), Vector2f(window->getSize().x / 2, 0))); // ������
		objects.insert(objects.begin(), Object(nullptr, Vector2f(1, window->getSize().y), Vector2f(0, window->getSize().y / 2))); // �����
		objects.insert(objects.begin(), Object(nullptr, Vector2f(1, window->getSize().y), Vector2f(window->getSize().x + 1, window->getSize().y / 2))); // ������
	}
#pragma endregion


	TiXmlDocument doc(filename.c_str());
	doc.LoadFile();
	// ��������� �����
	TiXmlElement* map = doc.FirstChildElement("map");


	// ��������� ������� �����
	int width = atoi(map->Attribute("width")), height = atoi(map->Attribute("height"));
	for (TiXmlElement* child = map->FirstChildElement("layer"); child != NULL && string(child->Value()) == "layer"; child = child->NextSiblingElement())
	{
		int* tileArray = new int[width * height];

#pragma region ��������� ������
		{
			// ���� data
			// ������������ � ������
			string data(child->FirstChildElement("data")->GetText());
			// ������ ����� ����� �� ���� ������
			istringstream ss(data);
			char comma; // ����� ��� �������, ��� COMMA-separated values
			int value; // ���������������� ����� ����� � ��������

			int i = 0;
			while (ss >> value) {
				ss >> comma;
				// ��� �������� ��������, ���� �������� ������ 0, ������ �� ��� ����� 141 ������ ��������(������ ������������)
				tileArray[i] = (value ? value - 1 : 141);
				i++;
			}
			//for (int k = 0; k < width * height; k++) {
			//	if (k % width == 0) cout << endl;
			//	cout << tileArray[k] << " ";
			//}
			//cout << endl;
		}
		TileMap tmap;
		tmap.load("TileMap/tileset.png", { 32, 32 }, tileArray, width, height);
		insertWithPriority(tileLayers, pair<int, TileMap>(atoi(child->Attribute("name")), tmap));
		delete tileArray;
		tileArray = nullptr;
	}
#pragma endregion

#pragma region ��������� ��������
	

	TiXmlElement* objGroup = map->FirstChildElement("objectgroup");
	std::map<string, vector<string>> knownInteractives;
	for (TiXmlElement* child = objGroup->FirstChildElement("object"); child != NULL; child = child->NextSiblingElement())
	{
		string objName = string(child->Attribute("name"));
		if (objName == "solid") {
			int x = atoi(child->Attribute("x")), y = atoi(child->Attribute("y"));
			int width = atoi(child->Attribute("width")), height = atoi(child->Attribute("height"));
			objects.push_back(Object(nullptr, Vector2f(width, height), Vector2f(x + width / 2, y + height / 2), true));
		}
		else {
			istringstream ss(objName);
			string specifier, interactiveName;
			ss >> specifier >> interactiveName;

			if (specifier == "button") {
				if (interactiveName.empty()) throw runtime_error("Level.load(): ����������� ��� ������");
				
				int x = atoi(child->Attribute("x")), y = atoi(child->Attribute("y"));
				int width = atoi(child->Attribute("width")), height = atoi(child->Attribute("height"));
				Texture* buttonTexture = new Texture();
				if (!buttonTexture->loadFromFile("Textures/" + interactiveName + ".png")) {
					delete buttonTexture;
					buttonTexture = nullptr;
				}

				interactives.push_back(new InteractiveButton(buttonTexture, Vector2f(width, height), Vector2f(x + width / 2, y + height / 2), interactiveName));
			} else if (specifier == "lever") {
				if (interactiveName.empty()) throw runtime_error("Level.load(): ����������� ��� ������");

				int x = atoi(child->Attribute("x")), y = atoi(child->Attribute("y"));
				int width = atoi(child->Attribute("width")), height = atoi(child->Attribute("height"));
				Texture* buttonTexture = new Texture();
				if (!buttonTexture->loadFromFile("Textures/" + interactiveName + ".png")) {
					delete buttonTexture;
					buttonTexture = nullptr;
				}

				interactives.push_back(new InteractiveLever(buttonTexture, Vector2f(width, height), Vector2f(x + width / 2, y + height / 2), interactiveName));
			}
			else throw runtime_error("Level.load(): ������������ ��� �������(������������� �������)");
		}
	}
#pragma endregion

#pragma region ����������� �������� � ������ � ������� �����
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i].solid)
			objects[i].getCollider().Move(offset.x, offset.y - 32 * height);
	}
	for (auto& it : tileLayers) {
		it.second.move(offset.x, offset.y - 32 * height);
	}
	for (auto& it : interactives) {
		it->move(Vector2f(offset.x, offset.y - 32 * height));
	}
#pragma endregion
	return *this;
}

void Level::Draw(RenderWindow& wnd, Player* player) const {
	// ��������� ������� �����
	for (auto& it : tileLayers) 
		if (it.first < 0)
			wnd.draw(it.second);

	// ��������� ������������� ��������
	for (auto& it : interactives)
		if (it->getTexture() != nullptr) it->Draw(wnd);

	if (player != nullptr)
		player->Draw(wnd);

	// ��������� ��������� �����
	for (auto& it : tileLayers) 
		if (it.first > 0)
			wnd.draw(it.second);
	

	

	if (_bordered) 
		for (int i = 0; i < 4; i++)
			objects[i].Draw(wnd);
		
	

}

void Level::Update(Player& player) {
	for (auto& obj : interactives)
		if (obj->isActive())
			if (!player.getCollider().collides(obj->getCollider()))
				if (obj->getType() == IntObjType::Button)
				{
					auto& button = *(InteractiveButton*)obj;
					if (button.getRow() == 1) button.Update();
					button.pressed = false;
				}
}
