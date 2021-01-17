#pragma once

#include "EnemyDescription.h"
#include "Enemy.h"
#include "Boss.h"

class EnemyFactory
{
public:
	EnemyFactory(std::vector<Enemy> enemyQueue, std::vector<Boss> bossQueue);
	~EnemyFactory();

	void MakeEnemies(std::vector<Enemy*> &enemies);

	void MakeBosses(std::vector<Boss*> &bosses);

	void ResetTimers(); //reset the creationTimer

	int Remaining();

	void ResetQueues(std::vector<Enemy> enemyQueue, std::vector<Boss> bossQueue);

private:
	std::vector<Enemy> enemyQueue;
	std::vector<Boss> bossQueue;

	std::vector<sf::Texture> enemyTextures; //List of enemy textures for the sprites. textures[i] will be the corresponding texture for sprites[i]
	std::vector<sf::Sprite> enemySprites;

	sf::Clock creationTimer; //keeps track of the time so it knows when to create enemies
};

