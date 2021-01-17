#include "stdafx.h"
#include "RenderDescription.h"


RenderDescription::RenderDescription(sf::Vector2f position, sf::Sprite &sprite)
	:	position{ position }, sprite{ &sprite }
{
}


RenderDescription::~RenderDescription()
{
}
