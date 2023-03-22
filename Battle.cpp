#include "Battle.h"
#include <time.h>
#include<fstream>
#include<iostream>

#include <stdio.h>
#include <stdlib.h>
using namespace std;

//(Noha)
void Battle::InputFunction(string inputData) {
	fstream inputFile;
	inputFile.open(inputData);
	double HC;
	int N, CP, M;
	inputFile >> HC >> N >> CP;
	BCastle.SetOriginalHealth(HC);
	BCastle.SetnoOfAttackedEnemies(N);
	BCastle.SetPower(CP);
	BCastle.SetStatus(ACTV);
	inputFile >> M;
	EnemyCount = M;
	for (int i = 0; i < M; i++) {
		int ID, TYP, AT, POW, RLD, SPD;
		double H;
		inputFile >> ID >> TYP >> AT >> H >> POW >> RLD >> SPD;
		Enemy* E;
		if (TYP == 0) {
			E = new Fighter(ID, TYP, AT, H, POW, RLD, SPD, INAC);
			Q_Inactive.enqueue(E);
		}
		else if (TYP == 1) {
			E = new Healer(ID, TYP, AT, H, POW, RLD, SPD, INAC);
			Q_Inactive.enqueue(E);
		}
		else if (TYP == 2) {
			E = new Freezer(ID, TYP, AT, H, POW, RLD, SPD, INAC);
			Q_Inactive.enqueue(E);
		}
	}
}

//Seif
void Battle::Outputfunction()
{
	ofstream MyFile("GameResult.txt");
	Enemy* KilledEnemy;
	int FSD = 0;
	int KD = 0;
	int NoOfKilled = 0;
	int NoOfAlive = 0;
	if (BCastle.GetHealth() <= 0)
	{
		MyFile << "Game is LOSS" << endl;
		MyFile << "KTS   ID     FD   KD   LT" << endl;
		while (!Killed.isEmpty())
		{
			Killed.dequeue(KilledEnemy);
			NoOfKilled++;
			FSD += KilledEnemy->FirstShotDelay();
			KD += KilledEnemy->KillDelay();
			MyFile << KilledEnemy->GetDeathTime() << "   " << KilledEnemy->GetID() << "     " << KilledEnemy->FirstShotDelay() << "   " << KilledEnemy->KillDelay() << "   " << KilledEnemy->Lifetime() << endl;
		}
		MyFile << "----------------------------------------------------" << endl;

		while (!Active_Fighter.isEmpty())
		{
			NoOfAlive++;
		}
		while (!Active_Freezer.isEmpty())
		{
			NoOfAlive++;
		}
		while (!Active_Healer.isEmpty())
		{
			NoOfAlive++;
		}
		MyFile << "----------------------------------------------------" << endl << endl;

		MyFile << "Castle Total Damage       = " << BCastle.GetOriginalHealth() - BCastle.GetHealth() << "\n";
		MyFile << "Total Enemies ALive       = " << NoOfAlive << "\n";
		MyFile << "Total Enemies Killed      = " << KilledCount << "\n";
		
			MyFile << "Average First-Shot Delay  = " << FSD / NoOfKilled << "\n";
			MyFile << "Average Kill Delay        = " << KD / NoOfKilled << "\n";
		
	}
	else if (Active_Fighter.isEmpty() && Active_Freezer.isEmpty() && Active_Healer.isEmpty() && Frozen.isEmpty() && Q_Inactive.isEmpty())
	{
		MyFile << "Game is WIN" << endl;
		MyFile << "KTS   ID     FD   KD   LT" << endl;
		while (!Killed.isEmpty())
		{
			Killed.dequeue(KilledEnemy);
			NoOfKilled++;
			FSD += KilledEnemy->FirstShotDelay();
			KD += KilledEnemy->KillDelay();
			MyFile << KilledEnemy->GetDeathTime() << "   " << KilledEnemy->GetID() << "     " << KilledEnemy->FirstShotDelay() << "   " << KilledEnemy->KillDelay() << "   " << KilledEnemy->Lifetime() << endl;
		}
		MyFile << "----------------------------------------------------" << endl << endl;

		MyFile << "Castle Total Damage       = " << BCastle.GetOriginalHealth() - BCastle.GetHealth() << "\n" << endl;
		MyFile << "Total Enemies             = " << KilledCount << "\n";
		MyFile << "Average First-Shot Delay  = " << FSD / NoOfKilled << "\n";
		MyFile << "Average Kill Delay        = " << KD / NoOfKilled << "\n";

	}

	MyFile.close();
}
//arwa
void Battle::FightersToKill()
{
	Queue<Fighter*>temp;
	Fighter* fighter;
	while (!Active_Fighter.isEmpty())
	{
		Active_Fighter.dequeue(fighter);
		int priority = fighter->CalculatePriority();
		temp.enqueue(fighter);
		
		ToKill_Fighters.enqueue(fighter, priority);
	}

}

