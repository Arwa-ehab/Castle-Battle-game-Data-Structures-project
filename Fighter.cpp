#include "Fighter.h"
#include"Enemies/Enemy.h"
Fighter::Fighter(int id, int typ, int arrTime, double Health, int p, int rp, int s, ENMY_STATUS EnemyStatus) :Enemy(id, typ, arrTime, Health, p, rp, s, EnemyStatus) {

}
//arwa
int Fighter::CalculatePriority()
{
	int priority;
	int reload_difference = reloadPeriod - reload_counter;
	int statusnumber  ;
	if (this->GetStatus() == ACTV)
		statusnumber = 1;
	if (this->GetStatus() == FRST)
		statusnumber = 0;
	if (Health < Health * 0.5)
	{
		priority= Health + (1.0 / (float)reload_difference) + ((double)power*0.5 / (double)this->GetDistance()) + statusnumber;
	}
	else 		
		priority = Health + (1.0 / (float)reload_difference) + ((double)power  / (double)this->GetDistance()) + statusnumber;
	return priority;
}
//arwa
void Fighter::DecHealth(int castle_power)
{
	double damage = (1.0 / (float)this->GetDistance()) * castle_power ;
	double Dec_health = this->GetHealth() - damage;
	this->SetHealth(Dec_health);
}
//arwa
void Fighter::Move()
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
//arwa
void Fighter::Action(Castle* castle)
{
	this->ReloadCounter();
	if (this->GetStatus() == ACTV && this->GetcanTakeAction())
	{
		int check_health = originalHealth * 0.5;
		double damage;
		if (Health < check_health)
		{
			damage = 0.5 * (1.0 / (float)this->GetDistance()) * this->power;
			
		}
		else
			damage =  (1.0 / (float)this->GetDistance()) * this->power;
			castle->DecCastleHealth(damage);

			if (castle->GetHealth() < 0)
				castle->SetStatus(KILD);
	}
	

}


