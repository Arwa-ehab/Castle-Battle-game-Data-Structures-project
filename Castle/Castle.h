#pragma once
#include "..\Defs.h"
class Castle
{
	double Health;
	int noOfAttackedEnemies;
	int power;
	//Noha
	const double threshold=15;
	ENMY_STATUS status;
	//
	// TODO: Add More Data Members As Needed
	//
	//seif
	double OriginalHealth;
public:
	void SetnoOfAttackedEnemies(int N);
	int GetnoOfAttackedEnemies();
	void SetPower(int p);
	int GetPower();
	void SetHealth(double h);
	double GetHealth() const;
	//noha
	void DecCastleHealth(double DecVal);
	double GetThreshold();
	void SetStatus(ENMY_STATUS ActiveOrFrosted);
	ENMY_STATUS GetStatus();
	//seif
	void SetOriginalHealth(double h);
	double GetOriginalHealth();
	//
	// TODO: Add More Member Functions As Needed
	//

};

