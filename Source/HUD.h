#pragma once

#include <vector>

#include "MySprite.h"
#include "PlayerDescription.h"
#include "RenderDescription.h"

#include <iostream>

class HUD
{
public:
	HUD(MySprite livesSprite, MySprite bombsSprite, PlayerDescription desc);
	~HUD();

	void Update(PlayerDescription desc);

	std::vector<RenderDescription> GetRenderDescription();


private:
	MySprite livesSprite;
	MySprite bombsSprite;
	PlayerDescription player;
	
	

};

