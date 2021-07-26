#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;


class Animation
{
protected:
	Vector2u frameCount;
	Vector2u currFrame;
	Texture* texture;
	
	float switchTime = 0;
	float totalTime = 0;
	
	unsigned int _row = 0;
public:
	IntRect uvRect;

	Animation(Texture* text, Vector2u frameCount, float switchTime);;

	void Update(unsigned int row, float deltaTime, bool mirrored);
	
	unsigned int getRow() const;
	//void setScale(Vector2f scale) {
	//	Sprite* sp = new Sprite();
	//	sp->setTexture(*texture);
	//	sp->setScale(scale);
	//	r->clear();
	//	r->create(texture->getSize().x * scale.x, texture->getSize().y * scale.y);
	//	r->draw(*sp);
	//	r->display();

	//	delete texture;
	//	texture = new Texture(r->getTexture());
	//	uvRect.height *= scale.y;
	//	uvRect.width *= scale.x;
	//	
	//	//delete sp;
	//	//sp = nullptr;
	//}
};

