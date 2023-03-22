#pragma once
#include"Enemies/Enemy.h"

class Fighter : public Enemy {

public:
	Fighter(int id, int typ, int arrTime, double Health, int p, int rp, int s, ENMY_STATUS EnemyStatus);
	//arwa
	int CalculatePriority();
	//arwa
	void DecHealth(int castle_power);
	//arwa
	void Move();
	//arwa
	void Action(Castle* castle);
};