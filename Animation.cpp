#include "Animation.h"

Animation::Animation(Texture* text, Vector2u frameCount, float switchTime) {

	this->frameCount = frameCount;
	texture = text;
	this->switchTime = switchTime;
	currFrame.x = 0;

	uvRect.width = texture->getSize().x / frameCount.x;
	uvRect.height = texture->getSize().y / frameCount.y;
}

void Animation::Update(unsigned int row, float deltaTime, bool mirrored) {
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