void Battle::HealingEnemies()
{
	Fighter* fighter;
	Healer* healer;
	Freezer* freezer;
	Queue<Healer*> temp_heal;
	Queue<Fighter*> temp_fight;
	Queue<Freezer*> temp_freeze;
	int enemyDistance;
	int healerDistance;
	int distanceDifference;

	while (!Active_Healer.isEmpty())
	{
		Active_Healer.pop(healer);
		healerDistance = healer->GetDistance();
		temp_heal.enqueue(healer);
		//healer heals only when enemy->GetHealth() < 0.75 enemy->GetOriginalHealth()
		while (!Active_Fighter.isEmpty())
		{
			Active_Fighter.dequeue(fighter);
			enemyDistance = fighter->GetDistance();
			distanceDifference = abs(healerDistance - enemyDistance);
			if (distanceDifference <= 2)
				healer->Action(fighter);
			temp_fight.enqueue(fighter);
		}
		//healer heals only when enemy->GetHealth() < 0.75* enemy->GetOriginalHealth()
		while (!Active_Freezer.isEmpty())
		{
			Active_Freezer.dequeue(freezer);
			enemyDistance = freezer->GetDistance();
			distanceDifference = abs(healerDistance - enemyDistance);
			if (distanceDifference <= 2)
				healer->Action(freezer);
			temp_freeze.enqueue(freezer);
		}
		while (!temp_fight.isEmpty())
		{
			temp_fight.dequeue(fighter);
			Active_Fighter.enqueue(fighter);
		}
		while (!temp_freeze.isEmpty())
		{
			temp_freeze.dequeue(freezer);
			Active_Freezer.enqueue(freezer);
		}
	}
	while (!temp_heal.isEmpty())
	{
		temp_heal.dequeue(healer);
		Active_Healer.push(healer);
	}
}

