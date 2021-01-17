#pragma once

#include <SFML\Graphics.hpp>

class RectangleHitbox
{
public:
	RectangleHitbox(float left, float top, float width, float height);
	RectangleHitbox(sf::FloatRect);

	bool CheckIntersects(RectangleHitbox &&rect);
	bool CheckIntersects(float circlex, float circley, float radius);

	float left, top, width, height;
};

