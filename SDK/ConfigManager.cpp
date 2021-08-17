#include "ConfigManager.h"
#include <iostream>
#include <regex>
#include <map>

std::map<string, Level> ConfigManager::loadLevels(RenderWindow* wnd) {
	std::map<string, Level> map;

	TiXmlDocument document("cfg/config.xml");
	document.LoadFile();

	TiXmlElement* loader = document.FirstChildElement("Loader");
	try {
		for (TiXmlElement* child = loader->FirstChildElement("Level"); child != NULL && string(child->Value()) == "Level"; child = child->NextSiblingElement()) {
			
			try {
				Level newLevel = Level().load("TileMap/" + string(child->GetText()), wnd, child->Attribute("id"));
				map[newLevel.getName()] = newLevel;
			}
			catch (const string& err) {
				cout << err << endl;
			}
		}
	}
	catch (runtime_error re) {
		cout << re.what();
	}

	return map;
}
