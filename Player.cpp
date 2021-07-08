#include "Player.h"

Player::Player() {
	_sprite = nullptr;
	legHitbox = IntRect(0, 0, 0, 0);
}

Player::Player(AnimatedSprite& sprite) {
	_sprite = &sprite;
	legHitbox = sprite.getTextureRect();
}

void Player::setWindow(RenderWindow& w) {
	_window = &w;
}

IntRect Player::getLegHitbox() {
	return legHitbox;
}

void Player::setLegHitbox(const IntRect& rect) {
	legHitbox = rect;
}

AnimatedSprite& Player::getSprite() {
	return*_sprite;
}

void Player::move(Keyboard::Key key) {
	if (cl.getElapsedTime().asSeconds() - _sprite->getLastAnimTime() >= 0.1) {
		_sprite->nextFrame();
	}

	if (key == Keyboard::Right) {
		if (_sprite->getScale().x == -1) _sprite->mirror();
		if (_sprite->getPosition().x < _window->getSize().x - _sprite->getTextureRect().width * _sprite->getScale().x) {
			_sprite->move(Vector2f(moveSpeed, 0));
		}
	}
	if (key == Keyboard::Left) {
		if (_sprite->getScale().x != -1) _sprite->mirror();
		if (_sprite->getPosition().x > 0) {
			_sprite->move(Vector2f(-moveSpeed, 0));
		}
	}
	if (key == Keyboard::Up) {
		if (_sprite->getPosition().y > 0) {
			_sprite->move(Vector2f(0, -moveSpeed));
		}
	}
	if (key == Keyboard::Down) {
		if (_sprite->getPosition().y < _window->getSize().y - _sprite->getTextureRect().height * _sprite->getScale().y) {
			_sprite->move(Vector2f(0, moveSpeed));
		}
	}
}
