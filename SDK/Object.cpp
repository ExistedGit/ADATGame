#include "Object.h"

Object::Object(Animation* text, Vector2f size, Vector2f pos, bool solid, bool gravity , float weight, bool push) : 
	anim(text), solid(solid), gravity(gravity), weight(weight), push(push) 
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

void Object::Draw(RenderWindow& wnd) const {
	wnd.draw(body);
}

void Object::move(const Vector2f& offset) {
	body.move(offset);
}

int Object::getCurrFrame() const {
	return anim->getCurrFrame();
}
