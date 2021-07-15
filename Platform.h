#pragma once
#include "Collider.h"

class Platform
{
private:
	RectangleShape body;
public:
	Platform(Texture* text, Vector2f size, Vector2f pos) {
		body.setTexture(text);
		body.setSize(size);
		body.setOrigin(size/2.0f);
		body.setPosition(pos);
	}

	Collider getCollider() {
		return Collider(body);
	}

	void Draw(RenderWindow& wnd) {
		wnd.draw(body);
	}
};

