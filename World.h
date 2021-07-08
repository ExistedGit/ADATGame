#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <Windows.h>
#include "AnimatedSprite.h"
#include "Player.h"


using namespace sf;




class World {
private:

	RenderWindow* _window = nullptr;
	float acceleration = 1; //
	
public:
	std::vector<AnimatedSprite*> obstacles;
	float gq;// gravity quatient

	World(RenderWindow& window, float _gq = 0.2);

	void gravity(Player& p);

	void drawObstacles();
	std::map<Direction, AnimatedSprite*> checkCollision(AnimatedSprite& sp);

	void movePlayer(Keyboard::Key key) {
		
	}
};
