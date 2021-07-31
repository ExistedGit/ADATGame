#pragma once
#include "Button.h"
#include "tinyxml.h"
#include "Animation.h"

class Menu : public IButtonArray
{
private:
	vector<ComplexAnim> anims;
public:
	void load(const string& xmlDoc) {
		TiXmlDocument doc(xmlDoc.c_str());

		TiXmlElement* sprites = doc.FirstChildElement("sprites");
		string texturePath = sprites->Attribute("image");

		for (TiXmlElement* anim = sprites->FirstChildElement("animation"); anim != NULL && string(anim->Value()) == "animation"; anim = anim->NextSiblingElement()) {
			vector<IntRect> v;
			for (TiXmlElement* cut = anim->FirstChildElement("cut"); cut!= NULL && string(cut->Value()) == "cut"; cut = cut->NextSiblingElement()) {
				int x = atoi(cut->Attribute("x")), 
					y = atoi(cut->Attribute("y")), 
					width= atoi(cut->Attribute("width")), 
					height = atoi(cut->Attribute("height"));
				v.push_back(IntRect(x, y, width, height));
			}
			anims.push_back(ComplexAnim(texturePath, v, 0));
		}
	}
};

