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

	Vector2f getPosition();
	Vector2f getHS();

	bool CheckCollision(Collider other, Vector2f& direction, float push = 1, bool pushable = true) {
		direction = Vector2f(0, 0);

		Vector2f otherPos = other.getPosition();
		Vector2f otherHS = other.getHS();
		Vector2f thisPos = getPosition();
		Vector2f thisHS = getHS();

		float deltaX = otherPos.x - thisPos.x;
		float deltaY = otherPos.y - thisPos.y;

		float intersectX = abs(deltaX) - (otherHS.x + thisHS.x);
		float intersectY = abs(deltaY) - (otherHS.y + thisHS.y);

			
		if (intersectX < 0.0f && intersectY < 0.0f) {
			push = std::min(std::max(push, 0.0f), 1.0f);
			if (intersectX > intersectY) {
				if (deltaX > 0.0f) {
					Move(intersectX * (1.0f - push) , 0);
					other.Move(-intersectX * push * pushable, 0);

					direction.x = 1;
					direction.y = 0;
				}
				else {
					Move(-intersectX * (1 - push), 0);
					other.Move(intersectX * push * pushable, 0);
					

					direction.x = -1;
					direction.y = 0;
				}
			}
			else {
				if (deltaY > 0) {
					Move(0, intersectY * (1 - push));
					other.Move(0, -intersectY * push * pushable);

					direction.x = 0;
					direction.y = 1;
				}
				else {
					Move(0, -intersectY * (1 - push));
					other.Move(0, intersectY * push * pushable);


					direction.x = 0;
					direction.y = -1;
				}
			}
			return true;
		}

		return false;
	};

};
