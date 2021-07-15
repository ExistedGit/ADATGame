#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;


class Animation
{
protected:
	Vector2u frameCount;
	Vector2u currFrame;
	Texture* texture;
	
	float switchTime;
	float totalTime = 0;

public:
	IntRect uvRect;

	Animation(Texture* text, Vector2u frameCount, float switchTime) {
		
		this->frameCount = frameCount;
		texture = text;
		this->switchTime = switchTime;
		currFrame.x = 0;

		uvRect.width = texture->getSize().x / frameCount.x;
		uvRect.height = texture->getSize().y / frameCount.y;
	};

	void Update(unsigned int row, float deltaTime, bool mirrored) {
		currFrame.y = row;
		totalTime += deltaTime;

		if (totalTime >= switchTime) {
			totalTime -= switchTime;
			currFrame.x++;

			if (currFrame.x >= frameCount.x) {
				currFrame.x = 0;
			}
		}

		uvRect.top = currFrame.y * uvRect.height;

		if (!mirrored) {
			uvRect.left = currFrame.x * uvRect.width;
			uvRect.width = abs(uvRect.width);
		}
		else {
			uvRect.left = (currFrame.x + 1) * abs(uvRect.width);
			uvRect.width = -abs(uvRect.width);
		}
	}
	
};

