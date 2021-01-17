#include "stdafx.h"
#include "Renderer.h"

#include <iostream>

#include <assert.h>

Renderer::Renderer(sf::RenderWindow &window) 
	:	window{ &window }
{
}


Renderer::~Renderer()
{
}

void Renderer::Render(RenderDescription const &desc) {

	assert(desc.sprite != nullptr);

	desc.sprite->setPosition(desc.position);

	window->draw(*desc.sprite);
}

void Renderer::Render(std::vector<RenderDescription> list) {
	

	for (std::size_t i = 0; i < list.size(); ++i) {
		assert(list[i].sprite != nullptr);

		list[i].sprite->setPosition(list[i].position);

		window->draw(*list[i].sprite);
	}
}