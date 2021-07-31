#include "SDK/Animation.h"
#include "SDK/Player.h"
#include "SDK/Object.h"
#include <vector>
#include "SDK/Level.h"
#include "SDK/Button.h"
#include <iostream>

#include <SFML/Audio.hpp>
#include "SDK/ConfigManager.h"
#include <Windows.h>

const Clock cl;

using namespace std;

#define VIEW_HEIGHT 1080



int main() {
	SetConsoleCP(65001); SetConsoleOutputCP(65001);
#pragma region Инициализация
	cout.setf(ios::boolalpha);
	
	View view(Vector2f(0, 0), Vector2f(1920, 1080));
	RenderWindow mainWindow(sf::VideoMode(1920, 1080), L"ВЫ — КРЫСА!");
	vector<Level> levels = ConfigManager::loadLevels(&mainWindow);
	//vector<Level> levels = { Level().load("TileMap/untitled.tmx", "TileMap/tileset.png", {32, 32}, Vector2f(1, 1079), &mainWindow) };


	Player player(new Animation("Models/rat.xml"), Vector2f(191/2, 191/2), 650, 150, 1, Vector2f(200, 200));

	MusicPlayer mp({
		new Song("Never Gonna Give You Up", "Sounds/rickroll.ogg"),
		new Song("Gonna Give You Up",		 "Sounds/llorkcir.ogg")
		});
	
	bool viewCentered = true;
	//levels[0].applyUseMap({
	//		{"playButton", [&mp]() mutable {
	//			if (!mp.play()) mp.pause();
	//		}
	//	} });
	//levels[1].applyUseMap({
	//		{"lever", [&viewCentered]() mutable {
	//			viewCentered = !viewCentered;
	//		}
	//		}
	//	});


	Font pixelFont;
	pixelFont.loadFromFile("Fonts/kongtext.ttf");
	Font hintFont;
	hintFont.loadFromFile("Fonts/forward.ttf");
#pragma endregion

	

	Clock clock;
	float deltaTime = 0;
	Event ev;
	int currLevel = 0;
	
	Text songText("", pixelFont);
	songText.setPosition(200, 500);

	while (mainWindow.isOpen()) {
		while (!mainWindow.hasFocus()) {
			if (mainWindow.pollEvent(ev)) {
				if (ev.type == Event::GainedFocus) break;
			}
		}

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

		view.setCenter(player.getPos());
		if(viewCentered) mainWindow.setView(view);

		while (mainWindow.pollEvent(ev)) {

			switch (ev.type) {
			case Event::Closed:

				mainWindow.close();
				return 0;
				break;
			case Event::Resized:
				view.setSize(Vector2f(mainWindow.getSize()));//VIEW_HEIGHT * (float(mainWindow.getSize().x) / float(mainWindow.getSize().y)), VIEW_HEIGHT);
				break;
			case Event::KeyPressed: {}
				break;
			case Event::KeyReleased: {
				switch (ev.key.code) {
				case Keyboard::X:
					currLevel = currLevel == levels.size() - 1 ? 0 : currLevel + 1;
					break;
				case Keyboard::M:
					mp.setVolume(mp.muted() ? 100 : 0);
					break;
				case Keyboard::S:
					mp.setPosition(0);
					break;
				}
				}
			case Event::MouseButtonReleased:
				if (ev.mouseButton.button == Mouse::Button::Left) {
					mp.CheckClick(ev, mainWindow, view);
				}
				break;
			}
			levels[currLevel].checkInteraction(ev, player);
		}

		player.Update(deltaTime);

		Vector2f direction;

		bool groundCollision = false;
		for (auto& p : levels[currLevel].getObjects()) {
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

		levels[currLevel].Update(player);
		levels[currLevel].Draw(mainWindow, &player);
		levels[currLevel].drawHint(player, mainWindow, pixelFont);
		
		mp.draw(mainWindow);
		mainWindow.draw(songText);
		
		mainWindow.display();
	}
	return 0;
}