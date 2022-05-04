#pragma once
#include <iostream>
#include "SmartSprite.h"
#include <SFML/Graphics.hpp>
#include "SFML/System.hpp"
using namespace std;
using namespace sf;

class StopwatchSFML
{
private:
	Time  time;		// Прошедшее время 
	Clock clock;
	Font  font;		// Стиль текста
	SmartSprite sprite;			// Иконка часов
public:
	
	static enum Status {
		OnPause, Play
	};
	Status status;
	
	Text text;	    // Текст для отрисовки
	inline StopwatchSFML(unsigned int size = 500): status(OnPause) {
		
		/*font.loadFromFile("1.ttf");*/
		text.setStyle(Text::Bold);
		text.setFont(font);
		
		text.setPosition(95, 240);
		text.setCharacterSize(size);
		text.setFillColor(Color::Red);
	}

	inline bool setFont(string name) {
		if (font.loadFromFile(name)) {
			text.setFont(font);
			return true;
		}
		else return false;
	}
	inline void draw(RenderWindow& window) {

		string newText;

		if ((time.asMilliseconds() / 1000 / 60) < 10) newText += "0";
		newText += to_string(time.asMilliseconds() / 1000 / 60) + ":";

		if ((time.asMilliseconds() / 1000) >= 60) {
			if ((time.asMilliseconds() / 1000) % 60 < 10) newText += "0";
			newText += to_string((time.asMilliseconds() / 1000) % 60) + ":";
		}
		else {
			if ((time.asMilliseconds() / 1000) < 10) newText += "0";
			newText += to_string(time.asMilliseconds() / 1000) + ":";
		}
		if ((time.asMilliseconds() % 100) < 10) newText += "0";
		newText += to_string(time.asMilliseconds() % 100);

		text.setString(newText);
		window.draw(text);
	}

	// Возвращяет прошедшее время
	inline const Time& getPastTime() const {
		return time;
	}

	inline void Nullify() {
		time = milliseconds(0);
	}
	
	// Возвращает успешно ли произошел старт
	inline bool start() {
		if (status == Play) return false;
		else {
			status = Play;
			return true;
		}
	}
	
	// Возвращает успешно ли произошела остановка
	inline bool stop() {
		if (status == OnPause) return false;
		else {
			status = OnPause;
			return true;
		}
	}

	inline bool setSprite(const string& TexturePath) {
		return sprite.loadTexture(TexturePath);
	}
	
	// Возвращает успешно ли произошело обновление
	inline bool update() {
		if(status == Play) {
			time += clock.getElapsedTime();
			clock.restart();
			return true;
		}
		else return false;
	}
};

class TimerSFML
{
private:
	Time desiredTime;		// Общее время которое должно пройти
	
	Time delta;				// Дельта между текущим временем и необходимым временем 
	Clock clock;			// Сам объэкт кторый считает врямя 
	Font font;				// Шрифт текста
public:
	static enum Status {
		Сompleted, OnPause, Play
	};
	Status status;				// Текущий статус таймера
	Text text;				// Текст для вывода на экран
	SmartSprite sprite;			// Иконка часов
	
	inline TimerSFML(Time time, unsigned int size) :
		desiredTime(time),
		delta(time),
		status(OnPause) {
		
		font.loadFromFile("Fonts/clock.ttf");
		text.setStyle(Text::Bold);
		text.setFont(font);

		text.setPosition(90, 240);
		text.setCharacterSize(size);
		text.setFillColor(Color::Red);

		delta = desiredTime;
	}


	inline bool setSprite(string TexturePath) {
		return sprite.loadTexture(TexturePath);
	}
	inline bool setFont(string path) {
		if (font.loadFromFile(path)) {
			text.setFont(font);
			return true;
		}
		else return false;
	}
	inline void draw(RenderWindow& window) {

		string newText;


		if((delta.asMilliseconds() > 0)) {
			if ((delta.asMilliseconds() / 1000 / 60) < 10) newText += "0";
			newText += to_string(delta.asMilliseconds() / 1000 / 60) + ":";

			if ((delta.asMilliseconds() / 1000) >= 60) {
				if ((delta.asMilliseconds() / 1000) % 60 < 10) newText += "0";
				newText += to_string((delta.asMilliseconds() / 1000) % 60) + ":";
			}
			else {
				if ((delta.asMilliseconds() / 1000) < 10) newText += "0";
				newText += to_string(delta.asMilliseconds() / 1000) + ":";
			}
			if ((delta.asMilliseconds() % 100) < 10) newText += "0";
			newText += to_string(delta.asMilliseconds() % 100);

			
		}
		else {
			newText = "00:00:00";
		}
		text.setString(newText);
		
		window.draw(sprite);
		window.draw(text);
	}

	// Получить оставшееся время
	inline const Time& getRemainingTime() const {
		return delta;
	}

	// Запустить таймер
	inline bool start() {
		if (status == Play) return false;
		else {
			status = Play;
			return true;
		}
	}

	inline bool restart() {
		delta = desiredTime;
		start();
	}
	
	
	// Приостановить таймер
	inline bool stop() {
		if (status == OnPause) return false;
		else {
			status = OnPause;
			return true;
		}
	}  
	
	inline Status update() {

		if (delta <= milliseconds(0) && status == Play) {
			return status = OnPause;
		}
		else {
			delta -= clock.getElapsedTime();
			clock.restart();
			return status = Play;
		}
	}


	
};
