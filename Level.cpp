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

vector<Object>& Level::getObjects() {
	return objects;
}

Level& Level::load(const string& filename, Vector2f offset, const RenderWindow* window) {
	tileLayers.clear();
	objects.clear();

	if (window != nullptr) {
		objects.insert(objects.begin(), Object(nullptr, Vector2f(window->getSize().x, 1), Vector2f(window->getSize().x / 2, window->getSize().y))); // Снизу окна
		objects.insert(objects.begin(), Object(nullptr, Vector2f(window->getSize().x, 1), Vector2f(window->getSize().x / 2, 0))); // Сверху
		objects.insert(objects.begin(), Object(nullptr, Vector2f(1, window->getSize().y), Vector2f(0, window->getSize().y / 2))); // Слева
		objects.insert(objects.begin(), Object(nullptr, Vector2f(1, window->getSize().y), Vector2f(window->getSize().x + 1, window->getSize().y / 2))); // Справа
	}

	system("cls");

	TiXmlDocument doc(filename.c_str());
	doc.LoadFile();
	// Загружаем карту
	TiXmlElement* map = doc.FirstChildElement("map");


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
			//for (int k = 0; k < width * height; k++) {
			//	if (k % width == 0) cout << endl;
			//	cout << tileArray[k] << " ";
			//}
			cout << endl;
		}
		TileMap tmap;
		tmap.load("TileMap/tileset.png", { 32, 32 }, tileArray, width, height);
		insertWithPriority(tileLayers, pair<int, TileMap>(atoi(child->Attribute("name")), tmap));
		delete tileArray;
		tileArray = nullptr;
	}
#pragma endregion

#pragma region Обработка объектов
	TiXmlElement* objGroup = map->FirstChildElement("objectgroup");
	for (TiXmlElement* child = objGroup->FirstChildElement("object"); child != NULL; child = child->NextSiblingElement())
	{
		if (string(child->Attribute("name")) == "solid") {
			int x = atoi(child->Attribute("x")), y = atoi(child->Attribute("y"));
			int width = atoi(child->Attribute("width")), height = atoi(child->Attribute("height"));
			objects.push_back(Object(nullptr, Vector2f(width, height), Vector2f(x + width / 2, y + height / 2), true));
		}
	}
#pragma endregion

#pragma region Перемещение объектов и тайлов к нужному месту
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i].solid)
			objects[i].getCollider().Move(offset.x, offset.y - 32 * height);
	}
	for (auto& it : tileLayers) {
		it.second.move(offset.x, offset.y - 32 * height);
	}
#pragma endregion
	return *this;
}

void Level::Draw(RenderWindow& wnd, Player* player) {
	// Отрисовка заднего плана
	for (auto& it : tileLayers) {
		if (it.first < 0)
			wnd.draw(it.second);
	}

	if (player != nullptr)player->Draw(wnd);

	// Отрисовка переднего плана
	for (auto& it : tileLayers) {
		if (it.first > 0)
			wnd.draw(it.second);
	}

}
