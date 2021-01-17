#include "stdafx.h"
#include "MySprite.h"

#include <assert.h>

#include <iostream>

/*
MySprite::MySprite(sf::Sprite &sprite, sf::Vector2i spriteSize, int maxSprites, float updateRate) : spriteSize{ spriteSize }, maxSprites { maxSprites }, updateRate{ updateRate }
{
	baseSprite = &sprite;

	currentSprite = 1; // the curent sprite is number 1

	animationTimer.restart(); //start the animation timer
}
*/

MySprite::MySprite(sf::Texture &texture, sf::Vector2i spriteSize, int end, float updateRate) 
	: spriteSize{ spriteSize }, end{ end }, updateRate{ updateRate }
{
	sprite.setTexture(texture);

	currentSprite = 1; // the curent sprite is number 1
	
	//the intial loop
	start = 1; //start at 1
	row = 1; //start on row 1

	animationTimer.restart(); //start the animation timer
}

MySprite::MySprite(sf::Texture &texture, sf::Vector2i spriteSize, int start, int end, int row, float updateRate)
	: spriteSize{ spriteSize },start{ start }, end{ end }, row{ row }, updateRate{ updateRate }
{
	sprite.setTexture(texture);

	currentSprite = start; // the curent sprite is number 1

	animationTimer.restart(); //start the animation timer
}

MySprite::MySprite() {

}

MySprite::~MySprite()
{
}

void MySprite::Next() {
	
	if (animationTimer.getElapsedTime().asSeconds() > updateRate) {
		if (currentSprite == end) { //if we're at the last sprite set the current sprite back to the first which is 0
			currentSprite = start;
		}
		else { //else increment currentSprite
			++currentSprite;
		}

		assert((currentSprite >= start and currentSprite <= end));

		animationTimer.restart(); //restart the timer
	}
}

void MySprite::ChangeRow(int x) {
	row = x;
}

void MySprite::SetLoop(int new_start, int new_end, int new_row) {
	start = new_start;
	currentSprite = start; //reset the current sprite to the first int he new loop
	end = new_end;
	row = new_row;
}

sf::Sprite& MySprite::GetCurrentSprite() {
	sprite.setTextureRect(sf::IntRect({ (currentSprite - 1) * spriteSize.x, (row - 1) * spriteSize.y }, spriteSize));

	return sprite;
}

int MySprite::GetWidth() const {
	return spriteSize.x;
}

int MySprite::GetHeight() const {
	return spriteSize.y;
}

int MySprite::GetRow() const {
	return row;
}

void MySprite::SetUpdateRate(float rate) {
	updateRate = rate;
}