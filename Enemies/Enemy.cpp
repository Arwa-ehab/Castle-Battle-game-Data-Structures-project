#include "Enemy.h"

Enemy::Enemy(int id, int arrTime, double H, int d) :ID(id), ArrvTime(arrTime), originalHealth(H) {
	SetDistance(d);

}
Enemy::Enemy(int id, int typ, int arrTime, double orHealth, int p, int rp, int s, ENMY_STATUS EnemyStatus, int d,int reload_count,bool action) : ID(id), ArrvTime(arrTime), originalHealth(orHealth)
{
	FreezeTime = 0;
	Health = orHealth;
	power = p;
	reloadPeriod = rp;
	speed = d;
	status = EnemyStatus;
	type = typ;
	SetDistance(d);
	reload_counter = reload_count;
	canTakeAction = action;
}
double Enemy::GetHealth() {
	return Health;
}
double Enemy::GetOriginalHealth()
{
	return originalHealth;
}
int Enemy::GetType() {
	return type;
}
//Seif
int Enemy::Freeze(int Power, int CrntTimeStep)
{
	if (this->Health != 0)
	{
		FreezeTime = 0.7 * Power + 6.0 /this-> Health;
		SetStatus(FRST);
		TimeGotFrozen = CrntTimeStep;
		return FreezeTime;
	}
}
void Enemy::SetDeathTime(int time)
{
	DeathTime = time;
}
void Enemy::SetFirstTime(int time)
{
	FirstHit = time;
}
int Enemy::GetDeathTime()
{
	return DeathTime;
}
int Enemy::GetFirstTime()
{
	return FirstHit;
}
int Enemy::FirstShotDelay()
{
	return FirstHit-ArrvTime;
}
int Enemy::KillDelay()
{
	return DeathTime-FirstHit;
}
int Enemy::Lifetime()
{
	return DeathTime-ArrvTime;
}
void Enemy::SetFreezeTime(int time)
{
	FreezeTime = time;
}
int Enemy::GetFreezeTime()
{
	return FreezeTime;
}
//
Enemy::~Enemy()
{
}

int Enemy::GetID() const
{
	return ID;
}


void Enemy::SetStatus(ENMY_STATUS s)
{
	//if (s == KILD)
	status = s;
}
//arwa
void Enemy::SetHealth(int affected_health)
{
	Health = affected_health;
	if (Health <= 0)
		SetStatus(KILD); //Seif
}


ENMY_STATUS Enemy::GetStatus() const
{
	return status;
}


void Enemy::DecrementDist()
{
	if (Distance > MinDistance)
		Distance--;
}

double Enemy::GetOriginalHealth() const
{
	return originalHealth;
}

void Enemy::SetDistance(int d)
{
	if (d >= MinDistance && d <= MaxDistance)
		Distance = d;
	else
		Distance = MaxDistance;
}

int Enemy::GetDistance() const
{
	return Distance;
}


//arwa
int Enemy::GetSpeed()
{
	int check_health = originalHealth * 0.5;
	if (Health < check_health)
	{
		speed = 0.5 * speed;
		
	}
	return speed;
}
//arwa
bool Enemy::GetcanTakeAction()
{
	return canTakeAction;
}
//arwa
void Enemy::SetCanTakeAction()
{
	if (reload_counter == 0)
		canTakeAction = true;
	else
		canTakeAction = false;
}
//arwa
void Enemy::ReloadCounter()
{
	
	if (reload_counter > reloadPeriod)
		reload_counter = 0;
	this->SetCanTakeAction();
	reload_counter++;
}

int Enemy::GetArrvTime() const
{
	return ArrvTime;
}
//seif
bool Enemy::isFrozen(int CrntStep)
{
	if (CrntStep - TimeGotFrozen < FreezeTime)
		return true;
	else
	{
		SetStatus(ACTV);
		return false;
	}
}
//Noha
//void Enemy::DecHealth(int power) {
//	double newHealth = (1 / Distance) * power * (1);
//	Health = Health - newHealth;
//	//for healer class override it
//}
