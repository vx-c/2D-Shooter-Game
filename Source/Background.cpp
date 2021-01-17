#include "stdafx.h"
#include "Background.h"


Background::Background(sf::Sprite &new_sprite, float speed, float windowYResolution)
	: speed{ speed }, windowYResolution{ windowYResolution }
{
	sprite = &new_sprite;

	length = sprite->getTextureRect().height;

	position = 0 - length + windowYResolution;

	moveTimer.restart();
}


Background::~Background()
{
}

void Background::Update() {
	Move();
}


void Background::Move() {
	if (position > windowYResolution) { //if the background has completed its loop
		position = 0 - length + windowYResolution;
	}
	
	position = position + speed * moveTimer.getElapsedTime().asSeconds(); //move the background based on its speed and the time passed

	moveTimer.restart(); //reset the timer

}


std::vector<RenderDescription> Background::GetRenderDescription() {

	return { {{0.f, position }, *sprite}, {{ 0.f, position - length }, *sprite} };
}



