#pragma once

#include <vector>

#include "Bullet.h"

namespace patternType {
	enum patternType {
		simple,
		targetting,
	};
}

class BulletPattern
{
public:
	BulletPattern();
	BulletPattern(MySprite sprite, int damage, float fireRate, float rotation, int maxSets, float cooldown, int type, std::vector<float> baseAngles, std::vector<float> baseDistances, 
		std::vector<float> velocityAngles, std::vector<float> velocitySpeeds, std::vector <float> accelerationAngles, std::vector<float> accelerationSpeeds);

	~BulletPattern();

	std::vector<Bullet> GetBullets(sf::Vector2f initialPosition, sf::Vector2f playerPosition);

	float GetFireRate();

	void Reset_fireRateTimer();

private:

	void RotateBy(float degrees); 
	void UndoRotations();

	std::vector<Bullet> GetSimpleBullets(sf::Vector2f initialPosition);

	std::vector<Bullet> GetTargettingBullets(sf::Vector2f initialPosition, sf::Vector2f playerPosition);

//	std::vector<BulletDescription> GetRotatingBullets(sf::Vector2f initialPosition);

	int type; //type of bullet pattern

	int damage;

	int maxSets; //the number of sets of bullets to be fired in succession
	 
	int currentSet; //counts the sets of bullets fired since the last cooldown

	float fireRate; //the time between each set of bullets

	float rotation; //degrees the pattern rotates by every set, is only used for rotating patterns

	sf::Clock fireRateTimer;
	
	float cooldown; //the cooldown time after after all of the sets have been fired off

	sf::Clock cooldownTimer;
	
	MySprite sprite;


	std::vector<float> initialBaseAngles; //the initial base angles these need to be saved so we can undo rotations

	std::vector<float> baseAngles; //angle the bullet appears at around the object firing it
	std::vector<float> baseDistances; //distance the bullet appears from the object firing it
	std::vector<float> velocityAngles; //angle the bullet is initially travelling 
	std::vector<float> velocitySpeeds; //speed the bullet is initially going
	std::vector<float> accelerationAngles; //angle the bullet is accelerating
	std::vector<float> accelerationSpeeds; //the acceleration speed of the bullet
};

