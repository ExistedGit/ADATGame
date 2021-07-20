#include "Player.h"

Player::Player(Texture* text, Vector2u frameCount, float switchTime, float speed, float jumpHeight, float weight, Vector2f spawn) : animation(text, frameCount, switchTime) {
	this->speed = speed;
	this->jumpHeight = jumpHeight;
	this->weight = weight;
	row = 0;
	turned = false;
	body.setSize(Vector2f(animation.uvRect.width, animation.uvRect.height));
	body.setPosition(spawn);
	body.setTexture(text);
	body.setOrigin(body.getSize() / 2.0f);
	
}

Collider Player::getCollider() {
	return Collider(body);
}

void Player::Update(float deltaTime) {
	velocity.x *= 0;

	if (Keyboard::isKeyPressed(Keyboard::A)) {
		velocity.x -= speed;
	}
	if (Keyboard::isKeyPressed(Keyboard::D)) {
		velocity.x += speed;
	}
	if (Keyboard::isKeyPressed(Keyboard::Space)) {
		if (canJump) {
			canJump = false;
			row = 0;
			velocity.y = -sqrtf(2 * 981.0f * jumpHeight);
		}
	}

	velocity.y += 981.0f * deltaTime * weight;

	if (velocity.x == 0) {
		row = 0;
	}
	else {
		if(canJump) row = 1;

		turned = velocity.x < 0;
	}

	animation.Update(row, deltaTime, turned);
	body.setTextureRect(animation.uvRect);
	body.move(velocity * deltaTime);
}

void Player::onCollision(const Vector2f& direction) {

	if (direction.x != 0) velocity.x = 0; // Горизонтальная коллизия
	if (direction.y != 0) {
		velocity.y = 0; // Вертикальная коллизия
		if (direction.y < 0)canJump = true;
		else canJump = false;
	}
	if(direction.y >= 0 && direction.x == 0)
		canJump = false;
	
}

void Player::Draw(RenderWindow& wnd) {
	wnd.draw(body);
}

Vector2f Player::getPos() {
	return body.getPosition();
}

float Player::getWeight() {
	return weight;
}

void Player::setWeight(float weight) {
	this->weight = weight;
}
