#include "stdafx.h"
#include "EnemyPattern.h"


EnemyPattern::EnemyPattern(float spawnRate, int spawnNumber, Enemy enemy)
	: spawnRate{ spawnRate }, spawnNumber{ spawnNumber }, enemy{ enemy }
{

}


EnemyPattern::~EnemyPattern()
{
}


std::vector<Enemy> EnemyPattern::GetEnemies() {
	std::vector<Enemy> list;

	for (int i = 0; i < spawnNumber; ++i) {
		enemy.Set_enterTime(enemy.Get_enterTime() + spawnRate); //add the spawnRate to the enemies entertime
		list.push_back(enemy);	//add the enemy to the list
	}

	

	return list;
}
