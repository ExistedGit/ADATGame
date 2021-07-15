#include "Player.h"

Player::Player(Texture* text, Vector2u frameCount, float switchTime, float speed, float jumpHeight, Vector2f spawn) : animation(text, frameCount, switchTime) {
	this->speed = speed;
	this->jumpHeight = jumpHeight;
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
	velocity.x = 0;

	if (Keyboard::isKeyPressed(Keyboard::Left)) {
		velocity.x -= speed;
	}
	if (Keyboard::isKeyPressed(Keyboard::Right)) {
		velocity.x += speed;
	}
	if (Keyboard::isKeyPressed(Keyboard::Up) && canJump) {
		canJump = false;

		velocity.y = -sqrtf(2 * 981.0f * jumpHeight);
	}

	velocity.y += 981.0f * deltaTime;

	if (velocity.x == 0) {
		row = 0;
	}
	else {
		row = 1;

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
	}
}

void Player::Draw(RenderWindow& wnd) {
	wnd.draw(body);
}
