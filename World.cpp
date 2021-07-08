#include "World.h"

// gravity quatient

World::World(RenderWindow& window, float _gq) {
	_window = &window;
	gq = _gq;

	Texture* txt = new Texture();
	txt->loadFromFile("unnamed.jpg");
	AnimatedSprite* sp = new AnimatedSprite();
	sp->setTexture(*txt);
	sp->setScale(0.3, 0.2);
	//sp->setPosition(0, 0);
	sp->move(Vector2f(100, 768 - sp->getTextureRect().height * sp->getScale().y));
	obstacles.push_back(sp);

}

void World::gravity(Player& p) {
	bool windowOpen = true;
	AnimatedSprite& s = p.getSprite();
	while (windowOpen) {
		try {
			if (_window != nullptr && _window->isOpen()) {
				if (gq > 0) {
					if (s.getPosition().y + s.getTextureRect().height * abs(s.getScale().y) <= _window->getSize().y &&
						checkCollision(s)[DOWN] == nullptr) {
						s.move(Vector2f(0, gq * acceleration));
						acceleration += 0.198;
						Sleep(10);
					}
					else acceleration = 1;
				}
				else if (gq < 0) {
					
					if (s.getPosition().y >= 0 && checkCollision(s)[UP] == nullptr) {
						s.move(Vector2f(0, gq * acceleration));
						acceleration += 0.198;
						Sleep(10);
					}
					else acceleration = 1;
				}
				else acceleration = 1;

			}
		}
		catch (...) {
			windowOpen = false;
		}
	}
}

void World::drawObstacles() {
	try {
		for (auto i = obstacles.begin(); i < obstacles.end(); i++) {
			_window->draw(**i);
		}
	}
	catch (...) {};
}

std::map<Direction, AnimatedSprite*> World::checkCollision(AnimatedSprite& sp) {
	std::map<Direction, AnimatedSprite*> cmap = { {UP, nullptr}, {RIGHT, nullptr}, {DOWN, nullptr}, {LEFT, nullptr} };

	for (auto i = obstacles.begin(); i < obstacles.end(); i++) {
		if (IntRect((**i).getPosition().x, (**i).getPosition().y, (**i).getTextureRect().width * (**i).getScale().x, (**i).getTextureRect().height * (**i).getScale().y)
			.contains(sp.getPosition().x + sp.getTextureRect().width * sp.getScale().x + 1, sp.getPosition().y + sp.getTextureRect().height * sp.getScale().y / 2)) {
			cmap[RIGHT] = *i;
		};
	};
	for (auto i = obstacles.begin(); i < obstacles.end(); i++) {
		if (IntRect((**i).getPosition().x, (**i).getPosition().y, (**i).getTextureRect().width * (**i).getScale().x, (**i).getTextureRect().height * (**i).getScale().y)
			.contains(sp.getPosition().x - 1, sp.getPosition().y + sp.getTextureRect().height * sp.getScale().y / 2)) {
			cmap[LEFT] = *i;
		};
	};
	for (auto i = obstacles.begin(); i < obstacles.end(); i++) {
		if (IntRect((**i).getPosition().x, (**i).getPosition().y, (**i).getTextureRect().width * (**i).getScale().x, (**i).getTextureRect().height * (**i).getScale().y)
			.contains(sp.getPosition().x + sp.getTextureRect().width * sp.getScale().x / 2, sp.getPosition().y - 1)) {
			cmap[UP] = *i;
		};
	};
	for (auto i = obstacles.begin(); i < obstacles.end(); i++) {
		if (IntRect((**i).getPosition().x, (**i).getPosition().y, (**i).getTextureRect().width * (**i).getScale().x, (**i).getTextureRect().height * (**i).getScale().y)
			.contains(sp.getPosition().x + sp.getTextureRect().width * sp.getScale().x / 2, sp.getPosition().y + sp.getTextureRect().height * sp.getScale().y + 1)) {
			cmap[DOWN] = *i;
		};
	};
	return cmap;
}
