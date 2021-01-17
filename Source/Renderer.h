#pragma once

#include "RenderDescription.h"
#include <SFML\Graphics.hpp>

class Renderer
{
public:
	Renderer(sf::RenderWindow &window);
	~Renderer();

	void Render(RenderDescription const &desc);
	void Render(std::vector<RenderDescription> list);

private:
	sf::RenderWindow *window;

};

