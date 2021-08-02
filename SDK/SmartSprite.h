#pragma once
#include <SFML/Graphics.hpp> 
#include <string>

using namespace sf;

class SmartSprite : public sf::Sprite
{
public:
	// ����������� �� ���������
	inline SmartSprite() {};

	// ��������� �������� �� ���������� ���� � ������
	inline SmartSprite(const std::string& texturePath) {
		Texture* texture = new Texture();
		if (!texture->loadFromFile(texturePath))
			throw std::runtime_error(u8"SmartSprite(const string&): ���� �� ������");
		setTexture(*texture);
	}

};

