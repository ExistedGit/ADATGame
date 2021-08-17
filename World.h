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

	std::map<string, Level> levels;
	vector<string> levelNames;
	int currLevel = 0;

	Player player;

	map<string, SmartSprite*> sprites;
	
	inline void initLevels() {
		levels["lvl1"].applyUseMap({
			{"playButton", [this]() mutable {
				if (!mp.play()) mp.pause();
			}
		} });
	}

	enum class MenuState : int {
		MAIN, INGAME, NO_MENU
	};
public:
	

	inline World(const Vector2f& wndSize) :
		wnd(RenderWindow(sf::VideoMode(wndSize.x, wndSize.y), L"ВЫ — КРЫСА!", Style::Titlebar | Style::Close , cs)),
		view(View(Vector2f(wndSize.x, wndSize.y), wndSize)),
		levels(ConfigManager::loadLevels()),
		player(new Animation("Models/rat.xml"), Vector2f(191 / 2, 191 / 2), 650, 210,0.1),
		mp({
		new Song("Never Gonna Give You Up", "Sounds/rickroll.ogg"),
		new Song("Gonna Give You Up",		 "Sounds/llorkcir.ogg")
			})
	{
		cs.antialiasingLevel = 8;

		sprites["menu_bg"] = new SmartSprite("Textures/menu_bg.png"); 
		sprites["menu_bg"]->setPosition(Vector2f(wnd.getSize()/2u));
		sprites["ingame_menu_bg"] = new SmartSprite("Textures/ingame_menu_bg.png");
		sprites["ingame_menu_bg"]->setPosition(Vector2f(wnd.getSize() / 2u));

		sprites["hub_bg"] = new SmartSprite("Textures/hub_bg.jpg");
		
		for (const auto& it : levels) 
			levelNames.push_back(it.first);
		
		player.respawn(
			levels[levelNames[currLevel]].spawn.x, 
			levels[levelNames[currLevel]].spawn.y
		);

		initLevels();
	}

	inline void start() {
		view.setCenter(Vector2f(wnd.getSize() / 2u));

		Event ev;
		MenuState menuState = MenuState::MAIN;

		Menu mainMenu(Vector2f(wnd.getSize().x /2 - 200, wnd.getSize().y / 2+100));
		mainMenu.load("Models/menuButtons.xml", "Models/Menu/mainMenu.xml", 10);
		mainMenu.applyUseMap({
			{
			"start",
			[this, &menuState]() mutable { 
				player.respawn(levels[levelNames[currLevel]].spawn.x, levels[levelNames[currLevel]].spawn.y);
				menuState = MenuState::NO_MENU;
				
			}
			},
			{
			"exit",
			[this]()mutable {
				mp.stop();
				wnd.close();
				exit(0);
			}	
			}
			});
		Menu ingameMenu(Vector2f(wnd.getSize().x / 2, wnd.getSize().y / 2), true);
		ingameMenu.load("Models/menuButtons.xml", "Models/Menu/ingameMenu.xml", 10);
		ingameMenu.applyUseMap({
			{
			"continue",
			[this, &menuState]() mutable { 
				menuState = MenuState::NO_MENU; 
				if(mp.isInterrupted()) mp.play();
			}
			},
			{
			"menu",
			[this, &menuState]() mutable {
				mp.stop();
				view.setCenter(Vector2f(wnd.getSize() / 2u)); 
				wnd.setView(view); 
				menuState = MenuState::MAIN; 
			}
			}
			});

		Font pixelFont;
		pixelFont.loadFromFile("Fonts/kongtext.ttf");
		Font hintFont;
		hintFont.loadFromFile("Fonts/forward.ttf");

		Clock clock;
		float deltaTime = 0;

		Text songText("", pixelFont);
		songText.setPosition(200, 500);
		while (wnd.isOpen()) {
			while (!wnd.hasFocus())
				while (wnd.pollEvent(ev)) 
					if (ev.type == Event::GainedFocus) break;
				
			wnd.clear();
			sprites["hub_bg"]->setPosition(view.getCenter());
			wnd.draw(*sprites["hub_bg"]);

			
			if (menuState != MenuState::MAIN) {
				songText.setString("Now playing " + mp.getSongName() + (mp.getStatus() == Music::Paused || mp.getStatus() == Music::Stopped ? " (Paused)" : ""));
				
				if (menuState != MenuState::INGAME) {
					view.setCenter(Vector2f(player.getPos().x - 0.01, player.getPos().y));

					if (view.getCenter().x - wnd.getSize().x / 2 < 0)
						view.move(-(view.getCenter().x - wnd.getSize().x / 2) - 0.01, 0);
					else if (view.getCenter().x + wnd.getSize().x / 2 > levels[levelNames[currLevel]].getSize().x)
						view.move((levels[levelNames[currLevel]].getSize().x - (view.getCenter().x + wnd.getSize().x / 2)) - 0.01, 0);
					if (view.getCenter().y - wnd.getSize().y / 2 < 0)
						view.move(0, -(view.getCenter().y - wnd.getSize().y / 2));
					else if (view.getCenter().y + wnd.getSize().y / 2 > levels[levelNames[currLevel]].getSize().y)
						view.move(0, (levels[levelNames[currLevel]].getSize().y - (view.getCenter().y + wnd.getSize().y / 2)));

					if (viewCentered) wnd.setView(view);

					if (Keyboard::isKeyPressed(Keyboard::Dash)) 
						mp.setVolume(mp.getVolume() - 1);
					if (Keyboard::isKeyPressed((Keyboard::Key)55)) 
						mp.setVolume(mp.getVolume() + 1);

					deltaTime = clock.restart().asSeconds();
					if (deltaTime > 1. / 60.) deltaTime = 1. / 60.;

					while (wnd.pollEvent(ev)) {
						switch (ev.type) {
						case Event::Closed:
							wnd.close(); return;
							break;
						case Event::Resized:
							view.setSize(Vector2f(wnd.getSize()));//VIEW_HEIGHT * (float(wnd.getSize().x) / float(wnd.getSize().y)), VIEW_HEIGHT);
							break;
						case Event::KeyReleased: {
							switch (ev.key.code) {
							case Keyboard::Escape:
								mp.interrupt();
								menuState = MenuState::INGAME;
								sprites["ingame_menu_bg"]->setPosition(view.getCenter());
								ingameMenu.setPosition(Vector2f(view.getCenter().x, view.getCenter().y - 100));
								continue;
								break;
							case Keyboard::F11:
								Level::debug = !Level::debug;
								break;
							case Keyboard::X:
								levels[levelNames[currLevel]].reload();
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
							if (ev.mouseButton.button == Mouse::Button::Left) 
								mp.CheckClick(ev, wnd, view);
							
							break;
						}
						levels[levelNames[currLevel]].checkInteraction(ev, player);
					}

					player.Update(deltaTime);
					levels[levelNames[currLevel]].checkCollision(player);

					//// Отрисовка белых границ окна. Можно откомментировать когда-нибудь
					//if (levels[switched].bordered()) {
					//	for (int i = 0; i < 4; i++) {
					//		levels[switched].getObjects()[i].Draw(wnd);
					//	}
					//}

					levels[levelNames[currLevel]].Update(player);
				}

				levels[levelNames[currLevel]].Draw(wnd, &player);
				levels[levelNames[currLevel]].drawHint(wnd, player, pixelFont);

				mp.drawButtons(wnd);
				wnd.draw(songText);

				if (menuState == MenuState::INGAME) {
					wnd.draw(*sprites["ingame_menu_bg"]);
					ingameMenu.drawButtons(wnd);
					if (wnd.pollEvent(ev)) {
						if (ev.type == Event::KeyReleased)
							if (ev.key.code == (Keyboard::Escape)) {
								menuState = MenuState::NO_MENU;
								if (mp.isInterrupted()) mp.play();
								continue;
							}
						ingameMenu.CheckClick(ev, wnd, view);
					}
				}
			}
			else {
				wnd.draw(*sprites["menu_bg"]);
				mainMenu.drawButtons(wnd);
				while (wnd.pollEvent(ev)) {
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

