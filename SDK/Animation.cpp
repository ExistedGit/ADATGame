#include "Animation.h"
#include <iostream>

Animation::Animation(const string& xmlDoc) :
	texture(new Texture()) {
	
	TiXmlDocument doc(xmlDoc.c_str());
	doc.LoadFile();
	if (doc.FirstChildElement("default") != nullptr) {
		string textPath = "Textures/" + string(doc.FirstChildElement("default")->Attribute("src"));
		texture->loadFromFile(textPath);
		rectMap["default"].push_back(IntRect(Vector2i(0, 0), Vector2i(texture->getSize())));
		
		return;
	}
	
	TiXmlElement* sprites = doc.FirstChildElement("sprites");
	for (TiXmlElement* anim = sprites->FirstChildElement("animation"); anim != nullptr; anim = anim->NextSiblingElement()) {
		switchMap[anim->Attribute("title")] = atoi(anim->Attribute("delay")) / 1000.;
		if (switchMap[anim->Attribute("title")] <= 0.01) 
			switchMap[anim->Attribute("title")] = 0;

		for (TiXmlElement* cut = anim->FirstChildElement("cut"); cut != nullptr; cut = cut->NextSiblingElement()) {
			int width = atoi(cut->Attribute("w")),
				height = atoi(cut->Attribute("h")), 
				x = atoi(cut->Attribute("x")), 
				y= atoi(cut->Attribute("y"));
			rectMap[anim->Attribute("title")].push_back(IntRect(x, y, width, height));
		}
	}

	{
		string textPath = "Textures/" + string(sprites->Attribute("image"));
		texture->loadFromFile(textPath);
	}

	try {
		if (rectMap.empty()) 
			throw out_of_range("Анимация(конструктор): был передан пустой массив кадров");
		uvRect = (*rectMap.begin()).second[0];
		currAnim = (*rectMap.begin()).first;
	}
	catch (out_of_range err) {
		cerr << err.what();
	}
}

bool Animation::Update(const string& animName, float deltaTime, bool mirrored) {
	bool success = false;

	totalTime += deltaTime;
	if (animName != currAnim) {
		currAnim = animName;
		totalTime = 0;
		uvRect = rectMap[currAnim][0];
		success = true;
	}

	if (totalTime >= switchMap[currAnim]) {
		totalTime -= switchMap[currAnim];
		currFrame = currFrame == rectMap[currAnim].size() - 1 ? 0 : currFrame + 1;
		uvRect = rectMap[currAnim][currFrame];
		success = true;
	}

	
	if (mirrored) {
		uvRect.left += abs(uvRect.width);
		uvRect.width = -abs(uvRect.width);
	}
	
	_mirrored = mirrored;

	return success;
}

int Animation::getCurrFrame() const noexcept {
	return currFrame;
}

const Texture* Animation::getTexture() const noexcept {
	return texture;
}

const string& Animation::getCurrAnim() const noexcept {
	return currAnim;
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
