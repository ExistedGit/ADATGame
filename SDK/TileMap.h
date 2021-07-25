#pragma once
#include "SFML/Graphics.hpp"


class TileMap : public sf::Drawable, public sf::Transformable
{
public:

    bool load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height);

    sf::Vector2f getSize() const;

    
private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    unsigned int width, height;
    sf::VertexArray m_vertices;
    sf::Texture m_tileset;

    
};
