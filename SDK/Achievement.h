#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <map>
#include "FragShader.h"

using namespace std;



class Achievement
{
private:
	sf::Sprite sprite;
	std::string name;				// Имя достижения
	std::string condition;			// Условие выполнения 
	string id;
	int maxProgress;		    // Требуемый прогресс для выполнения ачивки
	int currProgress;				 // Текущий прогресс

	bool done;

	static const FragShader greyscale;
public:

	static map<string, Achievement*> ach_id;
	
	Achievement(sf::Texture* text, const std::string& name, const std::string& condition, const string& id, int maxProgress, int currProgress = 0) :
		name(name),
		condition(condition),
		maxProgress(maxProgress),
		currProgress(currProgress),
		id(id),
		sprite(*text)
	{
	}
	Achievement& operator++(int l) {
		if (!done)
			currProgress++;
		done = currProgress >= maxProgress;
		return *this;
	}
	Achievement& operator++() {
		if (!done)
			currProgress++;
		done = currProgress >= maxProgress;
		return *this;
	}
	
	void nullify() {
		if (!done) 
			currProgress = 0;		
	}
	void draw(sf::RenderWindow& window) const {
		window.draw(sprite, done ? sf::RenderStates::Default : &greyscale);
	}
	void print() const {
		std::cout << "Name: " << name << std::endl;
		std::cout << "Condition: " << condition << std::endl;
		std::cout << "Progress: " << currProgress << "/" << maxProgress << std::endl;
	}
	bool isDone() const {
		return done;
	}

	
	const std::string& getName() const {
		return name;
	}
	const std::string& getCondition() const {
		return condition;
	}
	int getCurrentProgress() const {
		return currProgress;
	}
	int getNecessaryProgress() const {
		return maxProgress;
	}
	const string& getID()const {
		return id;
	}
	

	void setTexture(sf::Texture* texture) {
		sprite.setTexture(*texture);
	}
	void setActive(bool sw) {
		done = sw;
	}

};

