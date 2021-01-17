#include "stdafx.h"
#include "Player.h"

#include <iostream>
#include <vector>

Player::Player(MySprite &mySprite, sf::Sprite &new_hitbox, sf::Vector2f new_position, float unfocusedSpeed, float focusedSpeed, BulletPattern bulletPattern)
	: mySprite{ &mySprite }, unfocusedSpeed{ unfocusedSpeed }, focusedSpeed{ focusedSpeed }, bulletPattern{ bulletPattern }
{
	lives = config::PLAYER_INITIAL_LIVES;
	bombs = config::PLAYER_INITIAL_BOMBS;

	iTime = 0;;

	position = new_position;

	hitbox = &new_hitbox;

	moveTimer.restart(); //start the move timer}
}

Player::~Player()
{

}


void Player::Move() { //the player moves based on user input
	
	float moveSpeed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
		moveSpeed = focusedSpeed;
	}
	else {
		moveSpeed = unfocusedSpeed;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) and sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		position.x += moveSpeed * moveTimer.getElapsedTime().asSeconds() / sqrt(2);
		position.y += moveSpeed * moveTimer.getElapsedTime().asSeconds() / sqrt(2);
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) and sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		position.x -= moveSpeed * moveTimer.getElapsedTime().asSeconds() / sqrt(2);
		position.y += moveSpeed * moveTimer.getElapsedTime().asSeconds() / sqrt(2);
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) and sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		position.x += moveSpeed * moveTimer.getElapsedTime().asSeconds() / sqrt(2);
		position.y -= moveSpeed * moveTimer.getElapsedTime().asSeconds() / sqrt(2);
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) and sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		position.x -= moveSpeed * moveTimer.getElapsedTime().asSeconds() / sqrt(2);
		position.y -= moveSpeed * moveTimer.getElapsedTime().asSeconds() / sqrt(2);
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		float time = moveTimer.getElapsedTime().asSeconds();
		position.y += moveSpeed * time;
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		float time = moveTimer.getElapsedTime().asSeconds();
		position.y -= moveSpeed * time;
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		float time = moveTimer.getElapsedTime().asSeconds();
		position.x += moveSpeed * time;	
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		float time = moveTimer.getElapsedTime().asSeconds();
		position.x -= moveSpeed * time;
	}

	//if the player has moved past the bounds of the window set his position to the edge of the window
	if (position.x < 0) { position.x = 0; }
	if (position.x > config::WINDOW_RESOLUTION[0] - mySprite->GetWidth()) { position.x = config::WINDOW_RESOLUTION[0] - mySprite->GetWidth(); }
	if (position.y < 0) { position.y = 0; }
	if (position.y > config::WINDOW_RESOLUTION[1] - mySprite->GetHeight()) { position.y = config::WINDOW_RESOLUTION[1] - mySprite->GetHeight(); }
	
	moveTimer.restart();
}

void Player::UpdateSprite() { //updates the sprite based on the player's state
	
	if (iTime > 0) {
		mySprite->GetCurrentSprite().setColor(sf::Color::Cyan);
	}
	else {
		mySprite->GetCurrentSprite().setColor(sf::Color::White);
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		if (mySprite->GetRow() != config::PLAYER_MOVING_RIGHT_SPRITE_ROW) {
			mySprite->SetLoop(config::PLAYER_MOVING_RIGHT_LOOP_START, config::PLAYER_MOVING_RIGHT_LOOP_END, config::PLAYER_MOVING_RIGHT_SPRITE_ROW);
		}
		mySprite->Next();
		return;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ){
		if (mySprite->GetRow() != config::PLAYER_MOVING_LEFT_SPRITE_ROW) {
			mySprite->SetLoop(config::PLAYER_MOVING_LEFT_LOOP_START, config::PLAYER_MOVING_LEFT_LOOP_END, config::PLAYER_MOVING_LEFT_SPRITE_ROW);
		}
		mySprite->Next();
		return;
	}
	else {
		if (mySprite->GetRow() != config::PLAYER_MOVING_FORWARD_SPRITE_ROW) {
			mySprite->SetLoop(config::PLAYER_MOVING_FORWARD_LOOP_START, config::PLAYER_MOVING_FORWARD_LOOP_END, config::PLAYER_MOVING_FORWARD_SPRITE_ROW);
		}
		mySprite->Next();
		return;
	}
}

PlayerDescription Player::GetDescription() const { //get info for hud
	return PlayerDescription(lives, bombs);
}

RenderDescription Player::GetRenderDescription() const {
	return RenderDescription(position, mySprite->GetCurrentSprite());
}

RenderDescription Player::GetHitboxRenderDescription() const {
	
	sf::Vector2f pos = { position.x + (mySprite->GetWidth() / 2), position.y + (mySprite->GetHeight() / 2) }; //set pos to the center of the player sprite
	
	pos = { pos.x - hitbox->getTextureRect().width / 2, pos.y - hitbox->getTextureRect().height / 2 }; //set pos to the top left of the hitbox

	return RenderDescription(pos, *hitbox);

}


void Player::Shoot(std::vector<Bullet*> &bullets) { //The player creates and adds bullets to the list
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) { //if the shoot key is pressed and the time has passed the pattern's fireRate

		sf::Vector2f pos = { position.x + (mySprite->GetWidth() / 2), position.y + (mySprite->GetHeight() / 2) }; //gets the position in the center of the player

		std::vector<Bullet> list = bulletPattern.GetBullets(pos, position);

		for (std::size_t i = 0; i < list.size(); ++i) { //create the bullets from the descriptions in the list

			list[i].ResetTimers();

			bullets.push_back(new Bullet(list[i]));
		}
	}
}


CircleHitbox Player::GetHitbox() const { //this needs to be changed to a circle hitbox at some point
	CircleHitbox circ(position.x + (mySprite->GetWidth() / 2) , position.y + (mySprite->GetHeight() / 2), (hitbox->getTextureRect().width / 2));
	return circ;
}


void Player::Collide() {
	if (!(iTime > 0)) {
		if (lives > 0) {
			lives--;	//lose a life
			bombs = config::PLAYER_INITIAL_BOMBS; //Reset number of bombs
			iTime = config::PLAYER_INVULNERABLE_TIME_AFTER_HIT;	
			iTimer.restart();
			
		}
	}
}

bool Player::CheckGameOver() {
	if (lives < 1) { return true; }
	else { return false; }
}

void Player::Update() { //Generic update funtion to update the player's state
	Move();
	if (iTime > 0) {
		iTime = iTime - iTimer.getElapsedTime().asSeconds();
		if (iTime < 0) {
			iTime = 0;
		}
		iTimer.restart();
	}
	UpdateSprite();
}


void Player::Bomb(std::vector<Bullet*> &bullets) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) and bombs > 0 and !(iTime > 0)) {
		
		for (std::vector<Bullet*>::iterator it = bullets.begin(); it != bullets.end();) { //delete non-friendly bullets
			if (not (*it)->IsFriendly()) {	//if the bullet isn't friendly erase it
				delete *it; *it = nullptr;
				it = bullets.erase(it);
			}
			else {
				it++;
			}
		}
		
		bombs--; //reduce the number of bombs by one
		
		iTime = config::PLAYER_INVULNERABLE_TIME_AFTER_BOMB; //make the player invulnerable
		iTimer.restart();
	}
}

void Player::Reset() {
	lives = config::PLAYER_INITIAL_LIVES;
	bombs = config::PLAYER_INITIAL_BOMBS;

	iTime = 0;;

	position = config::PLAYER_INITIAL_POSITION;

	moveTimer.restart(); //start the move timer}
}

