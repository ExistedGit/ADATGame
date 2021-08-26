#pragma once
#include "Collider.h"
#include "Animation.h"

enum ObjectType {
	Solid = 1, Platform = 2, Moving = 4, Interactive = 8, Respawn = 16
};

class Movable {
public:
	Vector2f velocity; 
	
	Vector2f getPos() const;
	void setPos(float x, float y);
	void setPos(const Vector2f& pos);
	void move(float x, float y);
	void move(const Vector2f& offset);
	const RectangleShape& getRect() const;

protected:
	RectangleShape body;
};

class Object : public Movable
{
protected:
	shared_ptr<Animation> anim;
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

	virtual void Update(float deltaTime);

	void Draw(RenderWindow& wnd) const;

	void move(const Vector2f& offset);
};

class MovingPlatform : public Object {
private:
	// Направление движения(отрицательно: false, положительно: true)
	bool direction = true;
	float start;
public:	
	// Плоскость(x: false, y: true)
	bool plane;
	// Длина пути платформы
	float distance;
	// Скорость перемещения
	float speed;

	inline MovingPlatform(const Object& obj, float speed, bool plane, float distance) :
		Object(obj),
		plane(plane),
		speed(speed),
		distance(distance)
	{
		start = plane ? getPos().y : getPos().x;
		direction = plane ? start < start + distance : start < start + distance;
	};
	inline void Update(float deltaTime) override {
		velocity *= 0.0f;

		if (!plane) {
			if (start < start + distance) {
				if (getPos().x > start + distance)
					direction = false;
				else if (getPos().x < start)
					direction = true;
			}
			else {
				if (getPos().x < start + distance)
					direction = true;
				else if (getPos().x > start)
					direction = false;
			}
			velocity.x = speed * deltaTime * (direction ? 1 : -1);
		}
		else {
			if (start < start + distance) {
				if (getPos().y > start + distance)
					direction = false;
				else if (getPos().y < start)
					direction = true;
			}
			else {
				if (getPos().y < start + distance)
					direction = true;
				else if (getPos().y > start)
					direction = false;
			}
			velocity.y = speed * deltaTime * (direction ? 1 : -1);
		}
		
		Object::Update(deltaTime);
	}
	bool getDirection();
};