#pragma once

#include "BulletDescription.h"
#include "CircleHitbox.h"
#include "RenderDescription.h"

class Bullet
{
public:
	Bullet(MySprite sprite, sf::Vector2f new_position, sf::Vector2f velocity, sf::Vector2f acceleration, int damage);

	~Bullet();

	void Move();

	RenderDescription GetRenderDescription();

	bool IsDestroyed();

	CircleHitbox GetHitbox() const;

	void Collide();

	int Get_damage() const;

	bool IsFriendly() const;

	void ResetTimers(); //reset the bullet timers

private:
	sf::Vector2f position;
	MySprite sprite;

	sf::Vector2f velocity;
	sf::Vector2f acceleration;
	sf::Clock moveTimer;
	bool isDestroyed = false;
	int damage;
};

