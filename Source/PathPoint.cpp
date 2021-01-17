#include "stdafx.h"
#include "PathPoint.h"



PathPoint::PathPoint(float x, float y, float waitTime) : waitTime{ waitTime }
{
	position = { x,y };
}

PathPoint::PathPoint(sf::Vector2f position, float waitTime) : position{ position }, waitTime{ waitTime }
{
}


PathPoint::~PathPoint()
{
}
