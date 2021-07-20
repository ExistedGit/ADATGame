#include "SDK/Animation.h"
#include "SDK/Player.h"
#include "SDK/Object.h"
#include <vector>
#include "SDK/Level.h"
#include "SDK/Button.h"
#include <iostream>

#include <SFML/Audio.hpp>

const Clock cl;

using namespace std;

#define VIEW_HEIGHT 1080



int main() {
#pragma region Инициализация
	vector<Level> levels = {};

	View view(Vector2f(0, 0), Vector2f(1920, 1080));
	RenderWindow mainWindow(sf::VideoMode(1920, 1080), L"ВЫ — КРЫСА!");
	
	Texture* playerTexture = new Texture();
	playerTexture->loadFromFile("Textures/NewRatR.png");

	Player player(playerTexture, Vector2u(4, 2), 0.15, 600, 150, 1, Vector2f(200, 200));
	
	levels.push_back(Level().load("TileMap/untitled.tmx", Vector2f(1, 767), &mainWindow));
	levels.push_back(Level().load("TileMap/other map.tmx", Vector2f(32 * 4 + 1, 767), &mainWindow));


	Font pixelFont;
	pixelFont.loadFromFile("Fonts/kongtext.ttf");
#pragma endregion


	Clock clock;
	float deltaTime = 0;
	Event ev;
	
	bool switched = false;


	Texture* buttonTexture = new Texture();
	buttonTexture->loadFromFile("Textures/button.png");
	
	MusicPlayer mp({
		{ "Never Gonna Give You Up", "Sounds/rickroll.ogg"},
		{ "Gonna Give You Up",		 "Sounds/llorkcir.ogg"}
	});
	Text songText("", pixelFont);
	songText.setPosition(200, 200);

	while (mainWindow.isOpen()) {
		while (!mainWindow.hasFocus()) {}

		songText.setString("Now playing " + mp.getSongName() + (mp.getStatus() == Music::Paused || mp.getStatus() == Music::Stopped ? " (Paused)" : ""));

		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			mainWindow.close();
			return 0;
		}

		if (Keyboard::isKeyPressed(Keyboard::Dash)) {
			mp.setVolume(mp.getVolume() - 1);
		}

		if (Keyboard::isKeyPressed((Keyboard::Key)55)) {
			mp.setVolume(mp.getVolume() + 1);
		}
		

		mainWindow.clear();

		deltaTime = clock.restart().asSeconds();
		if (deltaTime > 1. / 60.) deltaTime = 1. / 60.;
		

		while (mainWindow.pollEvent(ev)) {
			switch (ev.type) {
			case Event::Closed:
				
				mainWindow.close();
				return 0;
				break;
			case Event::Resized:
				view.setSize(Vector2f(mainWindow.getSize()));//VIEW_HEIGHT * (float(mainWindow.getSize().x) / float(mainWindow.getSize().y)), VIEW_HEIGHT);
				break;
			case Event::KeyReleased:
				switch(ev.key.code) {
				case Keyboard::X:
					switched = !switched;
					mp.next();
					break;
				case Keyboard::M:
					mp.setVolume(mp.muted() ? 100 : 0);
					break;
				case Keyboard::S:
					mp.setPosition(0);
					break;
				}
				break;
			case Event::MouseButtonReleased:
				if (ev.mouseButton.button == Mouse::Button::Left) {
					mp.CheckClick(ev, mainWindow, view);
				}
				break;
			}
		}
		
		player.Update(deltaTime);

		Vector2f direction;

		bool groundCollision = false;
		for (auto& p : levels[switched].getObjects()) {
			if (p.getCollider().CheckCollision(player.getCollider(), direction, !p.push)) {
				player.onCollision(direction);
				if (direction.y == -1) groundCollision = true;
			};
		}
		if (!groundCollision) player.onCollision(direction);

		//// Отрисовка белых границ окна. Можно откомментировать когда-нибудь
		//if (levels[switched].bordered()) {
		//	for (int i = 0; i < 4; i++) {
		//		levels[switched].getObjects()[i].Draw(mainWindow);
		//	}
		//}

		levels[switched].Draw(mainWindow, &player);

		view.setCenter(player.getPos());
		mainWindow.setView(view);

		mp.draw(mainWindow);
		mainWindow.draw(songText);

		mainWindow.display();
	}
	return 0;
}