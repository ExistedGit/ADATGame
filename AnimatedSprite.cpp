#include "AnimatedSprite.h"



void AnimatedSprite::move(const Vector2f& v2f) {
	sf::Sprite::move(v2f);
	_ifDraw = true;
}

bool AnimatedSprite::ifDraw() {
	return _ifDraw;
}

AnimatedSprite& AnimatedSprite::drawn() {
	_ifDraw = false;
	return *this;
}

void AnimatedSprite::setWindow(sf::RenderWindow& w) {
	_window = &w;
}

unsigned int AnimatedSprite::getAnimFrames() {
	return animFrames;
}

void AnimatedSprite::setAnimFrames(unsigned int af) {
	animFrames = af;
}

void AnimatedSprite::setCurrFrame(unsigned int cf) {
	currFrame = cf;
	setTextureRect(IntRect(currFrame * width, 0, width, height));
	lastAnimTime = cl.getElapsedTime().asSeconds();
	_ifDraw = true;
}

void AnimatedSprite::nextFrame() {
	if (currFrame < animFrames - 1)currFrame++;
	else currFrame = 0;
	setTextureRect(IntRect(currFrame * width, 0, width, height));
	lastAnimTime = cl.getElapsedTime().asSeconds();
	_ifDraw = true;
}

float AnimatedSprite::getLastAnimTime() {
	return lastAnimTime;
}

unsigned int AnimatedSprite::getCurrFrame() {
	return currFrame;
}

void AnimatedSprite::mirror() {
	if (!mirrored) {
		setScale(-1, 1);
		move(Vector2f(width, 0));
	}
	else {
		setScale(1, 1);
		move(Vector2f(-int(width), 0));
	}
	mirrored = !mirrored;
}
