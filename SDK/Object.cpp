#include "Object.h"

Object::Object(Texture* text, Vector2f size, Vector2f pos, bool solid, bool gravity , float weight, bool push) : solid(solid), gravity(gravity), weight(weight), push(push) {
	body.setTexture(text);
	body.setSize(size);
	body.setOrigin(size / 2.0f);
	body.setPosition(pos);
}

const Texture* Object::getTexture() const {
	return body.getTexture();
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

void Object::move(const Vector2f& offset) {
	body.move(offset);
}
