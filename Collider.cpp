#include "Collider.h"

Collider::Collider(RectangleShape& body) : body(body) {

}

Vector2f Collider::getHS() {
	return body.getSize() / 2.0f;
}

Vector2f Collider::getPosition() {
	return body.getPosition();
};