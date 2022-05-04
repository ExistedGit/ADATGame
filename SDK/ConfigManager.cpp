#include "ConfigManager.h"
#include <iostream>
#include <regex>
#include <map>

Level ConfigManager::loadLevel(string name, RenderWindow* wnd) {
	Level level;

	TiXmlDocument document("cfg/config.xml");
	document.LoadFile();

	TiXmlElement* loader = document.FirstChildElement("Loader");
	try {
		bool levelLoaded = false;
		for (TiXmlElement* child = loader->FirstChildElement("Level"); child != NULL && string(child->Value()) == "Level"; child = child->NextSiblingElement()) {
			if (name == "")
				name = child->Attribute("id");
			if (string(child->Attribute("id")) == name) {
				level = Level().load("TileMap/" + string(child->GetText()), wnd, child->Attribute("id"));
				levelLoaded = true;
				break;
			}
		}
		if(!levelLoaded)
			cerr << u8"ConfigManager.load(): уровень не загружен\n";
	}
	catch (const string& re) {
		cout << re;
	}

	return level;
}
