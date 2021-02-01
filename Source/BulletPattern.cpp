#include "stdafx.h"
#include "BulletPattern.h"

#include <math.h>

#include "assert.h"

#include <iostream>

#include "Config.h"

BulletPattern::BulletPattern() {
	cooldownTimer.restart();
	currentSet = 1;
}

BulletPattern::BulletPattern(MySprite sprite, int damage, float fireRate, float rotation, int maxSets,float cooldown, int type,
	std::vector<float> baseAngles, std::vector<float> baseDistances, std::vector<float> velocityAngles,
	std::vector<float> velocitySpeeds, std::vector <float> accelerationAngles, std::vector<float> accelerationSpeeds)

	: sprite{ sprite }, damage{ damage }, fireRate{ fireRate }, rotation{ rotation }, maxSets{ maxSets }, cooldown{ cooldown }, type{ type }, baseAngles{ baseAngles }, baseDistances{ baseDistances }, velocityAngles{ velocityAngles },
	velocitySpeeds{ velocitySpeeds }, accelerationAngles{ accelerationAngles }, accelerationSpeeds{ accelerationSpeeds }

{

	assert(baseAngles.size() == baseDistances.size() and baseDistances.size() == velocityAngles.size() //all vectors are the same size
		and velocityAngles.size() == velocitySpeeds.size() and velocitySpeeds.size() == accelerationAngles.size() and accelerationAngles.size() == accelerationSpeeds.size());

	cooldownTimer.restart();
	fireRateTimer.restart();

	initialBaseAngles = baseAngles;

	currentSet = 1;
}

BulletPattern::~BulletPattern()
{

}

std::vector<Bullet> BulletPattern::GetBullets(sf::Vector2f initialPosition, sf::Vector2f playerPosition) { //Get a list of bullet descriptions for the pattern

	std::vector<Bullet> list;

	if (cooldown > 0) {
		if (cooldownTimer.getElapsedTime().asSeconds() > cooldown) {

			if (fireRateTimer.getElapsedTime().asSeconds() > fireRate) {

				if (currentSet <= maxSets) {
					switch (type)
					{
					case patternType::targetting:
						++currentSet;
						fireRateTimer.restart();
						list = GetTargettingBullets(initialPosition, playerPosition);
						RotateBy(rotation);
						return list;

					case patternType::simple:
						++currentSet;
						fireRateTimer.restart();
						list = GetSimpleBullets(initialPosition);
						RotateBy(rotation);
						return list;


					default:
						break;
					}
				}
				else {
					cooldownTimer.restart();
					currentSet = 1;
					UndoRotations();
				}
			}
		}
	}

	else { // if the cooldown is 0
		if (fireRateTimer.getElapsedTime().asSeconds() > fireRate) {

			if (currentSet <= maxSets) {
				switch (type)
				{
				case patternType::targetting:
					++currentSet;
					fireRateTimer.restart();
					list = GetTargettingBullets(initialPosition, playerPosition);
					RotateBy(rotation);
					return list;

				case patternType::simple:
					++currentSet;
					fireRateTimer.restart();
					list = GetSimpleBullets(initialPosition);
					RotateBy(rotation);
					return list;


				default:
					list = {};
					return list;
				}
			}
			else {
				currentSet = 1;
				UndoRotations();
			}
		}
	}

	
}

float BulletPattern::GetFireRate() { //get the patterns fire rate
	return fireRate;
}

