#include "Collider.h"

Collider::Collider(RectangleShape& body) : body(body) {

}
void Collider::Move(float x, float y) {
	body.move(x, y);
}
Vector2f Collider::getHS() {
	return body.getSize() / 2.0f;
}

Vector2f Collider::getPosition() {
	return body.getPosition();
};