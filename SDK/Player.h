#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "Collider.h"
#include <map>
#include "Object.h"
using namespace sf;

class Player : public Movable
{
public:
	Player(Animation* animMap, Vector2f size, float speed, float jumpHeight, float weight = 1, Vector2f spawn = Vector2f(0, 0));
	Collider getCollider();
	void Update(float deltaTime);
	
	void onCollision(const Vector2f& direction);

	void Draw(RenderWindow& wnd) const;

	void respawn(float x, float y);

	float weight = 0;
	float speed;
	float jumpHeight;

private:
	std::string currAnim;
	Animation* anim;
	bool turned;
	bool canJump;

};

