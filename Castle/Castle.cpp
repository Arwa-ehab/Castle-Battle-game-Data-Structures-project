#include "Castle.h"




void Castle::SetHealth(double h)
{
	if (h > 0)
		Health = h;
	else
		Health = 0; // killed
}

double Castle::GetHealth() const
{
	return Health;
}
void Castle::SetnoOfAttackedEnemies(int N) {
	noOfAttackedEnemies = N;
}
int Castle::GetnoOfAttackedEnemies() {
	return noOfAttackedEnemies;
}
void Castle::SetPower(int p) {
	power = p;
}
int Castle::GetPower() {
	return power;
}

//noha
void Castle::DecCastleHealth(double DecVal) {
	double h= Health - DecVal;
	SetHealth(h);

}
//noha
double Castle::GetThreshold() {
	return threshold;
}
//noha
void Castle::SetStatus(ENMY_STATUS ActiveOrFrosted) {
	status = ActiveOrFrosted;
}

ENMY_STATUS Castle::GetStatus()
{
	return status;
}
void Castle::SetOriginalHealth(double hlth)
{
	OriginalHealth = hlth;
	SetHealth(hlth);
}
double Castle::GetOriginalHealth()
{
	return OriginalHealth;
}