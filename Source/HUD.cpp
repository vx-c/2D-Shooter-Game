#include "stdafx.h"
#include "HUD.h"


HUD::HUD(MySprite livesSprite, MySprite bombsSprite, PlayerDescription desc) : livesSprite{ livesSprite }, bombsSprite{ bombsSprite }, player { desc }
{
}


HUD::~HUD()
{
}

void HUD::Update(PlayerDescription desc) {
	player = desc;
}


std::vector<RenderDescription> HUD::GetRenderDescription() {
	std::vector<RenderDescription> list;
	
	float x = 30, y = 20;
	for (std::size_t i = 0; i < player.lives; ++i) {
		RenderDescription desc({ x,y }, livesSprite.GetCurrentSprite());
		x = x + 45;
		list.push_back(desc);
	}

	x = 1030;
	y = 24;

	for (std::size_t i = 0; i < player.bombs; ++i) {
		RenderDescription desc({ x,y }, bombsSprite.GetCurrentSprite());
		x = x + 25;
		list.push_back(desc);
	}

	return list;
}