//Noha
void Battle::CastleAttackFire(int Time) {
	int countAttacked = 0;
	Queue<Enemy*> AttackedEnemies;
	//while condition add that fighter priority queue is not null
	while (countAttacked < BCastle.GetnoOfAttackedEnemies()&& !ToKill_Fighters.isEmpty()) {
		//1)declare pointer of fighter
		Fighter* FT;
		//2)dequeue from fighter priority queue
		ToKill_Fighters.dequeue(FT);
		if (FT->GetOriginalHealth() == FT->GetHealth())
		{
			FT->SetFirstTime(Time);
		}
		//3)assign  the value of priority queue to the declared pointer
		//4)call decHealth through the pointer
		FT->DecHealth(BCastle.GetPower());
		//5)countAttacked++
		countAttacked++;
		//6)if(Health<=0)  enqueue pointer to killed enemies
		//ask?? enqueue the pointer to  (newCreated Attacked queue)
		if (FT->GetHealth() <= 0) {
			FT->SetDeathTime(Time);
			Killed.enqueue(FT);
			KilledCount++;
			EnemyCount--;
		}
		else {
			AttackedEnemies.enqueue(FT);
		}
	}
	while (countAttacked < BCastle.GetnoOfAttackedEnemies() && !Active_Healer.isEmpty()) {
		//1)declare pointer of healer
		Healer* HR;
		//2)dequeue from healer stack
		Active_Healer.pop(HR);
		//3)assign  the value of priority queue to the declared pointer
		if (HR->GetOriginalHealth() == HR->GetHealth())
		{
			HR->SetFirstTime(Time);
		}
		//4)call decHealth through the pointer
		HR->DecHealth(BCastle.GetPower());
		//5)countAttacked++
		countAttacked++;
		//6)if(Health<=0)  enqueue pointer to killed enemies
		//ask?? enqueue the pointer to  (newCreated Attacked queue)
		if (HR->GetHealth() <= 0) {
			HR->SetDeathTime(Time);
			Killed.enqueue(HR);
			KilledCount++;
			EnemyCount--;
		}
		else {
			AttackedEnemies.enqueue(HR);
		}
	}
	while (countAttacked < BCastle.GetnoOfAttackedEnemies() && !Active_Freezer.isEmpty()) {
		//1)declare pointer of freezer
		Freezer* FR;
		//2)dequeue from freezer priority queue
		Active_Freezer.dequeue(FR);

		//3)assign  the value of priority queue to the declared pointer
		if (FR->GetOriginalHealth() == FR->GetHealth())
		{
			FR->SetFirstTime(Time);
		}
		//4)call decHealth through the pointer
		FR->DecHealth(BCastle.GetPower());
		//5)countAttacked++
		countAttacked++;
		//6)if(Health<=0)  enqueue pointer to killed enemies
		//ask?? enqueue the pointer to  (newCreated Attacked queue)
		if (FR->GetHealth() <= 0) {
			FR->SetDeathTime(Time);
			Killed.enqueue(FR);
			KilledCount++;
			EnemyCount--;
		}
		else {
			AttackedEnemies.enqueue(FR);
		}
	}
	Enemy* E;
	Fighter* F;
	while (!AttackedEnemies.isEmpty()) {
		AttackedEnemies.dequeue(E);
		if (E->GetType() == 0) {
			Active_Fighter.enqueue(dynamic_cast<Fighter*>(E));
		}
		else if (E->GetType() == 1) {
			Healer* healer = dynamic_cast<Healer*>(E);
			Active_Healer.push(healer);
		}
		else if (E->GetType() == 2) {
        	Active_Freezer.enqueue(dynamic_cast<Freezer*>(E));
		}
	}
	while (!ToKill_Fighters.isEmpty()) {
		ToKill_Fighters.dequeue(F);
		Active_Fighter.enqueue(F);
	}
}

void Battle::CastleAttackIce()
{
	int countAttacked = 0;
	Queue<Enemy*> FrozenEnemies;
	//while condition add that fighter priority queue is not null
	while (countAttacked < BCastle.GetnoOfAttackedEnemies() && !ToKill_Fighters.isEmpty()) {
		//1)declare pointer of fighter
		Fighter* FT;
		//2)dequeue from fighter priority queue
		ToKill_Fighters.dequeue(FT);
		//3)assign  the value of priority queue to the declared pointer
		//4)call Freeze through the pointer
		FT->Freeze(BCastle.GetPower(),CurrentTimeStep);
		//5)countAttacked++
		countAttacked++;
		//6) enqueue the pointer to  (newCreated Frozen queue)
			FrozenEnemies.enqueue(FT);
		
	}
	while (countAttacked < BCastle.GetnoOfAttackedEnemies() && !Active_Healer.isEmpty()) {
		//1)declare pointer of healer
		Healer* HR;
		//2)dequeue from healer stack
		Active_Healer.pop(HR);
		//3)assign  the value of priority queue to the declared pointer
		//4)call decHealth through the pointer
		HR->Freeze(BCastle.GetPower(),CurrentTimeStep);
		//5)countAttacked++
		countAttacked++;
		//6)if(Health<=0)  enqueue pointer to killed enemies
		//ask?? enqueue the pointer to  (newCreated Attacked queue)
			FrozenEnemies.enqueue(HR);
	}
	while (countAttacked < BCastle.GetnoOfAttackedEnemies() && !Active_Freezer.isEmpty()) {
		//1)declare pointer of freezer
		Freezer* FR;
		//2)dequeue from freezer priority queue
		Active_Freezer.dequeue(FR);

		//3)assign  the value of priority queue to the declared pointer
		//4)call decHealth through the pointer
		FR->Freeze(BCastle.GetPower(),CurrentTimeStep);
		//5)countAttacked++
		countAttacked++;
		//6)if(Health<=0)  enqueue pointer to killed enemies
		//ask?? enqueue the pointer to  (newCreated Attacked queue)
			FrozenEnemies.enqueue(FR);
	}
	Enemy* E;
	Fighter* F;
	while (!FrozenEnemies.isEmpty()) {
		FrozenEnemies.dequeue(E);
		if (E->GetType() == 0) {
			Frozen.enqueue(dynamic_cast<Fighter*>(E));
		}
		else if (E->GetType() == 1) {
			Frozen.enqueue(dynamic_cast<Healer*>(E));
		}
		else if (E->GetType() == 2) {
			Frozen.enqueue(dynamic_cast<Freezer*>(E));
		}
	}
}



