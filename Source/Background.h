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

	int length;

	sf::Clock moveTimer;
	sf::Sprite *sprite;
};

