#include "ConfigManager.h"

vector<Level> ConfigManager::loadLevels(RenderWindow& wnd) {
	vector<Level> vector;

	TiXmlDocument document("cfg/config.xml");
	document.LoadFile();

	TiXmlElement* loader = document.FirstChildElement("Loader");
	for (TiXmlElement* child = loader->FirstChildElement("Level"); child != NULL && string(child->Value()) == "Level"; child = child->NextSiblingElement()) {
		Vector2f offset( atoi(child->Attribute("x")), atoi(child->Attribute("y")));
		
		vector.push_back(Level().load(string("TileMap/") + child->GetText(), offset, &wnd));
	}

	return vector;
}