Battle::Battle()
{
	EnemyCount = 0;
	KilledCount = 0;
	ActiveCount = 0;
	FrostedCount = 0;
	DemoListCount = 0;
	CurrentTimeStep = 0;
	pGUI = NULL;
	CastleDamage = 0;
}

void Battle::AddtoDemoList(Enemy* Ptr)
{
	DemoList[DemoListCount++] = Ptr;

	// Note that this function doesn't allocate any enemy objects
	// It just enqueue a pointer that is already allocated into the queue
}


 Castle* Battle::GetCastle()
{
	 return &BCastle;
}


void Battle::RunSimulation()
{
	pGUI = new GUI;
	PROG_MODE	mode = pGUI->getGUIMode();

	switch (mode)	//Add a function for each mode in next phases
	{
	case MODE_INTR:
		GameLogicInterActiveMode();
		break;
	case MODE_STEP:
		GameLogicStepMode();
		break;
		
	case MODE_SLNT:
		GameLogicSilentMode();
		break;
	/*case MODE_DEMO:
		Just_A_Demo();
		*/
	}

	delete pGUI;

}


//This is just a demo function for project introductory phase
//It should be removed in phases 1&2
//void Battle::Just_A_Demo()
//{
//
//	pGUI->PrintMessage("Just a Demo. Enter Enemies Count(next phases should read I/P filename):");
//	EnemyCount = atoi(pGUI->GetString().c_str());	//get user input as a string then convert to integer
//
//	pGUI->PrintMessage("Generating Enemies randomly... In next phases, Enemies should be loaded from a file...CLICK to continue");
//	pGUI->waitForClick();
//
//	CurrentTimeStep = 0;
//	//
//	// THIS IS JUST A DEMO Function
//	// IT SHOULD BE REMOVED IN PHASE 1 AND PHASE 2
//	//
//
//	srand(time(NULL));
//	int Enemy_id = 0;
//	int ArrivalTime = 1;
//	Enemy* pE = NULL;
//	//Create Random enemies and add them all to inactive queue
//	for (int i = 0; i < EnemyCount; i++)
//	{
//		ArrivalTime += (rand() % 3);	//Randomize arrival time
//		pE = new Enemy(++Enemy_id, ArrivalTime, 5); //hadba men noha
//		pE->SetStatus(INAC); //initiall all enemies are inactive
//		Q_Inactive.enqueue(pE);		//Add created enemy to inactive Queue
//	}
//
//	AddAllListsToDrawingList();
//	pGUI->UpdateInterface(CurrentTimeStep);	//upadte interface to show the initial case where all enemies are still inactive
//
//	pGUI->waitForClick();
//
//	while (KilledCount < EnemyCount)	//as long as some enemies are alive (should be updated in next phases)
//	{
//		CurrentTimeStep++;
//		ActivateEnemies();
//
//		Demo_UpdateEnemies();	//Randomly update enemies distance/status (for demo purposes only)
//
//		pGUI->ResetDrawingList();
//		AddAllListsToDrawingList();
//		pGUI->UpdateInterface(CurrentTimeStep);
//	Sleep(250);
//	}
//}

