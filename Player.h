#pragma once
#include "AnimatedSprite.h"

enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};


class Player
{
private:
	AnimatedSprite* _sprite;
	IntRect legHitbox;
	RenderWindow* _window;

public:
	int moveSpeed = 0;
	Player();
	Player(AnimatedSprite& sprite);

	void setWindow(RenderWindow& w);

	IntRect getLegHitbox();;
	void setLegHitbox(const IntRect& rect);
	AnimatedSprite& getSprite();

	void move(Keyboard::Key key);
};

