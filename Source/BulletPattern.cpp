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
						RotateBy(rotation); //rotate the pattern
						return list;

					case patternType::simple:
						++currentSet;
						fireRateTimer.restart();
						list = GetSimpleBullets(initialPosition);
						RotateBy(rotation); //rotate the pattern
						return list;


					default:
						break;
					}
				}
				else {
					cooldownTimer.restart();
					currentSet = 1;
					UndoRotations(); //undo any rotations
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
					RotateBy(rotation); //rotate the pattern
					return list;

				case patternType::simple:
					++currentSet;
					fireRateTimer.restart();
					list = GetSimpleBullets(initialPosition);
					RotateBy(rotation); //rotate the pattern
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

	std::vector<sf::Vector2f> positions; //list of positions
	std::vector<sf::Vector2f> velocities; //list of velocities
	std::vector<sf::Vector2f> accelerations; //list of accelerations

	float playerAngle; //is added to the base angle to account for player position

	sf::Vector2f vecToPlayer = playerPosition - initialPosition;

	float dotProduct = vecToPlayer.x * 1 + vecToPlayer.y * 0; // dot product of the vector from the initial position to the player and the direction vector for 0 degrees

	if (dotProduct != 0) { //confirm dot product isn't 0

		float cosine = dotProduct / sqrt(vecToPlayer.x * vecToPlayer.x + vecToPlayer.y * vecToPlayer.y); //get the cosine of the angle between the player vector and the 0 degrees direction vector

		if (vecToPlayer.y > 0) { //if the player is below the enemy 2pi gets added to the angle 
			playerAngle = 2 * config::PI - acosf(cosine);
		}
		else {
			playerAngle = acosf(cosine);
		}

	}

	else { //if the dot product is 0

		playerAngle = 0; //baseAngle is already targeting the player
	}

	for (std::size_t i = 0; i < baseAngles.size(); ++i) { //get the positions

		float radians = baseAngles[i] * config::PI / 180; //convert the angle to radians
		radians = radians + playerAngle; //adjust the angle with the playerAngle

		float cosine = cosf(radians); //get the cosine of the angle
		float sine = sinf(radians); //get the sine of the angle

		sf::Vector2f distance = { cosine, -sine }; //create a unit vector in the direction of the angle, sine is negative because of they way positions are mapped ont he window

		distance = distance * baseDistances[i]; //multiply it by the distance multiplier

		positions.push_back(initialPosition + distance); //add the distance vector to the initial position to get the position and add it to the positions list
	}

	for (std::size_t i = 0; i < velocityAngles.size(); ++i) { //get the velocities

		float angle = velocityAngles[i] + baseAngles[i]; //angle of the bullets velocity

		float radians = angle * config::PI / 180; //convert the angle to radians
		radians = radians + playerAngle; //adjust the angle with the playerAngle

		float cosine = cosf(radians); //get the cosine of the angle
		float sine = sinf(radians); //get the sine of the angle

		sf::Vector2f velocity = { cosine, -sine }; //create a unit vector in the direction of the angle, sine is flipped due to how positions are mapped ont he window

		velocities.push_back(velocity * velocitySpeeds[i]); //multiply it by the distance multiplier and add it to the velocities list

	}

	for (std::size_t i = 0; i < accelerationAngles.size(); ++i) {

		float angle = accelerationAngles[i] + baseAngles[i]; //angle the bullet is accelerating

		float radians = baseAngles[i] * config::PI / 180; //convert the angle to radians
		radians = radians + playerAngle; //adjust the angle with the playerAngle

		float cosine = cosf(radians); //get the cosine of the angle
		float sine = sinf(radians); //get the sine of the angle

		sf::Vector2f acceleration = { cosine, -sine }; //create a unit vector in the direction of the angle, sine is flipped due to how positions are mapped ont he window

		accelerations.push_back(acceleration * accelerationSpeeds[i]); //multiply it by the distance multiplier and add it to the accelerations list
	}

	for (std::size_t i = 0; i < baseAngles.size(); ++i) { //create the bulletDescriptions in the list
		list.push_back(Bullet(sprite, positions[i], velocities[i], accelerations[i], damage));

	}

	return list;
}

std::vector<Bullet> BulletPattern::GetSimpleBullets(sf::Vector2f initialPosition) {

	std::vector<Bullet> list;

	std::vector<sf::Vector2f> positions; //list of positions
	std::vector<sf::Vector2f> velocities; //list of velocities
	std::vector<sf::Vector2f> accelerations; //list of accelerations

	for (std::size_t i = 0; i < baseAngles.size(); ++i) { //get the positions

		float radians = baseAngles[i] * config::PI / 180; //convert the angle to radians


		float cosine = cosf(radians); //get the cosine of the angle
		float sine = sinf(radians); //get the sine of the angle

		sf::Vector2f distance = { cosine, -sine }; //create a unit vector in the direction of the angle, sine is negative because of they way positions are mapped ont he window

		distance = distance * baseDistances[i]; //multiply it by the distance multiplier

		positions.push_back(initialPosition + distance); //add the distance vector to the initial position to get the position and add it to the positions list
	}

	for (std::size_t i = 0; i < velocityAngles.size(); ++i) { //get the velocities

		float angle = velocityAngles[i] + baseAngles[i]; //angle of the bullets velocity

		float radians = angle * config::PI / 180; //convert the angle to radians

		float cosine = cosf(radians); //get the cosine of the angle
		float sine = sinf(radians); //get the sine of the angle

		sf::Vector2f velocity = { cosine, -sine }; //create a unit vector in the direction of the angle, sine is flipped due to how positions are mapped ont he window

		velocities.push_back(velocity * velocitySpeeds[i]); //multiply it by the distance multiplier and add it to the velocities list

	}

	for (std::size_t i = 0; i < accelerationAngles.size(); ++i) {

		float angle = accelerationAngles[i] + baseAngles[i]; //angle the bullet is accelerating

		float radians = baseAngles[i] * config::PI / 180; //convert the angle to radians

		float cosine = cosf(radians); //get the cosine of the angle
		float sine = sinf(radians); //get the sine of the angle

		sf::Vector2f acceleration = { cosine, -sine }; //create a unit vector in the direction of the angle, sine is flipped due to how positions are mapped ont he window

		accelerations.push_back(acceleration * accelerationSpeeds[i]); //multiply it by the distance multiplier and add it to the accelerations list
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