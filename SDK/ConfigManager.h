#pragma once
#include "Level.h"
#include "tinyxml.h"
#include "Achievement.h"

class ConfigManager
{
public:
	static std::map<string, Level> loadLevels(RenderWindow* wnd = nullptr);
	// TODO: сделать нормально
	static vector<Achievement> load(const string& xmlDoc)
	{
		vector<Achievement> v;

		TiXmlDocument doc(xmlDoc.c_str());
		if (doc.LoadFile()) {
			
			TiXmlElement* achievementsXML = doc.FirstChildElement("Achievements");
			for (TiXmlElement* achievement = achievementsXML->FirstChildElement("Achievement"); achievement != nullptr && std::string(achievement->Value()) == "Achievement"; achievement = achievement->NextSiblingElement()) {
				std::string path = string("Textures/") + achievement->Attribute("id") + ".png";
				sf::Texture* tmp = new sf::Texture();
				tmp->loadFromFile(path.c_str());
				Achievement newAch(
					tmp,
					achievement->Attribute("name"),
					achievement->Attribute("condition"),
					achievement->Attribute("id"),
					atoi(achievement->Attribute("currProgress")),
					atoi(achievement->Attribute("maxProgress"))
				);

				newAch.setActive(true);
				v.push_back(newAch);
			}
		}
}
};

