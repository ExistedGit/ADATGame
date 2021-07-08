#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <Windows.h>
using namespace std;
using namespace sf;

Shader rgbShader; //dankozz1

class animatedSprite : public Sprite {
private:
	unsigned int animFrames = 0;
	unsigned int currFrame = 0;

	RenderWindow* _window=nullptr;
	bool _ifDraw = true;

	bool mirrored;
public:
	unsigned int width;
	unsigned int height;
	void move(const Vector2f& v2f) {
		sf::Sprite::move(v2f);
		_ifDraw = true;
	}
	bool ifDraw() {
		return _ifDraw;
	}

	animatedSprite& drawn() {
		_ifDraw = false;
		return *this;
	}

	void setWindow(sf::RenderWindow& w) {
		_window = &w;
	}

	unsigned int getAnimFrames() {
		return animFrames;
	}
	void setAnimFrames(unsigned int af) {
		animFrames = af;
	}
	void setCurrFrame(unsigned int cf) {
		currFrame = cf;
		setTextureRect(IntRect(currFrame * width, 0, width, height));
		_ifDraw = true;
	}
	void nextFrame() {
		if (currFrame < animFrames - 1)currFrame++;
		else currFrame = 0;
		setTextureRect(IntRect(currFrame * width, 0, width, height));
		_ifDraw = true;
	}

	unsigned int getCurrFrame() {
		return currFrame;
	}

	void mirror() {
		if (!mirrored) {
			setOrigin(getLocalBounds().width, 0);
			setScale(-1, 1);
		}
		else {
			setOrigin(0, 0);
			setScale(1, 1);
		}
		mirrored = !mirrored;
	}
};



// Если мышка находится внутри прямоугольника
template <typename T>
bool mouseIntersects(const Rect<T>& rs, const RenderWindow& window) {
	return rs.contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y);
}


// Если мышка находится внутри текстуры спрайта
bool mouseIntersects(const Sprite& sprite, const RenderWindow& window) {
	return Rect<float>(sprite.getTextureRect().left + sprite.getPosition().x, sprite.getTextureRect().top + sprite.getPosition().y, sprite.getTextureRect().width * sprite.getScale().y, sprite.getTextureRect().height * sprite.getScale().y)
		.contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y);
}
// Если мышка находится внутри текстуры спрайта
bool mouseIntersects(const animatedSprite& sprite, const RenderWindow& window) {
	int x = sprite.getTextureRect().width * sprite.getScale().y;
	return Rect<float>(sprite.getPosition().x, sprite.getPosition().y, sprite.getTextureRect().width * sprite.getScale().y, sprite.getTextureRect().height * sprite.getScale().y).contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y);
}
void fillSprite(Uint32 r, Uint32 g, Uint32 b) {

	string fuck =
		"uniform sampler2D texture;                             " \
		"                                                       " \
		"void main() {                                          " \
		"    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);" \
		"                                                       " \
		"    pixel.r = ";
	fuck.append(to_string(r));
	fuck.append(".0f;                                    " \
		"    pixel.g = ");
	fuck.append(to_string(g));
	fuck.append(".0f;                                    " \
		"    pixel.b = ");
	fuck.append(to_string(b));
	fuck.append(".0f;                                    " \
		"                                                       " \
		"    gl_FragColor = pixel;                              " \
		"}                                                      ");

	rgbShader.loadFromMemory(fuck, sf::Shader::Fragment);
}
void drawWithOutline(const Sprite& sprite, RenderWindow& window, unsigned int thickness, Color color = Color(0, 0, 0)) {
	Sprite newSprite = sprite;
	
	fillSprite(color.r, color.g, color.b);

	//float sss = thickness / (sprite.getTextureRect().width * sprite.getScale().x);

	newSprite.setScale({ sprite.getScale().x + (thickness*2) / (sprite.getTextureRect().width * sprite.getScale().x), sprite.getScale().y + (thickness * 2) / (sprite.getTextureRect().height * sprite.getScale().y) });
	newSprite.move(Vector2f(-int(thickness), -int(thickness)));
	window.draw(newSprite, &rgbShader);
	window.draw(sprite);
}

void animationCycle(animatedSprite& sprite) {
	Clock clock;
	while (true) {
		if (int(clock.getElapsedTime().asSeconds() / 0.5) == clock.getElapsedTime().asSeconds() / 0.5) {
			cout << clock.getElapsedTime().asSeconds() << endl;
			sprite.nextFrame();
		}
	}
}

class World {
private:
	RenderWindow* _window;
	float acceleration = 1; //
public:
	float gq;// gravity quatient
	World(RenderWindow& window, float _gq = 0.2) {
		_window = &window;
		gq = _gq;
	}

