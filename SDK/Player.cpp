#include "Player.h"

Player::Player(map<string, Animation*> animMap, float speed, float jumpHeight, float weight, Vector2f spawn) :
	animMap(animMap), 
	speed(speed), 
	jumpHeight(jumpHeight), 
	weight(weight),
	turned(false),
	canJump(false)
{
	if (animMap.empty()) throw invalid_argument("Player(конструктор): карта анимаций пуста");

	currAnim = (*animMap.begin()).first;
	body.setSize(Vector2f(animMap[currAnim]->uvRect.width, animMap[currAnim]->uvRect.height));
	body.setPosition(spawn);
	body.setTexture(animMap[currAnim]->getTexture());
	body.setTextureRect(animMap[currAnim]->uvRect);
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

	if (velocity.x == 0) {
		currAnim = "idle";
	}
	else {
		if(canJump) currAnim = "walk";

		turned = velocity.x < 0;
	}

	animMap[currAnim]->Update(deltaTime, velocity.x < 0);
	body.setTextureRect(animMap[currAnim]->uvRect);
	body.move(velocity * deltaTime);
}

void Player::onCollision(const Vector2f& direction) {

	if (direction.x != 0) velocity.x = 0; // √оризонтальна€ коллизи€
	if (direction.y != 0) {
		velocity.y = 0; // ¬ертикальна€ коллизи€
		canJump = direction.y < 0;
	}
	if(direction.y >= 0 && direction.x == 0)
		canJump = false;
	
}

void Player::Draw(RenderWindow& wnd) const {
	wnd.draw(body);
}

Vector2f Player::getPos() const {
	return body.getPosition();
}

float Player::getWeight() const {
	return weight;
}

void Player::setWeight(float weight) {
	this->weight = weight;
}
