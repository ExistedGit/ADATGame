#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Collider
{
private:

	RectangleShape& body;
	
public:
	Collider(RectangleShape& body);

	void Move(float x, float y);

	Vector2f getPosition() const ;
	Vector2f getHS() const;

	bool CheckCollision(Collider other, Vector2f& direction, float push = 1, bool pushable = true);;

	bool collides(Collider other) {
		return FloatRect(getPosition(), body.getSize()).intersects(FloatRect(other.getPosition(), other.body.getSize()));
	}
};

