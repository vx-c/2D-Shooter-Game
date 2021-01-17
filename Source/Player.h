#pragma once

#include "Bullet.h"

#include "CircleHitbox.h"

#include "BulletPattern.h"

#include "MySprite.h"

#include "Config.h"

#include "PlayerDescription.h"


class Player
{
public:
	Player(MySprite &mySprite, sf::Sprite &new_hitbox, sf::Vector2f new_position, float unfocusedSpeed, float focusedSpeed, BulletPattern bulletPattern);
	~Player();

	void Update();

	void Move();

	void UpdateSprite();

	void Shoot(std::vector<Bullet*> &bullets);

	void Bomb(std::vector<Bullet*> &bullets); 

	PlayerDescription GetDescription() const;

	RenderDescription GetRenderDescription() const;

	RenderDescription GetHitboxRenderDescription() const;

	CircleHitbox GetHitbox() const; //this needs to be changed to a circle hitbox at some point

	void Collide();

	void Reset(); //reset the player to its initial state

	bool CheckGameOver();

private:

	sf::Vector2f position; //The entity's position

	int lives; //life counter

	int bombs; //bomb counter

	sf::Sprite *hitbox;

	sf::Clock moveTimer;

	MySprite *mySprite;

	float unfocusedSpeed;
	float focusedSpeed;

	float iTime; //how much iTime the player has remaining
	sf::Clock iTimer; //timer for invulnerability

	BulletPattern bulletPattern; //Pattern of the player's bullets

};