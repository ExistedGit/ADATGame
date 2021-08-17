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
	anim->Update(deltaTime, false, "default");
	body.setTextureRect(anim->uvRect);
}

const RectangleShape& Object::getRect() const {
	return body;
}

void Object::Draw(RenderWindow& wnd) const {
	wnd.draw(body);
}

void Object::move(const Vector2f& offset) {
	body.move(offset);
}

int Object::getCurrFrame() const {
	return anim->getCurrFrame();
}