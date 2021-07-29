#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "Collider.h"
#include <map>

using namespace sf;

class Player
{
public:
	Player(map<string, Animation*> animMap, float speed, float jumpHeight, float weight = 1, Vector2f spawn = Vector2f(0, 0));
	Collider getCollider();
	void Update(float deltaTime);
	
	void onCollision(const Vector2f& direction);

	void Draw(RenderWindow& wnd) const;
	Vector2f getPos() const;
	float getWeight() const;
	void setWeight(float weight);
private:
	map<string, Animation*> animMap;
	string currAnim;
	RectangleShape body;

	float weight = 0;
	float speed;
	float jumpHeight;
	bool turned ;
	bool canJump;

	Vector2f velocity;
};

