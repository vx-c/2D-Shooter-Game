#include "stdafx.h"
#include "Bullet.h"

#include <iostream>

#include "Config.h"

Bullet::Bullet(MySprite sprite, sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f acceleration, int damage)
	: sprite{ sprite }, position{ position }, velocity{ velocity }, acceleration{ acceleration }, damage{ damage }
{

	moveTimer.restart(); //start the movetimer

}


Bullet::~Bullet()
{

}


void Bullet::Move() {
	float timeElapsed = moveTimer.getElapsedTime().asSeconds(); //get the time passed from the move timer

	position += velocity * timeElapsed + 0.5f * acceleration * timeElapsed * timeElapsed; //add the displacement to the position

	velocity += acceleration * timeElapsed; //update the velocity

	moveTimer.restart(); //reset the move timer

}

RenderDescription Bullet::GetRenderDescription() {
	RenderDescription desc({ position.x - sprite.GetWidth() / 2, position.y - sprite.GetHeight() / 2 }, sprite.GetCurrentSprite());
	return desc;
}

bool Bullet::IsDestroyed() { //checks if the bullet needs to be destroyed
	if (position.x < 0 - sprite.GetWidth() or position.x > config::WINDOW_RESOLUTION[0] + sprite.GetWidth()) { isDestroyed = true; }
	else if (position.y < 0 - sprite.GetHeight() or position.y > config::WINDOW_RESOLUTION[1] + sprite.GetHeight()) { isDestroyed = true; }

	return isDestroyed;
}

CircleHitbox Bullet::GetHitbox() const{ //Returns a CircleHitbox that uses the dimensions of the sprite texture and the objects position to get its values
	
	return CircleHitbox(position.x, position.y, sprite.GetWidth() / 2);
} 

void Bullet::Collide() {
	isDestroyed = true;
}


int Bullet::Get_damage() const {
	return damage;
}

bool Bullet::IsFriendly() const {
	if (damage == 0) { //if the bullets damage is 0 then it is not friendly
		return false;
	}
	else { 
		return true;
	}
}

void Bullet::ResetTimers() { //restart the bullet timers so copies will work, , i know this is probably not the best way to do this
	moveTimer.restart();
}
