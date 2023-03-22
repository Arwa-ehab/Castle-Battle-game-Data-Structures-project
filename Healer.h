#pragma once
#include"Enemies/Enemy.h"

class Healer : public Enemy {
private:
	char direction;
public:
	Healer(int id, int typ, int arrTime, double Health, int p, int rp, int s, ENMY_STATUS EnemyStatus, char Direction='F');
	//arwa
	 void Move();
	virtual void Action(Enemy* enemy);
	void Action(Castle* c);
	void DecHealth(int castle_power);
	
};