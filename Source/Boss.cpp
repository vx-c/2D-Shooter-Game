#include "stdafx.h"
#include "Boss.h"


Boss::Boss(MySprite &sprite, std::vector<BossPhase> phases, float enterTime) 
	: sprite{ sprite }, phases{ phases }, enterTime{ enterTime }
{

	phase = 0; //start at phase 0
	pointNum = 0;
	position = phases[phase].path[0].position; //initial position

	isDestroyed = false;

	//start timers
	moveTimer.restart();
	waitTimer.restart();
}

Boss::Boss(const Boss &boss2 )
	: sprite{ boss2.sprite }, phases{ boss2.phases }, enterTime{ boss2.enterTime }
{

	phase = 0; //start at phase 0
	pointNum = 0;
	position = phases[phase].path[0].position; //initial position

	isDestroyed = false;

	//start timers
	moveTimer.restart();
	waitTimer.restart();
}


Boss::~Boss()
{
}

void Boss::Move() {

	if (pointNum < phases[phase].path.size()) { //do nothing if the enemy is already at the final point

		if (waitTimer.getElapsedTime().asSeconds() > phases[phase].path[pointNum].waitTime) { //if the enemy has finished waiting at the last point

			if (position == phases[phase].path[pointNum].position) { //make sure we're not dividing by zero
				pointNum++;
				waitTimer.restart(); //restart the timer
			}
			else {
				sf::Vector2f v = phases[phase].path[pointNum].position - position; //a vector in the correct direction
				float len = sqrt(v.x * v.x + v.y * v.y); //the length of the vector
				v = v / len; //the unit vector in the correct direction
				v = v * phases[phase].moveSpeed * moveTimer.getElapsedTime().asSeconds(); //the final vector

				if (sqrt(v.x * v.x + v.y * v.y) >= len) { //if the length of the final vector is greater than or equal to the length of the distance from the position to the next point 
					position = phases[phase].path[pointNum].position;
					pointNum++; //increment the point number
					waitTimer.restart(); //restart the timer
				}

				else {
					position += v; //add the vector to the current position
				}
			}
		}
	} 
	else {
		pointNum = 0;

		//reverse the path
		std::vector<PathPoint> list;
		for (int i = phases[phase].path.size() - 1; i >= 0; --i) {
			
			list.push_back(phases[phase].path[i]);
		}
		phases[phase].path = list;

	}

	moveTimer.restart();

}

void Boss::UpdateSprite() {
	sprite.Next();
}

RenderDescription Boss::GetRenderDescription() {
	RenderDescription desc(position, sprite.GetCurrentSprite());
	return desc;
}

RectangleHitbox Boss::GetHitbox() {
	//cuts some of the rectangle height so it looks better
	RectangleHitbox rect(position.x, position.y, sprite.GetWidth(), sprite.GetHeight() * 0.7);
	return rect;
}

void Boss::Collide(Bullet bullet) {
	phases[phase].hp -= bullet.Get_damage();
}

void Boss::Update() {
	Move();

	UpdateSprite();

	if (phases[phase].hp <= 0) {

		if (phase < phases.size() - 1) {
			++phase;
			moveTimer.restart();
			waitTimer.restart();
			pointNum = 0;
		}
		else {
			isDestroyed = true;
		}
	}
}

void Boss::Shoot(std::vector<Bullet*> &bullets, sf::Vector2f playerPos) { //The enemy creates and adds bullets to the list


	sf::Vector2f initialPosition = { position.x + (sprite.GetWidth() / 2), position.y + (sprite.GetHeight() / 2) }; //gets the position in the center of the boss


	for (std::size_t i = 0; i < phases[phase].bulletPatterns.size(); ++i) {

		std::vector<Bullet> list = phases[phase].bulletPatterns[i].GetBullets(initialPosition, playerPos);

		for (std::size_t ii = 0; ii < list.size(); ++ii) { //create the bullets from the descriptions in the list

			list[ii].ResetTimers();

			bullets.push_back(new Bullet(list[ii]));
		}

	}

}

bool Boss::Get_isDestroyed() {
	return isDestroyed;
}

float Boss::Get_enterTime() {
	return enterTime;
}