#pragma once

#include <SFML/Graphics.hpp>

#include "MySprite.h"
#include "BossPhase.h"
#include "RenderDescription.h"
#include "RectangleHitbox.h"
#include "Bullet.h"

#include <iostream>

class Boss
{
public:
	Boss(MySprite &sprite, std::vector<BossPhase> phases, float enterTime);
	Boss(const Boss &boss2);
	~Boss();

	void Move();

	void UpdateSprite();

	RenderDescription GetRenderDescription();

	RectangleHitbox GetHitbox();

	void Collide(Bullet bullet);

	void Shoot(std::vector<Bullet*> &bullets, sf::Vector2f playerPos);

	void Update();

	float Get_enterTime();

	bool Get_isDestroyed();


private:
	sf::Vector2f position; //The enemy's position

	bool isDestroyed;

	MySprite sprite;

	float enterTime;

	int phase; //the current phase
	std::vector<BossPhase> phases;

	std::size_t pointNum; //used for keeping track of which point we're on in the path
	sf::Clock waitTimer; //wait timer for the path

	sf::Clock moveTimer;

};

