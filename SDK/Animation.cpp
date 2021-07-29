#include "Animation.h"
#include <iostream>

Animation::Animation(const string& texturePath, const string& modelPath, const vector<IntRect>& rects, float switchTime)
	: rects(rects), switchTime(switchTime) {

	texture = new Texture();
	texture->loadFromFile(texturePath);

	try {
		if (rects.empty()) 
			throw out_of_range("Анимация(конструктор): был передан пустой массив кадров");
		uvRect = rects[0];
	}
	catch (out_of_range err) {
		cerr << err.what();
	}
}

void Animation::Update(float deltaTime, bool mirrored) {
	totalTime += deltaTime;

	if (totalTime >= switchTime) {
		totalTime -= switchTime;		
	}

	currFrame = currFrame == rects.size() - 1 ? 0 : currFrame + 1;
	uvRect = rects[currFrame];

	if (mirrored && !_mirrored) {
		uvRect.left += abs(uvRect.width);
		uvRect.width = -abs(uvRect.width);
	}
	else {
		uvRect.left += -abs(uvRect.width);
		uvRect.width = abs(uvRect.width);
	}
}

int Animation::getCurrFrame() const noexcept {
	return currFrame;
}

const Texture* Animation::getTexture() const noexcept {
	return texture;
}

Animation& Animation::load(const string& xmlDoc) {
	TiXmlDocument doc(xmlDoc.c_str());

	TiXmlElement* sprites = doc.FirstChildElement("sprites");
	string texturePath = sprites->Attribute("image");

	for (TiXmlElement* anim = sprites->FirstChildElement("animation"); anim != NULL && string(anim->Value()) == "animation"; anim = anim->NextSiblingElement()) {

		for (TiXmlElement* cut = anim->FirstChildElement("cut"); cut != NULL && string(cut->Value()) == "cut"; cut = cut->NextSiblingElement()) {
			int x = atoi(cut->Attribute("x")),
				y = atoi(cut->Attribute("y")),
				width = atoi(cut->Attribute("width")),
				height = atoi(cut->Attribute("height"));
			rects.push_back(IntRect(x, y, width, height));
		}
	}
	return *this;
}


ComplexAnim::ComplexAnim(const std::string& texturePath, const std::vector<IntRect>& rect, float switchTime) : switchTime(switchTime), rect(rect) {	
	texture = new Texture();
	texture->loadFromFile(texturePath);
}

void ComplexAnim::Update(float deltaTime, bool mirrored) {

	totalTime += deltaTime;
	if (totalTime - switchTime >= 0) {
		totalTime -= switchTime;

		currIndex = currIndex == rect.size() - 1 ? 0 : currIndex + 1;
		uvRect = rect[currIndex];
	}
}
