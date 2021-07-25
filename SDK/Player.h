#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "Collider.h"

using namespace sf;

class Player
{
public:
	Player(Texture* text, Vector2u frameCount, float switchTime, float speed = 0, float jumpHeight = 0, float weight = 1, Vector2f spawn = Vector2f(0, 0));
	Collider getCollider();
	void Update(float deltaTime);

	void onCollision(const Vector2f& direction);

	void Draw(RenderWindow& wnd);
	Vector2f getPos();
	float getWeight();
	void setWeight(float weight);
private:
	RectangleShape body;
	Animation animation;
	unsigned int row;

	float weight = 0;
	float speed;
	float jumpHeight;
	bool turned;
	bool canJump = false;

	Vector2f velocity;
};

