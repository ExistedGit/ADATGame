#pragma once
#include "Collider.h"
#include "Animation.h"

class Object
{
protected:
	RectangleShape body;
	Animation* anim;
public:
	bool solid = false;
	bool gravity = false;
	bool push = false;
	float weight = 1;

	Object(Animation* text, Vector2f size, Vector2f pos, bool solid = false, bool gravity = false, float weight = 1, bool push = false);

	const Texture* getTexture() const;

	Collider getCollider();

	void Update(float deltaTime);

	void Draw(RenderWindow& wnd) const;

	void move(const Vector2f& offset);
	int getCurrFrame() const;
};

