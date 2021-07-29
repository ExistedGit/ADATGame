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

	vector<IntRect> rects;

	float switchTime = 0;
	float totalTime = 0;
	Texture* texture;
public:
	IntRect uvRect;

	Animation(const string& texturePath, const string& modelPath, const vector<IntRect>& rects, float switchTime);

	void Update( float deltaTime, bool mirrored);
	
	int getCurrFrame() const noexcept;;
	
	const Texture* getTexture() const noexcept;

	Animation& load(const string& xmlDoc);
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
