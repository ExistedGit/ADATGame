#include "ConfigManager.h"
#include <iostream>
#include <regex>

vector<Level> ConfigManager::loadLevels(RenderWindow* wnd) {
	vector<Level> vector;

	TiXmlDocument document("cfg/config.xml");
	document.LoadFile();

	TiXmlElement* loader = document.FirstChildElement("Loader");
	try {
		for (TiXmlElement* child = loader->FirstChildElement("Level"); child != NULL && string(child->Value()) == "Level"; child = child->NextSiblingElement()) {
			Vector2f offset(atoi(child->Attribute("x")), atoi(child->Attribute("y")));
			
			try {
				vector.push_back(Level().
					load("TileMap/" + string(child->GetText()), offset, wnd));
			}
			catch (const string& err) {
				cout << err << endl;
			}
		}
	}
	catch (runtime_error re) {
		cout << re.what();
	}

	return vector;
}
