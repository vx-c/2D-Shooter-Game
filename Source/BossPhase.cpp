#include "stdafx.h"
#include "BossPhase.h"


BossPhase::BossPhase(int hp, float moveSpeed, std::vector<PathPoint> path, std::vector<BulletPattern> bulletPatterns)
	: hp{ hp }, moveSpeed{ moveSpeed }, path{ path }, bulletPatterns{ bulletPatterns }
{
}


BossPhase::~BossPhase()
{
}
