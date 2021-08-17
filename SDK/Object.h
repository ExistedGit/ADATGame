#pragma once
#include "Collider.h"
#include "Animation.h"

enum ObjectType {
	Solid = 1, Platform = 2, Interactive = 4
};

class Object
{
protected:
	RectangleShape body;
	Animation* anim;
public:
	bool active = true;
	ObjectType type;
	bool gravity = false;
	bool push = false;
	float weight = 1;

	Object(Animation* text, const Vector2f& size, const Vector2f& pos, const ObjectType& type, bool gravity = false, float weight = 1, bool push = false);

	const Texture* getTexture() const;
	Collider getCollider();
	int getCurrFrame() const;
	const RectangleShape& getRect() const;

	void Update(float deltaTime);

	void Draw(RenderWindow& wnd) const;

	void move(const Vector2f& offset);
};