//Add enemy lists (inactive, active,.....) to drawing list to be displayed on user interface
//menna
void Battle::AddAllListsToDrawingList()
{
	//Add inactive queue to drawing list
	int InactiveCount;
	Enemy* const* EnemyList = Q_Inactive.toArray(InactiveCount);
	for (int i = 0; i < InactiveCount; i++)
		pGUI->AddToDrawingList(EnemyList[i]);
	int FighterCount;
	Fighter* const* fighterList = Active_Fighter.toArray(FighterCount);
	for (int i = 0; i < FighterCount; i++)
		pGUI->AddToDrawingList(fighterList[i]);
	int FreezerCount;
	Freezer* const* FreezerList = Active_Freezer.toArray(FreezerCount);
	for (int i = 0; i < FreezerCount; i++)
		pGUI->AddToDrawingList(FreezerList[i]);
	int KilledCount;
	Enemy* const* KilledList = Killed.toArray(KilledCount);
	for (int i = 0; i < KilledCount; i++)
		pGUI->AddToDrawingList(KilledList[i]);
	int FrozenCount;
	Enemy* const* FrozenList = Frozen.toArray(FrozenCount);
	for (int i = 0; i < FrozenCount; i++)
		pGUI->AddToDrawingList(FrozenList[i]);
	int HealerCount ;
	Healer* const* HealerList = Active_Healer.toArray(HealerCount);
	for (int i = 0; i < HealerCount; i++)
		pGUI->AddToDrawingList(HealerList[i]);

	//Add other lists to drawing list
	//TO DO
	//In next phases, you should add enemies from different lists to the drawing list
	//For the sake of demo, we will use DemoList
	//for (int i = 0; i < DemoListCount; i++)
		//pGUI->AddToDrawingList(DemoList[i]);
}


//noha
//check the inactive list and activate all enemies that has arrived
void Battle::ActivateEnemies()
{
	Enemy* pE;
	Queue<Enemy*> temp_queue;
	while (!Q_Inactive.isEmpty())	//as long as there are more inactive enemies
	{
		Q_Inactive.dequeue(pE);	//remove enemy from the queue
		if (pE->GetArrvTime() > CurrentTimeStep)	//no more arrivals at current time
		{
			temp_queue.enqueue(pE);
			
		}
		if (pE->GetArrvTime() == CurrentTimeStep)
		{
			pE->SetStatus(ACTV);

			if (pE->GetType() == 0) {
				Active_Fighter.enqueue(dynamic_cast<Fighter*>(pE));
			}
			else if (pE->GetType() == 1) {
				Active_Healer.push(dynamic_cast<Healer*>(pE));
			}
			else if (pE->GetType() == 2) {
				Active_Freezer.enqueue(dynamic_cast<Freezer*>(pE));
			}
			//pE->SetStatus(ACTV);	//make status active
			//AddtoDemoList(pE);		//move it to demo list (for demo purposes)
		}
	
	}
	while (!temp_queue.isEmpty())
	{
		temp_queue.dequeue(pE);
		Q_Inactive.enqueue(pE);
	}
}


//Randomly update enemies distance/status (for demo purposes)
void Battle::Demo_UpdateEnemies()
{
	Enemy* pE;
	int Prop;
	//Freeze, activate, and kill propablities (for sake of demo)
	int FreezProp = 5, ActvProp = 10, KillProp = 1;
	srand(time(0));
	for (int i = 0; i < DemoListCount; i++)
	{
		pE = DemoList[i];
		switch (pE->GetStatus())
		{
		case ACTV:
			pE->DecrementDist();	//move the enemy towards the castle
			Prop = rand() % 100;
			if (Prop < FreezProp)		//with Freeze propablity, change some active enemies to be frosted
			{
				pE->SetStatus(FRST);
				ActiveCount--;
				FrostedCount++;
			}
			else	if (Prop < (FreezProp + KillProp))	//with kill propablity, kill some active enemies
			{
				pE->SetStatus(KILD);
				ActiveCount--;
				KilledCount++;
			}

			break;
		case FRST:
			Prop = rand() % 100;
			if (Prop < ActvProp)			//with activation propablity, change restore some frosted enemies to be active again
			{
				pE->SetStatus(ACTV);
				ActiveCount++;
				FrostedCount--;
			}

			else	if (Prop < (ActvProp + KillProp))			//with kill propablity, kill some frosted enemies
			{
				pE->SetStatus(KILD);
				FrostedCount--;
				KilledCount++;
			}

			break;
		}
	}
}

