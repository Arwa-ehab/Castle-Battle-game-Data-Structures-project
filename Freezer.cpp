#include "Freezer.h"
#include"Enemies/Enemy.h"

//noha
Freezer::Freezer(int id, int typ, int arrTime, double Health, int p, int rp, int s, ENMY_STATUS EnemyStatus) :Enemy(id, typ, arrTime, Health, p, rp, s, EnemyStatus) {

}
void Freezer::Action(Castle*C) {
	this->ReloadCounter();
	//int decHealthValue = abs(Health + power - Distance);
	if (this->GetStatus() == ACTV && this->GetcanTakeAction())
	{
		double amountOfIce = abs(Health * 0.1 - Distance*0.1);
		double accumulateEfect = 0;
		accumulateEfect += amountOfIce;
		if (accumulateEfect > C->GetThreshold()) {
			C->SetStatus(FRST);
			accumulateEfect = 0;
		}
		
	}
	
}
//arwa
void Freezer::DecHealth(int castle_power)
{
	double damage = (1.0 / (float)this->GetDistance()) * castle_power;
	double Dec_health = this->GetHealth() - damage;
	this->SetHealth(Dec_health);
}
//arwa
void Freezer::Move()
{
	if (this->GetStatus() == ACTV)
	{
		int speedcopy = this->GetSpeed();
		int difference = this->GetDistance() - MinDistance;
		if (speedcopy > difference)
			Distance = MaxDistance - (speedcopy - difference);
		else
			Distance = Distance - speedcopy;
	}


}
