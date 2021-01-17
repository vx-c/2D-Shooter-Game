#pragma once

#include <SFML\Graphics.hpp>

class RenderDescription
{
public:
	RenderDescription(sf::Vector2f position, sf::Sprite &sprite);
	~RenderDescription();

	sf::Vector2f position;
	sf::Sprite *sprite;
};

