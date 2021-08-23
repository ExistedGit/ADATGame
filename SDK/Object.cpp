#include "Object.h"

Object::Object(Animation* text, const Vector2f& size, const Vector2f& pos, const ObjectType& type, bool gravity , float weight, bool push) :
	anim(text), type(type), gravity(gravity), weight(weight), push(push) 
{
	if (text != nullptr) body.setTexture(anim->getTexture());
	else body.setTexture(nullptr);
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
	if (anim != nullptr) {
		anim->Update(deltaTime, false, "default");
		body.setTextureRect(anim->uvRect);
	}
	body.move(velocity);
}

const RectangleShape& Movable::getRect() const {
	return body;
}

Vector2f Movable::getPos() const {
	return body.getPosition();
}

void Movable::setPos(float x, float y) {
	body.setPosition(x, y);
}

void Movable::setPos(const Vector2f& pos) {
	body.setPosition(pos);
}

void Movable::move(float x, float y) {
	body.move(x, y);
}

void Movable::move(const Vector2f& offset) {
	body.move(offset);
}
void Object::Draw(RenderWindow& wnd) const {
	if(body.getTexture() != nullptr)
		wnd.draw(body);
}

void Object::move(const Vector2f& offset) {
	body.move(offset);
}

int Object::getCurrFrame() const {
	return anim->getCurrFrame();
}

bool MovingPlatform::getDirection() {
	return direction;
}
