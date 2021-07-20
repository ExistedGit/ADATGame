#include "Object.h"

Object::Object(Texture* text, Vector2f size, Vector2f pos, bool solid, bool gravity , float weight, bool push) {
	body.setTexture(text);
	body.setSize(size);
	body.setOrigin(size / 2.0f);
	body.setPosition(pos);
	this->gravity = gravity;
	this->solid = solid;
	this->weight = weight;
	this->push = push;
}

Collider Object::getCollider() {
	return Collider(body);
}

void Object::Update(float deltaTime) {
	if (gravity) Collider(body).Move(0, 981.0f * deltaTime * weight);
}

void Object::Draw(RenderWindow& wnd) const {
	wnd.draw(body);
}
