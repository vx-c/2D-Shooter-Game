#pragma once

#include <SFML/Graphics.hpp>

#include "RenderDescription.h"

#include "Config.h"

class Background
{
public:
	Background(sf::Sprite &new_sprite, float speed, float windowYResolution);
	~Background();

	void Update();

	void Move();

	std::vector<RenderDescription> GetRenderDescription();

private:
	float position;
	float speed; //the number of pixels the background moves each second

	float windowYResolution;

	int length; //the length of the background

	sf::Clock moveTimer; //timer for the movement of the background
	sf::Sprite *sprite;
};

