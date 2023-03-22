#pragma once

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include"..\Castle\Castle.h"
class GUI;
// Enemy is the base class of each type of enemy
// Enemy should be an abstract class in next phases
class Enemy
{


protected:
	const int ID;         //Each enemy has a unique ID (sequence number)
	const int ArrvTime;	//arrival time (time it starts to join battle)

	ENMY_STATUS status;	    //status of the enemy (inactive, inactive, frosted, killed)
	int Distance;	//Horizontal distance between enemy & the tower of its region
					//Always positive (ranges from 2 to 60)
	double Health;	//Enemy health
	//noha
	const double originalHealth;
	int power;
	int reloadPeriod;
	int speed;
	int type;
	//arwa
	int reload_counter;      //counts the reload time
	bool canTakeAction;//used to check when the enemy can take action
		//arwa
	void SetCanTakeAction();   //utility function
	//Seif
	int DeathTime;//The timestep the enemy died at
	int FirstHit;//The timestep the enemy got 1st hit by the castle
	int FreezeTime;//Timesteps the player is forbidden from moving
	int TimeGotFrozen;
	// TODO: Add More Data Members As Needed
	//
	// first shot, kill
public:
	Enemy(int id, int tp, int arrTime, double originalHealth, int p, int rp, int s, ENMY_STATUS EnemyStatus, int d = MaxDistance,int reload_count=0,bool action=true);
	virtual ~Enemy();
	Enemy(int id, int arrTime, double h, int d = MaxDistance);
	int Enemy::GetID() const;
	ENMY_STATUS GetStatus() const;
	void SetStatus(ENMY_STATUS);
	//arwa
	void SetHealth(int affected_health);
	void DecrementDist();
	//arwa
	double GetOriginalHealth() const;
	void SetDistance(int);
	int GetDistance() const;
	//arwa
	//an enemy moves at its original speed each time step.
	//But an enemy with(health < 0.5 its original health) moves at its half original speed
	int GetSpeed();
	//arwa
	bool GetcanTakeAction();  //call this function in every enemy action()
	                           //if it return true enemy can act and is not at reload time
	//arwa
	void ReloadCounter();// call it at the beginning of every enemy action function;
	int GetArrvTime() const;
	//noha
	virtual void DecHealth(int castle_power)=0;
	//Healer should override DecHealth
	virtual void Move()=0;
	//Healer should override move
	virtual void Action(Castle*C) =0;
	double GetHealth();
	double GetOriginalHealth();
	int GetType();
	//Seif
	int Freeze(int Power, int CrntTimeStep);
	void SetDeathTime(int time);
	void SetFirstTime(int time);
	int GetDeathTime();
	int GetFirstTime();
	int FirstShotDelay();
	int KillDelay();
	int Lifetime();
	void SetFreezeTime(int time);
	int GetFreezeTime();
	bool isFrozen(int CrntStep);
	// Virtual Functions: ----------------

	//virtual void Move() = 0;	//All enemies can move
	//virtual void Act() = 0;	//Acting means fighting or healing

	//
	// TODO: Add More Member Functions As Needed
	//

};

