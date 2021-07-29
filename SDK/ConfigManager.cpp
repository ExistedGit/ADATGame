#include "ConfigManager.h"
#include <iostream>
#include <regex>

vector<Level> ConfigManager::loadLevels(RenderWindow& wnd) {
	vector<Level> vector;

	TiXmlDocument document("cfg/config.xml");
	document.LoadFile();

	TiXmlElement* loader = document.FirstChildElement("Loader");
	string source;
	Vector2u tileSize(0,0);
	try {
		for (TiXmlElement* child = loader->FirstChildElement(); child != NULL && (string(child->Value()) == "Level" || string(child->Value()) == "tileset"); child = child->NextSiblingElement()) {
			if (string(child->Value()) == "tileset") {
				source = "TileMap/" + string(child->Attribute("src"));
				string tilesetPath = source;
				tilesetPath = regex_replace(tilesetPath,  regex("\\.png"), ".tsx");

				TiXmlDocument tilesetDoc(tilesetPath.c_str());
				tilesetDoc.LoadFile();
				tileSize = Vector2u(atoi(tilesetDoc.FirstChildElement("tileset")->Attribute("tilewidth")), atoi(tilesetDoc.FirstChildElement("tileset")->Attribute("tileheight")));
				continue;
			}
			if (tileSize == Vector2u(0, 0)) throw runtime_error("Изначальный тайлсет не был задан");
			Vector2f offset(atoi(child->Attribute("x")), atoi(child->Attribute("y")));

			vector.push_back(Level().load("TileMap/" + string(child->GetText()),source, tileSize, offset, &wnd));
		}
	}
	catch (runtime_error re) {
		cout << re.what();
	}

	return vector;
}