	void gravity(animatedSprite& s) {
		while (true) {
			if (_window->isOpen()) {
				if (gq > 0) {
					if (s.getPosition().y + s.getTextureRect().height * abs(s.getScale().y) <= _window->getSize().y) {
						s.move(Vector2f(0, gq* acceleration));
						acceleration += 0.098;
						Sleep(10);
					}
					else acceleration = 0;
				}
				else if (gq < 0) {
					
					if (s.getPosition().y >= 0) {
						s.move(Vector2f(0, gq * acceleration));
						acceleration += 0.098;
						Sleep(10);
					}
					else acceleration = 0;
				}
			}
		}
	}

	
};

int main()
{
	fillSprite(255, 255, 255);
	
	ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(1366, 768), "", sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close, settings);
	window.setVerticalSyncEnabled(true);

	animatedSprite sprite;

	World main(window);

	Color spriteColor = sprite.getColor();
	{
		Texture* van = new Texture();
		van->loadFromFile("RatAnim.png");
		
		sprite.setTexture(*van);
		sprite.setTextureRect(IntRect(0, 0, 191, 192));
		sprite.setAnimFrames(3);
		sprite.setWindow(window);
		sprite.width = 191;
		sprite.height = 192;
		//sprite.setScale({ 0.3, 0.3 });
	}

	thread gravity(&World::gravity, &main, ref(sprite));
	gravity.detach();

	bool fullscreen = false;

	int animFrame = 0;
	
	sf::Event event;


	bool ratPressed = false;

	Clock cl;
	float lastAnimTime = 0;
	

	window.draw(sprite);

	while (window.isOpen())
	{
		if (sprite.ifDraw()) {
			window.clear();
			window.draw(sprite.drawn());
			window.display();
		}
		while (window.pollEvent(event))
		{
			
			switch (event.type) {
			case Event::Closed:
				window.close();
				break;
			case Event::KeyPressed: {
				
				if (event.key.code == Keyboard::Escape && fullscreen) {
					fullscreen = !fullscreen;
					window.close();
					window.create(sf::VideoMode(1920, 1080), "SFML works!", sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close);
				}
				else if (event.key.code == Keyboard::F11 && !fullscreen) {
					fullscreen = !fullscreen;
					window.close();
					window.create(sf::VideoMode(1920, 1080), "SFML works!", sf::Style::Fullscreen | sf::Style::Titlebar | sf::Style::Close);
				}

				// Стрелочки
				
				float offset = 6;
				if (event.key.code >= 71 && event.key.code <= 74) {
					if (cl.getElapsedTime().asSeconds() - lastAnimTime >= 0.1) {
						lastAnimTime = cl.getElapsedTime().asSeconds();
						sprite.nextFrame();
					}

					if (event.key.code == Keyboard::Right) {
						if (sprite.getScale().x == -1) sprite.mirror();
						if (sprite.getPosition().x < window.getSize().x - sprite.getTextureRect().width * sprite.getScale().x) {
							sprite.move({ offset, 0 });
						}
					}
					if (event.key.code == Keyboard::Left) {
						if (sprite.getScale().x != -1) sprite.mirror();
						if (sprite.getPosition().x > 0) {
							sprite.move({ -offset, 0 });
						}
					}
					if (event.key.code == Keyboard::Up) {
						if (sprite.getPosition().y > 0) {
							sprite.move({ 0, -offset });
						}
					}
					if (event.key.code == Keyboard::Down) {
						if (sprite.getPosition().y < window.getSize().y - sprite.getTextureRect().height * sprite.getScale().y) {
							sprite.move({ 0, offset });
						}
					}
					window.clear();
					window.draw(sprite);
					window.display();
				}


				if (event.key.code == 56) {
					main.gq++;
				}
				else if (event.key.code == 55) {
					main.gq--;
				}
				break;
			}
			case Event::KeyReleased:

				if (event.key.code >= 71 && event.key.code <= 74) {
					sprite.setCurrFrame(0);
				}
				break;
			case Event::MouseMoved:
				ratPressed = false;
				if (Mouse::isButtonPressed(Mouse::Button::Left)) {
					if(mouseIntersects(sprite, window)) ratPressed = true;
				};
				break;
			case Event::MouseButtonReleased:
				ratPressed = false;
				break;
			case Event::MouseButtonPressed: {
				
				ratPressed = mouseIntersects(sprite, window);
				if (ratPressed) {
					
					sprite.nextFrame();
				}
				break;
			}
			}

			if (ratPressed) {
				window.clear();
				drawWithOutline(sprite, window, 5, Color::Yellow);
				window.setTitle(L"Меня зовут Ван. Я артист, исполняющий артист. Нанимаемый людьми, чтобы исполнить их глубокие, тёмные фантазии");
			}

			
		}
		window.display();
	}
	return 0;
}