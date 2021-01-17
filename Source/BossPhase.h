#pragma once

#include "BulletPattern.h"
#include <vector>
#include "PathPoint.h"

class BossPhase
{

public:
	BossPhase(int hp, float moveSpeed, std::vector<PathPoint> path, std::vector<BulletPattern> bulletPatterns);
	~BossPhase();

	int hp;
	float moveSpeed;
	std::vector<PathPoint> path;
	std::vector<BulletPattern> bulletPatterns;

};

