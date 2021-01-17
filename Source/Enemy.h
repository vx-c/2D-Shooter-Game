#pragma once

#include "RenderDescription.h"

#include "RectangleHitbox.h"
#include "BulletDescription.h"
#include "Bullet.h"
#include "BulletPattern.h"
#include "MySprite.h"

#include "PathPoint.h"

class Enemy
{
public:
	Enemy(MySprite sprite, int hp, float moveSpeed, std::vector<PathPoint> path, float enterTime, float exitTime, BulletPattern bulletPattern);

	Enemy();

	~Enemy();

	void Update();

	void Move();

	void UpdateSprite();

	RenderDescription GetRenderDescription();

	RectangleHitbox GetHitbox() const;

	void Collide(Bullet bullet);

	bool IsDestroyed() const;

	void Shoot(std::vector<Bullet*> &bullets, sf::Vector2f playerPos);

	float Get_enterTime() const;

	void Set_enterTime(float time);

	void ResetTimers();

private:
	

	sf::Vector2f position; //The enemy's position
	
	MySprite sprite;

	float moveSpeed;
	BulletPattern bulletPattern;

	std::vector<PathPoint> path; //the path the enemy moves along
	std::size_t pointNum; //used for keeping track of which point we're on in the path
	sf::Clock waitTimer; //wait timer for the path

	sf::Clock moveTimer;

	sf::Clock exitTimer;
	float enterTime;
	float exitTime;

	int hp; //enemies life total
};

