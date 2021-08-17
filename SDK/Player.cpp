#include "Player.h"

Player::Player(Animation* animMap, Vector2f size, float speed, float jumpHeight, float weight, Vector2f spawn) :
	anim(animMap), 
	speed(speed), 
	jumpHeight(jumpHeight), 
	weight(weight),
	turned(false),
	canJump(false)
{
	currAnim = "idle";
	body.setSize(size);
	body.setPosition(spawn);
	body.setTexture(anim->getTexture());
	body.setTextureRect(anim->uvRect);
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
			velocity.y = -sqrtf(2 * 981.0f * jumpHeight);
		}
	}

	velocity.y += 981.0f * deltaTime * weight;

	string oldAnim = anim->getCurrAnim();
	if (velocity.x == 0) {
		currAnim = "idle";
	}
	else {
		if(canJump) 
			currAnim = "walk";

		turned = velocity.x < 0;
	}
	if(!canJump) 
		currAnim = "idle";

	anim->Update(deltaTime, false, currAnim);
	body.setTextureRect(anim->uvRect);
	body.move(velocity * deltaTime);

	if(velocity.x != 0) 
		body.setScale((velocity.x < 0 ? -1 : 1), 1);
}

void Player::onCollision(const Vector2f& direction) {

	if (direction.x != 0)
		velocity.x = 0; // Горизонтальная коллизия
	if (direction.y != 0) {
		velocity.y = 0; // Вертикальная коллизия
		canJump = direction.y < 0;
	}
	if (direction.y >= 0 && direction.x == 0)
		canJump = false;
	
}

void Player::Draw(RenderWindow& wnd) const {
	wnd.draw(body);
}



void Player::respawn(float x, float y) {
	velocity = Vector2f(0, 0);
	setPos(x, y);
	canJump = false;
}

