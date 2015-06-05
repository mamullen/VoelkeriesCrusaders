#include "stdafx.h"
#include "PowerProjectile.h"
#include "GameLogic.h"

PowerProjectile::PowerProjectile()
{
	hitRadius = 24;
	isRecoil = true;
	isStun = true;
	recoilAmount = 20;
}


PowerProjectile::~PowerProjectile()
{
}