void Battle::EnemiesMoveAction() {
	Fighter* F;
	while (!Active_Fighter.isEmpty()) {
		Active_Fighter.dequeue(F);
		F->Move();
		F->Action(&BCastle);
		Print_Fighter.enqueue(F);
	}
	while (!Print_Fighter.isEmpty()) {
		Print_Fighter.dequeue(F);
		Active_Fighter.enqueue(F);
	}
	Freezer* freezer;
	while (!Active_Freezer.isEmpty()) {
		Active_Freezer.dequeue(freezer);
		freezer->Move();
		freezer->Action(&BCastle);
		Print_Freezer.enqueue(freezer);
	}
	while (!Print_Freezer.isEmpty()) {
		Print_Freezer.dequeue(freezer);

		Active_Freezer.enqueue(freezer);
	}
	Healer* healer;
	while (!Active_Healer.isEmpty()) {
		Active_Healer.pop(healer);
		healer->Move();
		Print_Healer.enqueue(healer);
	}
	while (!Print_Healer.isEmpty()) {
		Print_Healer.dequeue(healer);
		Active_Healer.push(healer);
	}
}

//Noha
void Battle::GameLogicInterActiveMode() {
	CurrentTimeStep = 0;
	int frozenFighter, frozenFreezer, frozenHealer, KilledFighter, KilledFreezer, KilledHealer, FighterCount, FreezerCount, HealerCount;
//	int ice = rand() % 350 + 1;
	InputFunction("input.txt");
	AddAllListsToDrawingList();
	CountOfActiveEnemies(FighterCount, FreezerCount, HealerCount);

	CountOfEnemiesFrozenKilled(frozenFighter, frozenFreezer, frozenHealer, KilledFighter, KilledFreezer, KilledHealer);

	pGUI->UpdateInterface(CurrentTimeStep,BCastle.GetHealth(),BCastle.GetStatus(),FighterCount,FreezerCount,HealerCount,frozenFighter,frozenFreezer,frozenHealer,KilledFighter,KilledFreezer,KilledHealer);
	pGUI->waitForClick();
	CurrentTimeStep++;
	CountOfActiveEnemies(FighterCount, FreezerCount, HealerCount);

	CountOfEnemiesFrozenKilled(frozenFighter, frozenFreezer, frozenHealer, KilledFighter, KilledFreezer, KilledHealer);

	pGUI->UpdateInterface(CurrentTimeStep, BCastle.GetHealth(), BCastle.GetStatus(), FighterCount, FreezerCount, HealerCount, frozenFighter, frozenFreezer, frozenHealer, KilledFighter, KilledFreezer, KilledHealer);
	while (KilledCount < EnemyCount && BCastle.GetStatus() == ACTV) {

		ActivateEnemies();
	//	pGUI->waitForClick();
		pGUI->ResetDrawingList();
		AddAllListsToDrawingList();
		EnemiesMoveAction();
	//	pGUI->waitForClick();
		pGUI->ResetDrawingList();
		AddAllListsToDrawingList();
		FightersToKill();
		
		
		//for (int i = 0; i < BCastle.GetnoOfAttackedEnemies(); i++) {
		//	if (rand() % 5==0)
		//		CastleAttackIce();
			
			CastleAttackFire(CurrentTimeStep); HealingEnemies();
			
				
		

		//AddAllListsToDrawingList();
		pGUI->ResetDrawingList();
		AddAllListsToDrawingList();
		CountOfActiveEnemies(FighterCount, FreezerCount, HealerCount);
		CountOfEnemiesFrozenKilled(frozenFighter, frozenFreezer, frozenHealer, KilledFighter, KilledFreezer, KilledHealer);
		pGUI->UpdateInterface(CurrentTimeStep, BCastle.GetHealth(), BCastle.GetStatus(), FighterCount, FreezerCount, HealerCount, frozenFighter, frozenFreezer, frozenHealer, KilledFighter, KilledFreezer, KilledHealer);
		pGUI->waitForClick();
		FrozenActive();
		CurrentTimeStep++;
		
	}
	Outputfunction();
}


