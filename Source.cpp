#include "Animation.h"
#include "Player.h"
#include "Object.h"
#include <vector>
#include "Level.h"
#include <map>

const Clock cl;

using namespace std;

#define VIEW_HEIGHT 768


int main() {
#pragma region Инициализация


	vector<Level> levels = {};

	View view(Vector2f(0, 0), Vector2f(1366,768));
	RenderWindow mainWindow(sf::VideoMode(1366, 768), "SFML");
	
	Texture* playerTexture = new Texture();
	playerTexture->loadFromFile("Textures/NewRatR.png");
	//Texture* mirrorTexture = new Texture();
	//mirrorTexture->loadFromFile("Textures/NewRatL.png");

	//Texture* vanTexture = new Texture();
	//vanTexture->loadFromFile("Textures/unnamed.jpg");

	Player player(playerTexture, Vector2u(4, 2), 0.15, 600, 150, 1, Vector2f(200, 200));
	Vector2u mapSize;
	
	levels.push_back(Level().load("TileMap/untitled.tmx", Vector2f(32*4 + 1, 767), &mainWindow));
	levels.push_back(Level().load("TileMap/other map.tmx", Vector2f(1, 767), &mainWindow));

	
	
#pragma endregion

	Clock clock;
	float deltaTime = 0;
	Event ev;
	
	bool switched = false;

	while (mainWindow.isOpen()) {
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			mainWindow.close();
			return 0;
		}
		

		
		mainWindow.clear();

		deltaTime = clock.restart().asSeconds();
		if (deltaTime > 1. / 60.) {
			deltaTime = 1. / 60.;
		}

		while (mainWindow.pollEvent(ev)) {
			switch (ev.type) {
			case Event::Closed:
				mainWindow.close();
				return 0;
				break;
			case Event::Resized:
				view.setSize(VIEW_HEIGHT * (float(mainWindow.getSize().x) / float(mainWindow.getSize().y)), VIEW_HEIGHT);
				break;
			case Event::KeyReleased:
				if (ev.key.code == Keyboard::X) {
					switched = !switched;
				}
				break;
			}
		}
		
		player.Update(deltaTime);

		Vector2f direction;

		bool groundObj = false;
		for (auto& p : levels[switched].getObjects()) {
			if (p.getCollider().CheckCollision(player.getCollider(), direction, !p.push)) {
				player.onCollision(direction);
				if (direction.y == -1) groundObj = true;
			};
		}
		if (!groundObj) player.onCollision(direction);

		// Отрисовка белых границ окна. Можно откомментировать когда-нибудь
		for (int i = 0; i < 4; i++) {
			levels[switched].getObjects()[i].Draw(mainWindow);
		}

		levels[switched].Draw(mainWindow, &player);

		view.setCenter(player.getPos());
		mainWindow.setView(view);

		mainWindow.display();
	}
	return 0;
}