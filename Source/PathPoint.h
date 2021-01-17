#pragma once

#include <SFML/Graphics.hpp>

class PathPoint
{
public:
	PathPoint(float x, float y, float waitTime);
	PathPoint(sf::Vector2f position, float waitTime);
	~PathPoint();

	sf::Vector2f position;
	float waitTime;
};