void Battle::GameLogicStepMode() {
	CurrentTimeStep = 0;
	int frozenFighter, frozenFreezer, frozenHealer, KilledFighter, KilledFreezer, KilledHealer, FighterCount, FreezerCount, HealerCount;

	//int ice = rand() % 350 + 1;
	InputFunction("InputFiles(2).txt");
	AddAllListsToDrawingList();
	CountOfActiveEnemies(FighterCount, FreezerCount, HealerCount);

	CountOfEnemiesFrozenKilled(frozenFighter, frozenFreezer, frozenHealer, KilledFighter, KilledFreezer, KilledHealer);
	pGUI->UpdateInterface(CurrentTimeStep, BCastle.GetHealth(), BCastle.GetStatus(), FighterCount, FreezerCount, HealerCount, frozenFighter, frozenFreezer, frozenHealer, KilledFighter, KilledFreezer, KilledHealer);
	Sleep(1000);
	CurrentTimeStep++;
	CountOfActiveEnemies(FighterCount, FreezerCount, HealerCount);
	CountOfEnemiesFrozenKilled(frozenFighter, frozenFreezer, frozenHealer, KilledFighter, KilledFreezer, KilledHealer);

	pGUI->UpdateInterface(CurrentTimeStep, BCastle.GetHealth(), BCastle.GetStatus(), FighterCount, FreezerCount, HealerCount, frozenFighter, frozenFreezer, frozenHealer, KilledFighter, KilledFreezer, KilledHealer);
	while (KilledCount < EnemyCount && BCastle.GetStatus() == ACTV) {

		ActivateEnemies();
		pGUI->ResetDrawingList();
		AddAllListsToDrawingList();
		EnemiesMoveAction();
		pGUI->ResetDrawingList();
		AddAllListsToDrawingList();
		FightersToKill();


		for (int i = 0; i < BCastle.GetnoOfAttackedEnemies(); i++) {
			if (rand() % 5 == 0)
				CastleAttackIce();

			else {
				CastleAttackFire(CurrentTimeStep);
				HealingEnemies();
			}

		}

		//AddAllListsToDrawingList();
		pGUI->ResetDrawingList();
		AddAllListsToDrawingList();
		CountOfActiveEnemies(FighterCount, FreezerCount, HealerCount);
		CountOfEnemiesFrozenKilled(frozenFighter, frozenFreezer, frozenHealer, KilledFighter, KilledFreezer, KilledHealer);
		pGUI->UpdateInterface(CurrentTimeStep, BCastle.GetHealth(), BCastle.GetStatus(), FighterCount, FreezerCount, HealerCount, frozenFighter, frozenFreezer, frozenHealer, KilledFighter, KilledFreezer, KilledHealer);
		Sleep(1000);
		FrozenActive();
		CurrentTimeStep++;

	}
	Outputfunction();
}
//	
//
void Battle::GameLogicSilentMode() {
	CurrentTimeStep = 0;
	//int frozenFighter, frozenFreezer, frozenHealer, KilledFighter, KilledFreezer, KilledHealer, FighterCount, FreezerCount, HealerCount;

	//int ice = rand() % 350 + 1;
	InputFunction("InputFiles(2).txt");

//	CountOfActiveEnemies(FighterCount, FreezerCount, HealerCount);

	//CountOfEnemiesFrozenKilled(frozenFighter, frozenFreezer, frozenHealer, KilledFighter, KilledFreezer, KilledHealer);

	//pGUI->UpdateInterface(CurrentTimeStep, BCastle.GetHealth(), BCastle.GetStatus(), FighterCount, FreezerCount, HealerCount, frozenFighter, frozenFreezer, frozenHealer, KilledFighter, KilledFreezer, KilledHealer);
	CurrentTimeStep++;
//	pGUI->UpdateInterface(CurrentTimeStep);
	while (KilledCount < EnemyCount && BCastle.GetStatus() == ACTV) {
        ActivateEnemies();
		EnemiesMoveAction();
    	FightersToKill();


		for (int i = 0; i < BCastle.GetnoOfAttackedEnemies(); i++) {
					if (rand() % 5==0)
						CastleAttackIce();

					else {
						CastleAttackFire(CurrentTimeStep);
						HealingEnemies();
					}

		}

				
				//pGUI->UpdateInterface(CurrentTimeStep, BCastle.GetHealth(), BCastle.GetStatus(), FighterCount, FreezerCount, HealerCount, frozenFighter, frozenFreezer, frozenHealer, KilledFighter, KilledFreezer, KilledHealer);
		FrozenActive();
		CurrentTimeStep++;

	}
	Outputfunction();
}
void Battle::CountOfEnemiesFrozenKilled(int& frez_fighters, int& frez_freezer, int& frez_healer, int& kld_fighters, int& kld_freezer, int& kld_healer) {
	Queue<Enemy*> temp_Frozen;
	Enemy* E;
	frez_fighters = 0;
	frez_freezer = 0;
	frez_healer = 0;
	while (!Frozen.isEmpty()) {
		Frozen.dequeue(E);
		temp_Frozen.enqueue(E);
		if (E->GetType() == 0) {
			frez_fighters++;
		}
		else if (E->GetType() == 1) {
			frez_freezer++;
		}
		else if (E->GetType() == 1) {
			frez_healer++;
		}

	}
	while (!temp_Frozen.isEmpty()) {
		temp_Frozen.dequeue(E);
		Frozen.enqueue(E);
	}

	Queue<Enemy*> temp_Killed;
	//Enemy* E;
	kld_fighters = 0;
	kld_freezer = 0;
	kld_healer = 0;
	while (!Killed.isEmpty()) {
		Killed.dequeue(E);
		temp_Killed.enqueue(E);
		if (E->GetType() == 0) {
			kld_fighters++;
		}
		else if (E->GetType() == 1) {
			kld_freezer++;
		}
		else if (E->GetType() == 1) {
			kld_healer++;
		}

	}
	while (!temp_Killed.isEmpty()) {
		temp_Killed.dequeue(E);
		Killed.enqueue(E);
	}

}


