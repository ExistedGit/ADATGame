#pragma once
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
#include "SDK/Menu.h"
#include "SDK/SmartSprite.h"

using namespace std;
using namespace sf;

class World
{
private:
	View view;
	bool viewCentered = true;
	MusicPlayer mp;

	ContextSettings cs;
	RenderWindow wnd;
	vector<Level> levels;
	Player player;

	map<string, SmartSprite*> sprites;
	
	inline void initLevels() {
		levels[0].applyUseMap({
			{"playButton", [this]() mutable {
				if (!mp.play()) mp.pause();
			}
		} });

	}
public:
	

	inline World(const Vector2f& wndSize) :
		wnd(RenderWindow(sf::VideoMode(wndSize.x, wndSize.y), L"ВЫ — КРЫСА!", 7U, cs)),
		view(View(Vector2f(0, 0), wndSize)),
		levels(ConfigManager::loadLevels()),
		player(new Animation("Models/rat.xml"), Vector2f(191 / 2, 191 / 2), 650, 150, 1, Vector2f(200, 200)),
		mp({
		new Song("Never Gonna Give You Up", "Sounds/rickroll.ogg"),
		new Song("Gonna Give You Up",		 "Sounds/llorkcir.ogg")
			})
	{
		cs.antialiasingLevel = 8;

		sprites["menu_bg"] = new SmartSprite("Textures/menu_bg.png");
		sprites["hub_bg"] = new SmartSprite("Textures/hub_bg.jpg");
		initLevels();
	}

	inline void start() {
		view.setCenter(Vector2f(wnd.getSize() / 2u));

		Event ev;

		Menu mainMenu(Vector2f(wnd.getSize().x /2 - 200, wnd.getSize().y / 2+100));
		mainMenu.load("Models/menuButtons.xml", "Models/Menu/mainMenu.xml", 10);
		bool gameStarted = false;
		mainMenu.applyUseMap({
			{
			"start",
			[&gameStarted]() mutable { gameStarted = true; }
			},
			{
			"exit",
			[this]()mutable {wnd.close(); exit(0); }
			}
			});

		Font pixelFont;
		pixelFont.loadFromFile("Fonts/kongtext.ttf");
		Font hintFont;
		hintFont.loadFromFile("Fonts/forward.ttf");

		Clock clock;
		float deltaTime = 0;
		int currLevel = 0;

		Text songText("", pixelFont);
		songText.setPosition(200, 500);


		while (wnd.isOpen()) {
			while (!wnd.hasFocus()) {
				if (wnd.pollEvent(ev)) {
					if (ev.type == Event::GainedFocus) break;
				}
			}
			wnd.clear();
			sprites["hub_bg"]->setPosition(player.getPos().x - sprites["hub_bg"]->getTexture()->getSize().x/2, player.getPos().y - sprites["hub_bg"]->getTexture()->getSize().y / 2);
			wnd.draw(*sprites["hub_bg"]);

			if (gameStarted) {
				songText.setString("Now playing " + mp.getSongName() + (mp.getStatus() == Music::Paused || mp.getStatus() == Music::Stopped ? " (Paused)" : ""));

				if (Keyboard::isKeyPressed(Keyboard::Escape)) {
					gameStarted = false;
					
					view.setCenter(Vector2f(wnd.getSize() / 2u));
					wnd.setView(view);
					player.respawn(200, 200);
					continue;
				}

				if (Keyboard::isKeyPressed(Keyboard::Dash)) {
					mp.setVolume(mp.getVolume() - 1);
				}

				if (Keyboard::isKeyPressed((Keyboard::Key)55)) {
					mp.setVolume(mp.getVolume() + 1);
				}



				deltaTime = clock.restart().asSeconds();
				if (deltaTime > 1. / 60.) deltaTime = 1. / 60.;

				view.setCenter(Vector2f(player.getPos().x - 0.01, player.getPos().y));
				if (viewCentered) wnd.setView(view);

				while (wnd.pollEvent(ev)) {

					switch (ev.type) {
					case Event::Closed:

						wnd.close();
						return;
						break;
					case Event::Resized:
						view.setSize(Vector2f(wnd.getSize()));//VIEW_HEIGHT * (float(wnd.getSize().x) / float(wnd.getSize().y)), VIEW_HEIGHT);
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
							mp.CheckClick(ev, wnd, view);
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
				//		levels[switched].getObjects()[i].Draw(wnd);
				//	}
				//}

				levels[currLevel].Update(player);
				levels[currLevel].Draw(wnd, &player);
				levels[currLevel].drawHint(wnd, player, pixelFont);

				mp.drawButtons(wnd);
				wnd.draw(songText);
			}
			else {
				wnd.draw(*sprites["menu_bg"]);
				mainMenu.drawButtons(wnd);
				if (wnd.pollEvent(ev)) {
					mainMenu.CheckClick(ev, wnd, view);
					if (ev.type == Event::Closed) {
						wnd.close();
						exit(0);
					}
				}
				
			}

			wnd.display();
		}
	}
};

