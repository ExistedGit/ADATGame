#include "ConfigManager.h"
#include <iostream>
#include <regex>
#include <map>

Level ConfigManager::loadLevel(const string& name, RenderWindow* wnd) {
	Level level;

	TiXmlDocument document("cfg/config.xml");
	document.LoadFile();

	TiXmlElement* loader = document.FirstChildElement("Loader");
	try {
		for (TiXmlElement* child = loader->FirstChildElement("Level"); child != NULL && string(child->Value()) == "Level"; child = child->NextSiblingElement()) {
			
			try {
				level = Level().load("TileMap/" + string(child->GetText()), wnd, child->Attribute("id"));
			}
			catch (const string& err) {
				cout << err << endl;
			}
		}
	}
	catch (const string& re) {
		cout << re;
	}

	return level;
}
