#pragma once
#include <SFML/Graphics.hpp> 
#include <string>

using namespace sf;

class SmartSprite : public sf::Sprite
{
public:
	// Конструктор по умолчанию
	inline SmartSprite() {};

	// Загружает текстуру по указанному пути в спрайт
	inline SmartSprite(const std::string& texturePath) {
		Texture* texture = new Texture();
		if (!texture->loadFromFile(texturePath))
			throw std::runtime_error(u8"SmartSprite(const string&): файл не найден");
		setTexture(*texture);
		setOrigin(Vector2f(getTexture()->getSize() / 2u));
	}

	inline bool loadTexture(const std::string& texturePath) {
		Texture* texture = new Texture();
		if (!texture->loadFromFile(texturePath))
			return false;
		setTexture(*texture);
		setOrigin(Vector2f(getTexture()->getSize() / 2u));
		return true;
	}

};

