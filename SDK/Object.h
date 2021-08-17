#pragma once
#include "Collider.h"
#include "Animation.h"

enum ObjectType {
	Solid = 1, Platform = 2, Moving = 4, Interactive = 8
};

class Movable {
public:
	Vector2f velocity; 
	
	Vector2f getPos() const;
	void setPos(float x, float y);
	void setPos(const Vector2f& pos);
	void move(float x, float y);;
	void move(const Vector2f& offset);;


	const RectangleShape& getRect() const;

protected:
	RectangleShape body;
};

class Object : public Movable
{
protected:
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

	void Update(float deltaTime);

	void Draw(RenderWindow& wnd) const;

	void move(const Vector2f& offset);
};