void  Battle::CountOfActiveEnemies(int& actv_fighters, int& actv_freezer, int& actv_healer) {
	actv_fighters = 0; actv_freezer = 0;  actv_healer = 0;
	Fighter* const* fighterList = Active_Fighter.toArray(actv_fighters);

	Freezer* const* FreezerList = Active_Freezer.toArray(actv_freezer);

	Healer* const* HealerList = Active_Healer.toArray(actv_healer);
}
void Battle::FrozenActive() {
	Enemy* E;
	//bool EnemyFrozen=E->isFrozen(CurrentTimeStep);
	Queue<Enemy*>temp;
	while (!Frozen.isEmpty()) {
		Frozen.dequeue(E);
		bool EnemyFrozen = E->isFrozen(CurrentTimeStep);
		if (EnemyFrozen == false) {
			if (E->GetType() == 0) {
				Active_Fighter.enqueue(dynamic_cast<Fighter*>(E));
			}
			else if (E->GetType() == 1) {
				Active_Healer.push(dynamic_cast<Healer*>(E));
			}
			else if (E->GetType() == 2) {
				Active_Freezer.enqueue(dynamic_cast<Freezer*>(E));
			}
		}
		else {
			temp.enqueue(E);
		}
	}
	while (!temp.isEmpty()) {
		temp.dequeue(E);
		Frozen.enqueue(E);
	}

}