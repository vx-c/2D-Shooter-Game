#include "stdafx.h"
#include "Enemy.h"

#include "Config.h"

#include <iostream>

Enemy::Enemy(MySprite sprite, int hp, float moveSpeed, std::vector<PathPoint> path, float enterTime, float exitTime, BulletPattern bulletPattern)
	: sprite{ sprite }, hp{ hp }, moveSpeed{ moveSpeed }, path{ path }, enterTime{ enterTime }, exitTime{ exitTime }, bulletPattern{ bulletPattern }
{
	//the initial position is the first position on the path
	position = path[0].position;
	pointNum = 1;

	moveTimer.restart();
	exitTimer.restart();
	waitTimer.restart();

}

Enemy::Enemy() {
	//the initial position is the first position on the path
	position = path[0].position;
	pointNum = 1;

	moveTimer.restart();
	exitTimer.restart();
	waitTimer.restart();
}

Enemy::~Enemy()
{

}

void Enemy::Update() {
	Move();

	UpdateSprite();
}

void Enemy::Move() { 

	if (pointNum < path.size()) { //do nothing if the enemy is already at the final point

		if (waitTimer.getElapsedTime().asSeconds() > path[pointNum - 1].waitTime) { //if the enemy has finished waiting at the last point

			if (position == path[pointNum].position) { //make sure we're not dividing by zero
				pointNum++;
				waitTimer.restart(); //restart the timer
			}
			else {
				sf::Vector2f v = path[pointNum].position - position; //a vector in the correct direction
				float len = sqrt(v.x * v.x + v.y * v.y); //the length of the vector
				v = v / len; //the unit vector in the correct direction
				v = v * moveSpeed * moveTimer.getElapsedTime().asSeconds(); //the final vector

				if (sqrt(v.x * v.x + v.y * v.y) >= len) { //if the length of the final vector is greater than or equal to the length of the distance from the position to the next point 
					position = path[pointNum].position;
					pointNum++; //increment the point number
					waitTimer.restart(); //restart the timer
				}

				else {
					position += v; //add the vector to the current position
				}
			}
		}
	}

	moveTimer.restart();
}

void Enemy::UpdateSprite() {
	sprite.Next();
}

RenderDescription Enemy::GetRenderDescription() {
	RenderDescription desc(position, sprite.GetCurrentSprite());

	return desc;
}

RectangleHitbox Enemy::GetHitbox() const {

	//cuts some of the rectangle height so that the bullets actually appear to touch the enemy
	RectangleHitbox rect(position.x, position.y, sprite.GetWidth(), sprite.GetHeight() * 0.7); 
	return rect;
}

void Enemy::Collide(Bullet bullet) {
	hp = hp - bullet.Get_damage();
}

bool Enemy::IsDestroyed() const {
	
	if (hp < 1) { //the enemy has had its hp reduced to 0
		return true; 	
	} 

	else if (exitTimer.getElapsedTime().asSeconds() > exitTime) { //the enemies lifespan is over
		return true;
	}

	else if (!(pointNum < path.size()) and (position.x < 0 - sprite.GetWidth() or position.y < 0 - sprite.GetHeight()
		or position.x > config::WINDOW_RESOLUTION[0] + sprite.GetWidth() or position.y > config::WINDOW_RESOLUTION[1] + sprite.GetHeight())) {
		return true; //the enemy has reached its final point and is outside the window	
	}

	else { return false; }
}

void Enemy::Shoot(std::vector<Bullet*> &bullets, sf::Vector2f playerPos) { //The enemy creates and adds bullets to the list

	if (position.x < 0 - sprite.GetWidth() or position.y < 0 - sprite.GetHeight()
		or position.x > config::WINDOW_RESOLUTION[0] + sprite.GetWidth() or position.y > config::WINDOW_RESOLUTION[1] + sprite.GetHeight()) { //the enemy can't shoot when its outside the window
		
		bulletPattern.Reset_fireRateTimer(); //reset the timer for the patterns fire rate when the enemy is offscreen
		return;
	}

	sf::Vector2f initialPosition = { position.x + (sprite.GetWidth() / 2), position.y + (sprite.GetHeight() / 2) }; //gets the position in the center of the enemy


	std::vector<Bullet> list = bulletPattern.GetBullets(initialPosition, playerPos);


	for (std::size_t i = 0; i < list.size(); ++i) { //create the bullets from the descriptions in the list

		list[i].ResetTimers();

		bullets.push_back(new Bullet(list[i]));
	}
}

float Enemy::Get_enterTime() const {
	return enterTime;
}

void Enemy::ResetTimers() { //restart the enemy timers so copies will work, i know this is probably not the best way to do this but copy constructors are tricky
	waitTimer.restart();
	moveTimer.restart();
	exitTimer.restart();
}

void Enemy::Set_enterTime(float time) {
	enterTime = time;
}