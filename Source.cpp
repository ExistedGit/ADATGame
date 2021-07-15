#include "Animation.h"
#include "Player.h"
#include "Platform.h"
#include <vector>
const Clock cl;

using namespace std;

int main() {
	RenderWindow mainWindow(sf::VideoMode(1366, 768), "SFML");
	
	Texture* playerTexture = new Texture();
	playerTexture->loadFromFile("NewRatR.png");
	Texture* mirrorTexture = new Texture();
	mirrorTexture->loadFromFile("NewRatL.png");

	Texture* vanTexture = new Texture();
	vanTexture->loadFromFile("unnamed.jpg");

	Player player(playerTexture, Vector2u(4, 2), 0.4, 1366/1.5, 300, Vector2f(200, 200));
	vector<Platform> obstacles = { 
		Platform(nullptr, Vector2f(mainWindow.getSize().x, 1), Vector2f(mainWindow.getSize().x / 2, mainWindow.getSize().y)), // Снизу окна
		Platform(nullptr, Vector2f(mainWindow.getSize().x, 1), Vector2f(mainWindow.getSize().x / 2, 0)), // Снизу окна

		Platform(nullptr, Vector2f(1, mainWindow.getSize().y), Vector2f(0, mainWindow.getSize().y/2)), // Слева
		Platform(nullptr, Vector2f(1, mainWindow.getSize().y), Vector2f(mainWindow.getSize().x+1, mainWindow.getSize().y/2)) // Справа
	};
	//player.setTexture(playerTexture);
	
	//Player* player = new Player(playerTexture, mirrorTexture, 4, 190, mirrorTexture);
	//Stage mainStage(mainWindow, *player);

	//Object* vanObj = new Object(vanTexture, nullptr, 1, true, false, 0.1);
	//vanObj->setScale(0.3, 0.3);
	//vanObj->setPosition(200, 500);

	//mainStage.addObject(*vanObj);
	Clock clock;
	float deltaTime = 0;
	Event ev;
	while (mainWindow.isOpen()) {
		mainWindow.clear();

		deltaTime = clock.restart().asSeconds();
		if (deltaTime > 1. / 60.) {
			deltaTime = 1. / 60.;
		}

		while (mainWindow.pollEvent(ev)) {
			switch (ev.type) {
			case Event::Closed:
				mainWindow.close();
			}
		}
		player.Update(deltaTime);
		//player->Update(deltaTime);
		//vanObj->CheckCollision(player->getObj());
		//player->Draw(mainWindow);
		//mainWindow.draw(*vanObj);
		Vector2f direction;
		for (auto& p : obstacles) {
			if (p.getCollider().CheckCollision(player.getCollider(), direction, 1.0f)) {
				player.onCollision(direction);
			};
		}
		player.Draw(mainWindow);

		for (auto& p : obstacles) {
			p.Draw(mainWindow);
		}

		mainWindow.display();
	}
	return 0;
}