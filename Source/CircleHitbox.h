#pragma once

#include <SFML\Graphics.hpp>

class CircleHitbox
{
public:
	CircleHitbox(float x, float y, float radius);

	bool CheckIntersects(CircleHitbox &&circle) const;
	bool CheckIntersects(float left, float top, float width, float height) const;
	
	float x, y, radius;
};

