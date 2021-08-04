#pragma once
#include <SFML/Graphics.hpp>
#include <string>
class FragShader : public sf::Shader
{
public:
	inline FragShader() : Shader() {};
	inline FragShader(const std::string& code) {
		loadFromMemory(code, Shader::Type::Fragment);
		setUniform("texture", sf::Shader::CurrentTexture);
	}
};

