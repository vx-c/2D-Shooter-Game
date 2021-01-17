#include "stdafx.h"
#include "RectangleHitbox.h"


RectangleHitbox::RectangleHitbox(float left, float top, float width, float height)
	: left{ left }, top{ top }, width { width }, height{ height }
{
	
}

RectangleHitbox::RectangleHitbox(sf::FloatRect rect)
	: left{ rect.left }, top{ rect.top }, width{ rect.width }, height{ rect.height }
{

}

bool RectangleHitbox::CheckIntersects(RectangleHitbox &&rect) {
	if (rect.top + rect.height < top) { return false; } //rect is above the hitbox
	else if (rect.top > top + height) { return false; } //rect is below the hitbox
	else if (rect.left + width < left) { return false; } //rect is to the left of the hitbox
	else if (rect.left > left + width) { return false; } //rect is to the right of the hitbox
	else { return true; } //if we get here then they are intersecting
}


bool RectangleHitbox::CheckIntersects(float circlex, float circley, float radius) {
	if (circley + radius < top) { return false; } //circle is above the hitbox
	else if (circley - radius > top + height) { return false; } //circle is below the hitbox
	else if (circlex + radius < left) { return false; } //circle is to the left of the hitbox
	else if (circlex - radius > left + width) { return false; } //circle is to the right of the hitbox
	else { return true; }
}