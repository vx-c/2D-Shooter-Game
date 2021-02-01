#include "stdafx.h"
#include "MySprite.h"

#include <assert.h>

#include <iostream>

MySprite::MySprite(sf::Texture &texture, sf::Vector2i spriteSize, int end, float updateRate) 
	: spriteSize{ spriteSize }, end{ end }, updateRate{ updateRate }
{
	sprite.setTexture(texture);

	currentSprite = 1; 
	
	start = 1; 
	row = 1; 

	animationTimer.restart(); 
}

MySprite::MySprite(sf::Texture &texture, sf::Vector2i spriteSize, int start, int end, int row, float updateRate)
	: spriteSize{ spriteSize },start{ start }, end{ end }, row{ row }, updateRate{ updateRate }
{
	sprite.setTexture(texture);

	currentSprite = start; 

	animationTimer.restart(); 
}

MySprite::MySprite() {

}

MySprite::~MySprite()
{
}

void MySprite::Next() {
	
	if (animationTimer.getElapsedTime().asSeconds() > updateRate) {
		if (currentSprite == end) { 
			currentSprite = start;
		}
		else { 
			++currentSprite;
		}

		assert((currentSprite >= start and currentSprite <= end));

		animationTimer.restart();
	}
}

void MySprite::ChangeRow(int x) {
	row = x;
}

void MySprite::SetLoop(int new_start, int new_end, int new_row) {
	start = new_start;
	currentSprite = start; 
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
