#pragma once

#include "Player.h"
#include "Renderer.h"
#include "Bullet.h"
#include "Enemy.h"
#include "EnemyDescription.h"
#include "Boss.h"

class CollisionManager
{
public:
	CollisionManager();


	void CheckCollision(Bullet &bullet, Player &player);
	void CheckCollision(Bullet &bullet, Enemy &enemy);
	void CheckCollision(Bullet &bullet, Boss &boss);
	void CheckCollision(Enemy &enemy, Player &player);
	void CheckCollision(Boss &boss, Player &player);
	

private:

};

