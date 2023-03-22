#pragma once
#include"Enemies/Enemy.h"
#include"Castle/Castle.h"
class Freezer : public Enemy
{
	
public:
	Freezer(int id, int typ, int arrTime, double Health, int p, int rp, int s, ENMY_STATUS EnemyStatus);
	virtual void Action(Castle*C);
	//arwa
	void DecHealth(int castle_power);
	void Move();
	
};