std::vector<Bullet> BulletPattern::GetTargettingBullets(sf::Vector2f initialPosition, sf::Vector2f playerPosition) {
	
	std::vector<Bullet> list;

	std::vector<sf::Vector2f> positions;
	std::vector<sf::Vector2f> velocities;
	std::vector<sf::Vector2f> accelerations;

	float playerAngle; //is added to the base angle to account for player position

	sf::Vector2f vecToPlayer = playerPosition - initialPosition;

	float dotProduct = vecToPlayer.x * 1 + vecToPlayer.y * 0;

	if (dotProduct != 0) {

		float cosine = dotProduct / sqrt(vecToPlayer.x * vecToPlayer.x + vecToPlayer.y * vecToPlayer.y); //get the cosine of the angle between the player vector and the 0 degrees direction vector

		if (vecToPlayer.y > 0) { //if the player is below the enemy 2pi gets added to the angle 
			playerAngle = 2 * config::PI - acosf(cosine);
		}
		else {
			playerAngle = acosf(cosine);
		}

	}

	else {

		playerAngle = 0; //baseAngle is already targeting the player
	}

	for (std::size_t i = 0; i < baseAngles.size(); ++i) { //get the positions

		float radians = baseAngles[i] * config::PI / 180;
		radians = radians + playerAngle;

		float cosine = cosf(radians);
		float sine = sinf(radians);

		sf::Vector2f distance = { cosine, -sine }; //create a unit vector in the direction of the angle, sine is negative because of they way positions are mapped on the window

		distance = distance * baseDistances[i];

		positions.push_back(initialPosition + distance);
	}

	for (std::size_t i = 0; i < velocityAngles.size(); ++i) { //get the velocities

		float angle = velocityAngles[i] + baseAngles[i];

		float radians = angle * config::PI / 180; 
		radians = radians + playerAngle; 

		float cosine = cosf(radians); 
		float sine = sinf(radians); 

		sf::Vector2f velocity = { cosine, -sine }; 

		velocities.push_back(velocity * velocitySpeeds[i]); 

	}

	for (std::size_t i = 0; i < accelerationAngles.size(); ++i) {

		float angle = accelerationAngles[i] + baseAngles[i]; 

		float radians = baseAngles[i] * config::PI / 180;
		radians = radians + playerAngle; 

		float cosine = cosf(radians); 
		float sine = sinf(radians); 

		sf::Vector2f acceleration = { cosine, -sine };

		accelerations.push_back(acceleration * accelerationSpeeds[i]); 
	}

	for (std::size_t i = 0; i < baseAngles.size(); ++i) {
		list.push_back(Bullet(sprite, positions[i], velocities[i], accelerations[i], damage));
	}

	return list;
}

std::vector<Bullet> BulletPattern::GetSimpleBullets(sf::Vector2f initialPosition) {

	std::vector<Bullet> list;

	std::vector<sf::Vector2f> positions;
	std::vector<sf::Vector2f> velocities;
	std::vector<sf::Vector2f> accelerations;

	for (std::size_t i = 0; i < baseAngles.size(); ++i) {

		float radians = baseAngles[i] * config::PI / 180;

		float cosine = cosf(radians);
		float sine = sinf(radians);

		sf::Vector2f distance = { cosine, -sine };

		distance = distance * baseDistances[i];

		positions.push_back(initialPosition + distance);
	}

	for (std::size_t i = 0; i < velocityAngles.size(); ++i) {

		float angle = velocityAngles[i] + baseAngles[i];

		float radians = angle * config::PI / 180; 

		float cosine = cosf(radians); 
		float sine = sinf(radians); 

		sf::Vector2f velocity = { cosine, -sine }; 

		velocities.push_back(velocity * velocitySpeeds[i]); 

	}

	for (std::size_t i = 0; i < accelerationAngles.size(); ++i) {

		float angle = accelerationAngles[i] + baseAngles[i]; 

		float radians = baseAngles[i] * config::PI / 180; 

		float cosine = cosf(radians); 
		float sine = sinf(radians); 

		sf::Vector2f acceleration = { cosine, -sine }; 

		accelerations.push_back(acceleration * accelerationSpeeds[i]); 
	}

	for (std::size_t i = 0; i < baseAngles.size(); ++i) { //create the bulletDescriptions in the list
		list.push_back(Bullet(sprite, positions[i], velocities[i], accelerations[i], damage));

	}

	return list;
}

void BulletPattern::Reset_fireRateTimer() {
	fireRateTimer.restart();
}

void BulletPattern::RotateBy(float degrees) { //rotate the pattern
	for (std::size_t i = 0; i < baseAngles.size(); ++i) {
		baseAngles[i] += degrees;
	}
}

void BulletPattern::UndoRotations() { //undo any rotations done to the pattern
	for (std::size_t i = 0; i < baseAngles.size(); ++i) {
		baseAngles[i] = initialBaseAngles[i];
	}
}
