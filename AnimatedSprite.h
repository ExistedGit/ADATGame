#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

extern const Clock cl;

class AnimatedSprite : public Sprite
{
protected:
	unsigned int currentFrame;
	unsigned int totalFrames;
	unsigned int frameWidth;
	unsigned int frameHeight;
	float lastAnimTime = 0;

	bool _mirrored = false;

	Texture* texture = nullptr;
	Texture* mirrorTexture = nullptr;
public:
	AnimatedSprite(Texture* t, Texture* mirrorTexture, unsigned int frames);
	
	void setMirrorTexture(Texture* t) {
		mirrorTexture = t;
	}

	float getLastAnimTime();;
	void setFrame(unsigned int frame);
	void nextFrame();

	bool mirrored() {
		return _mirrored;
	}

	void mirror() {
		if (!_mirrored) {
			if (mirrorTexture != nullptr) {
				setTexture(*mirrorTexture);
				setFrame(currentFrame);
			}
			else {
				setScale(-1, 1);
				move(frameWidth,0);
			}
		}
		else {
			setTexture(*texture);
			setFrame(currentFrame);
		}
		_mirrored = !_mirrored;
	}
};

