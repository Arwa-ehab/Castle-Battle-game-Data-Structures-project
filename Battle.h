 #pragma once

#include "Enemies\Enemy.h"
#include "Castle\Castle.h"
#include"Freezer.h"
#include"Fighter.h"
#include"Healer.h"
#include "Generic_DS\Queue.h"
#include"ArrayStack.h"
#include"PriorityQueue.h"
#include "GUI\GUI.h"
#include "Stack.h"
// it is the controller of the project
class Battle
{
private:
	GUI* pGUI;
	Castle BCastle; //to be able to access the same castle in other classes
	int EnemyCount;	//the actual number of enemies in the game
	int ActiveCount, FrostedCount, KilledCount;	//no. of enemies (Active, Frosted, killed so far)
	int CurrentTimeStep;
	//Enemy * BEnemiesForDraw[MaxEnemyCount]; // This Array of Pointers is used for drawing elements in the GUI
											// No matter what list type you are using to hold enemies, 
											// you must pass the enemies to the GUI function as an array of enemy pointers. 
											// At every time step, you should update those pointers 
											// to point to the current active enemies 
											// then pass the pointers list to the GUI function

	Queue<Enemy*> Q_Inactive;		//Queue of inactive enemies
	//Noha(needed to be modified after finishing the missing ADTS)
	Queue<Enemy*> Killed;
	Queue<Enemy*> Frozen;
	Queue<Freezer*> Active_Freezer;   
	Queue<Fighter*> Active_Fighter;
	Stack<Healer*>Active_Healer;
	Queue<Fighter*>Print_Fighter;
	Queue<Freezer*>Print_Freezer;
	Queue<Healer*>Print_Healer;

	//arwa
	PriorityQueue<Fighter*> ToKill_Fighters; //castle chooses which fighter to kill according to priority
	//Seif
	int CastleDamage;
	/// ==> 
	//	DEMO-related members. Should be removed in phases 1&2
	//Queue for all enemies in the battle
	int DemoListCount;
	Enemy* DemoList[MaxEnemyCount];	//Important: This is just for demo
	/// ==>


	//
	// TODO: Add More Data Members As Needed
	//

public:

	Battle();

	void AddAllListsToDrawingList();		//Add enemy lists (inactive, active,.....) to drawing list to be displayed on user interface
	 Castle* GetCastle();
	void RunSimulation();
	void ActivateEnemies();		//check the inactive list and activate all enemies that has arrived


	void AddtoDemoList(Enemy* Ptr);		//Add Enemy to the demo queue of enemies (for demo purposes only)
	//void Just_A_Demo();	//just to show a demo and should be removed in phase1 1 & 2
	void Demo_UpdateEnemies();	//Randomly update enemies distance/status (for demo purposes)
	void InputFunction(string inputData);
	void Outputfunction();
	//arwa
	void FrozenActive();
	void FightersToKill();//orders active fighters according to priority and fills ToKill_Freezers list
	                       //should be called at each time step
	void HealingEnemies(); //heals enemies after they are attacked;
						  //healer heals only when enemy->GetHealth() < 0.75* enemy->GetOriginalHealth()
	//noha
	void CastleAttackFire(int Time);
	void CastleAttackIce();
	void GameLogicInterActiveMode();
	void GameLogicStepMode();
	void GameLogicSilentMode();
	void EnemiesMoveAction();
	void CountOfEnemiesFrozenKilled(int &frez_fighters, int &frez_freezer, int& frez_healer, int& kld_fighters, int &kld_freezer, int& kld_healer);
	void CountOfActiveEnemies(int& actv_fighters, int& actv_freezer, int &actv_healer);
	//

	//Seif

	// TODO: Add More Member Functions As Needed
	//
};

