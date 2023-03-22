#include "Healer.h"

#include"Enemies/Enemy.h"
Healer::Healer(int id, int typ, int arrTime, double Health, int p, int rp, int s, ENMY_STATUS EnemyStatus, char direc ) :Enemy(id, typ, arrTime, Health, p, rp, s, EnemyStatus) {
	direction = direc;

}
//arwa
void Healer::Move()
{
	if (this->GetStatus() == ACTV)
	{
		int calculated_speed = this->GetSpeed();
		if (direction == 'F')
		{
			int check = this->GetDistance() - 2;
			if (check < calculated_speed)
			{
				int calculated_distance = MinDistance + (calculated_speed - check);
				this->SetDistance(calculated_distance);
				direction = 'B';
			}
			else
			{
				int calculated_distance1 = this->GetDistance() - calculated_speed;
				this->SetDistance(calculated_distance1);
			}
		}
		if (direction == 'B')
		{
			int difference = MaxDistance - this->GetDistance();
			if (difference < calculated_speed)
			{
				int calculated_distance = MaxDistance - (calculated_speed - difference);
				this->SetDistance(calculated_distance);
				direction = 'F';

			}
			else {
				int calculated_distance1 =this->GetDistance() + calculated_speed;
				this->SetDistance(calculated_distance1);
				
			}
		}
	}
}
//arwa
void Healer::Action(Enemy* enemy)
{
	this->ReloadCounter();
	if (this->GetStatus() == ACTV &&this->GetcanTakeAction() && enemy->GetStatus()==ACTV)
	{
		double new_health;
		int distance_difference;
		
			distance_difference = abs(enemy->GetDistance() - this->GetDistance());
		
			
		
	
		if (enemy->GetHealth() < 0.75* enemy->GetOriginalHealth())
		{
			if (this->GetHealth() < 0.5 * this->originalHealth)
				new_health = enemy->GetHealth() * 1.1 + (0.2 / distance_difference);
			else 
				new_health= enemy->GetHealth() * 1.15 + (0.2 / distance_difference);
			enemy->SetHealth(new_health);
		}
		

	}
}
//arwa
void Healer::DecHealth(int castle_power)
{
	double damage = (1.0 / (float)this->GetDistance()) * castle_power * 0.5;
	double Dec_health = this->GetHealth() - damage;
	this->SetHealth(Dec_health);
}
void Healer::Action(Castle* C) {

}
