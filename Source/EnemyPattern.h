#pragma once

#include "Enemy.h"
#include <iostream>

class EnemyPattern
{
public:
	EnemyPattern(float spawnRate, int spawnNumber, Enemy enemy);

	~EnemyPattern();

	std::vector<Enemy> GetEnemies();

private:
	Enemy enemy; //the enemy being created in the pattern
	float spawnRate; //rate at which enemies spawn in seconds
	int spawnNumber; //number of enemies in the pattern

};

