#include "stdafx.h"
#include "CircleHitbox.h"



CircleHitbox::CircleHitbox(float x, float y, float radius)
	:	x{ x }, y{ y }, radius{ radius }
{

}

bool CircleHitbox::CheckIntersects(CircleHitbox &&circle) const {
	float len = sqrt((circle.x - x)*(circle.x - x) + (circle.y - y)*(circle.y - y)); //calculate the length of the distance between the circles centers
	if (len < radius + circle.radius) { return true; } //if the circles intersect
	else { return false; } //if the circles don't intersect
}

bool CircleHitbox::CheckIntersects(float left, float top, float width, float height) const {
	if (y + radius < top) { return false; } //circle is above the hitbox
	else if (y - radius > top + height) { return false; } //circle is below the hitbox
	else if (x + radius < left) { return false; } //circle is to the left of the hitbox
	else if (x - radius > left + width) { return false; } //circle is to the right of the hitbox
	else { return true; }
}