

#include "stdafx.h"
#include "EnemyFactory.h"

#include <assert.h>

#include <iostream>

EnemyFactory::EnemyFactory(std::vector<Enemy> enemyQueue, std::vector<Boss> bossQueue ) : enemyQueue{ enemyQueue }, bossQueue{ bossQueue }
{
	creationTimer.restart();

}

EnemyFactory::~EnemyFactory()
{
}

void EnemyFactory::MakeEnemies(std::vector<Enemy*> &enemies) {

	if (enemyQueue.size() != 0) {
		
		std::vector<Enemy>::iterator it = enemyQueue.begin(); //iterator

		if (enemyQueue[0].Get_enterTime() < creationTimer.getElapsedTime().asSeconds()) { //add each enemy that is ready to the enemies vector and delete them from the list
			
			enemyQueue[0].ResetTimers(); //reset the enemies timers so the new enemy will function properly
			enemies.push_back(new Enemy(enemyQueue[0]));			

			it = enemyQueue.erase(it);
		}
	}
}

void EnemyFactory::MakeBosses(std::vector<Boss*> &bosses) {
	
	if (bossQueue.size() != 0) {

		std::vector<Boss>::iterator it = bossQueue.begin(); //iterator

		if (bossQueue[0].Get_enterTime() < creationTimer.getElapsedTime().asSeconds()) { //add each enemy that is ready to the enemies vector and delete them from the list

			bosses.push_back(new Boss(bossQueue[0]));

			it = bossQueue.erase(it);
		}
	}
}

void EnemyFactory::ResetTimers() {
	creationTimer.restart();
}


int EnemyFactory::Remaining() {
	return enemyQueue.size() + bossQueue.size();
}

void EnemyFactory::ResetQueues(std::vector<Enemy> enemies, std::vector<Boss> bosses) {
	enemyQueue = enemies;
	bossQueue = bosses;
}