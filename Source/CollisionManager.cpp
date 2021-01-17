#include "stdafx.h"
#include "CollisionManager.h"

#include <iostream>

CollisionManager::CollisionManager()
{
}

void CollisionManager::CheckCollision(Bullet &bullet, Player &player) {
	
	if (bullet.IsFriendly()) { //no collision with friendly bullets
		return; //there is no collision
	}

	else if (bullet.GetHitbox().CheckIntersects(player.GetHitbox())) {
		bullet.Collide();
		player.Collide();
	}
}

void CollisionManager::CheckCollision(Bullet &bullet, Enemy &enemy) {

	if (not (bullet.IsFriendly())) { //no collision with non friendly bullets
		return; //there is no collision
	}
	
	else if (bullet.GetHitbox().CheckIntersects(enemy.GetHitbox().left, enemy.GetHitbox().top, enemy.GetHitbox().width, enemy.GetHitbox().height)) {
		bullet.Collide();
		enemy.Collide(bullet);
		return; //collision occured
	}
}

void CollisionManager::CheckCollision(Enemy &enemy, Player &player) {
	if (enemy.GetHitbox().CheckIntersects(player.GetHitbox().x, player.GetHitbox().y, player.GetHitbox().radius)) {
		player.Collide();
	}
}

void CollisionManager::CheckCollision(Boss &boss, Player &player) {
	if (boss.GetHitbox().CheckIntersects(player.GetHitbox().x, player.GetHitbox().y, player.GetHitbox().radius)) {
		player.Collide();
	}
}

void CollisionManager::CheckCollision(Bullet &bullet, Boss &boss) {
	if (not (bullet.IsFriendly())) { //no collision with non friendly bullets
		return; //there is no collision
	}

	else if (bullet.GetHitbox().CheckIntersects(boss.GetHitbox().left, boss.GetHitbox().top, boss.GetHitbox().width, boss.GetHitbox().height)) {
		bullet.Collide();
		boss.Collide(bullet);
		return; //collision occured
	}
}