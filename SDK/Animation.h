#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "tinyxml.h"

using namespace sf;
using namespace std;

class Animation
{
private:
	int currFrame = 0;
	bool _mirrored = false;
	string currAnim = "default";
	map<string, vector<IntRect>> rectMap;
	map<string, float> switchMap;

	float totalTime = 0;
	Texture* texture;
public:
	IntRect uvRect;

	Animation(const string& xmlDoc);

	bool Update(float deltaTime, bool mirrored = false, string animName="");
	
	int getCurrFrame() const noexcept;;
	
	const Texture* getTexture() const noexcept;
	const string& getCurrAnim() const noexcept;

	bool setAnim(const string& animName);

};

class ComplexAnim {
private:
	Texture* texture;
	std::vector<IntRect> rect;
	float totalTime = 0;
	float switchTime;
	int currIndex = 0;

	
public:
	IntRect uvRect;
	ComplexAnim(const std::string& texturePath, const std::vector<IntRect>& rect, float switchTime);
	void Update(float deltaTime, bool mirrored = false);;
};
