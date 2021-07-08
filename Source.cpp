#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <thread>
#include "World.h"
using namespace std;
using namespace sf;

const Clock cl;

Shader rgbShader;
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
bool mouseIntersects(const AnimatedSprite& sprite, const RenderWindow& window) {
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
	newSprite.setScale({ sprite.getScale().x + (thickness*2) / (sprite.getTextureRect().width * sprite.getScale().x), sprite.getScale().y + (thickness * 2) / (sprite.getTextureRect().height * sprite.getScale().y) });
	newSprite.move(Vector2f(-int(thickness), -int(thickness)));
	window.draw(newSprite, &rgbShader);
	window.draw(sprite);
}

//void animationCycle(AnimatedSprite& sprite) {
//	Clock clock;
//	while (true) {
//		if (int(clock.getElapsedTime().asSeconds() / 0.5) == clock.getElapsedTime().asSeconds() / 0.5) {
//			cout << clock.getElapsedTime().asSeconds() << endl;
//			sprite.nextFrame();
//		}
//	}
//}



int main()
{
	fillSprite(255, 255, 255);
	

	ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(1366, 768), "", sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close, settings);
	window.setVerticalSyncEnabled(true);


	AnimatedSprite sprite;
	World mainWorld(window);
	{
		Texture* texture = new Texture();
		texture->loadFromFile("RatAnim.png");
		
		sprite.setTexture(*texture);
		sprite.setTextureRect(IntRect(0, 0, 191, 192));
		sprite.setAnimFrames(4);
		sprite.setWindow(window);
		sprite.width = 191;
		sprite.height = 192;
	}
	Player player(sprite);
	player.setLegHitbox(IntRect(sprite.getTextureRect().width * sprite.getScale().x / 1.5, sprite.getTextureRect().height * sprite.getScale().y * 0.8, sprite.getTextureRect().width * sprite.getScale().x / 3, sprite.getTextureRect().height * sprite.getScale().y * 0.2));
	player.moveSpeed = 5;
	player.setWindow(window);

	thread gravity(&World::gravity, &mainWorld, ref(player));
	gravity.detach();

	bool fullscreen = false;

	int animFrame = 0;
	
	sf::Event event;


	bool ratPressed = false;

	

	window.draw(sprite.drawn());

	while (window.isOpen())
	{
		if (sprite.ifDraw()) {
			window.clear();
			mainWorld.drawObstacles();
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
				
				if (event.key.code >= 71 && event.key.code <= 74) {
					player.move(event.key.code);
					window.clear();
					mainWorld.drawObstacles();
					window.draw(sprite.drawn());
					window.display();
				}


				if (event.key.code == 55) {
					mainWorld.gq+=0.1;
				}
				else if (event.key.code == 56) {
					mainWorld.gq-=0.1;
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
				mainWorld.drawObstacles();
				drawWithOutline(sprite, window, 5, Color::Yellow);
				window.setTitle(L"Меня зовут Ван. Я артист, исполняющий артист. Нанимаемый людьми, чтобы исполнить их глубокие, тёмные фантазии");
			}

			
		}
		window.display();
	}
	return 0;
	
}