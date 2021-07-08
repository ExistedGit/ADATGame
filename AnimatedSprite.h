#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

extern const Clock cl;

class AnimatedSprite : public Sprite {
private:
	unsigned int animFrames = 0;
	unsigned int currFrame = 0;
	float lastAnimTime = 0;

	RenderWindow* _window = nullptr;
	bool _ifDraw = true;

	bool mirrored = false;
public:
	unsigned int width;
	unsigned int height;
	void move(const Vector2f& v2f);
	bool ifDraw();

	AnimatedSprite& drawn();

	void setWindow(sf::RenderWindow& w);

	unsigned int getAnimFrames();
	void setAnimFrames(unsigned int af);
	void setCurrFrame(unsigned int cf);
	void nextFrame();
	float getLastAnimTime();
	unsigned int getCurrFrame();

	void mirror();
};