#pragma once
#include "Collider.h"

class Object
{
protected:
	RectangleShape body;
public:
	bool solid = false;
	bool gravity = false;
	bool push = false;
	float weight = 1;

	

	Object(Texture* text, Vector2f size, Vector2f pos, bool solid = false, bool gravity = false, float weight = 1, bool push = false);

	const Texture* getTexture() const;

	Collider getCollider();

	void Update(float deltaTime);

	void Draw(RenderWindow& wnd) const;

	void move(const Vector2f& offset);
};

