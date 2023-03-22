#include "GUI.h"
#include <time.h>
#include <cstdlib>
#include "../Fighter.h"
#include <string>

//////////////////////////////////////////////////////////////////////////////////////////
GUI::GUI()
{
	DrawingItemsCount = 0;
	pWind = new window(WindWidth+15,WindHeight,0,0); 
	pWind->ChangeTitle("The Castle Battle");

	BackgroundClr = KHAKI;	//Background color
	StatusBarClr = WHITE;	//Status bar color

	//Set color for each enemy type
	DrawingColors[FIGHTER] =  BLUE;		//fighter-enemy color
	DrawingColors[FREEZER] = ORANGE;		//Frosted-enemy color
	DrawingColors[HEALER] =  RED;		//healer-enemy color	

	RegionsLabels[INAC] = "INAC";
	RegionsLabels[ACTV] = "ACTV";
	RegionsLabels[FRST] = "FRST";
	RegionsLabels[KILD] = "KILD";

	ClearStatusBar();
	ClearDrawingArea(); 
	DrawCastleArea();  
}
//////////////////////////////////////////////////////////////////////////////////////////
GUI::~GUI()
{
	delete pWind;
}

//////////////////////////////////////////////////////////////////////////////////////////
// ================================== INPUT FUNCTIONS ====================================
//////////////////////////////////////////////////////////////////////////////////////////

void GUI::waitForClick() const
{
	int x,y;
	pWind->WaitMouseClick(x, y);	//Wait for mouse click
}
//////////////////////////////////////////////////////////////////////////////////////////
string GUI::GetString() const 
{
	string Label;
	char Key;
	while(1)
	{
		pWind->WaitKeyPress(Key);
		if(Key == 27 )	//ESCAPE key is pressed
			return "";	//returns nothing as user has cancelled label
		if(Key == 13 )	//ENTER key is pressed
			return Label;
		if((Key == 8) && (Label.size() >= 1))	//BackSpace is pressed
			Label.resize(Label.size() -1 );			
		else
			Label += Key;
		
		PrintMessage(Label);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// ================================== OUTPUT FUNCTIONS ===================================
//////////////////////////////////////////////////////////////////////////////////////////

void GUI::PrintMessage(string msg,string msg2,string msg3,string msg4,string msg5) const	//Prints a message on status bar
{
	ClearStatusBar();	//First clear the status bar
	
	pWind->SetPen(DARKRED);
	pWind->SetFont(18, BOLD , BY_NAME, "Arial");   
	pWind->DrawString(10, WindHeight - (int) (StatusBarHeight/1.2), msg); // You may need to change these coordinates later 
	pWind->DrawString(10, WindHeight - (int)(StatusBarHeight / 1.5), msg2);             
	pWind->DrawString(10, WindHeight - (int)(StatusBarHeight / 2), msg3);// to be able to write multi-line
	pWind->DrawString(10, WindHeight - (int)(StatusBarHeight / 2.5), msg4);
	pWind->DrawString(10, WindHeight - (int)(StatusBarHeight / 3.3), msg5);
}
void GUI::PrintMessage(string msg) const
{
	ClearStatusBar();	//First clear the status bar

	pWind->SetPen(DARKRED);
	pWind->SetFont(18, BOLD, BY_NAME, "Arial");
	pWind->DrawString(10, WindHeight - (int)(StatusBarHeight / 1.5) , msg); // You may need to change these coordinates later 

}
//////////////////////////////////////////////////////////////////////////////////////////
void GUI::DrawString(const int iX, const int iY, const string Text)
{
	pWind->SetPen(DARKRED);
	pWind->SetFont(18, BOLD , BY_NAME, "Arial");   
	pWind->DrawString(iX, iY, Text);
}

//////////////////////////////////////////////////////////////////////////////////////////
void GUI::ClearStatusBar() const
{
	pWind->SetPen(StatusBarClr, 3);
	pWind->SetBrush(StatusBarClr);
	pWind->DrawRectangle(0, WindHeight - StatusBarHeight , WindWidth, WindHeight);	

	pWind->SetPen(BROWN, 3);
	pWind->DrawLine(0, WindHeight - StatusBarHeight , WindWidth, WindHeight - StatusBarHeight);	
}
///////////////////////////////////////////////////////////////////////////////////
void GUI::ClearDrawingArea() const
{
	// Clearing the Drawing area
	pWind->SetPen(BackgroundClr, 3);
	pWind->SetBrush(BackgroundClr);
	pWind->DrawRectangle(0, MenuBarHeight, WindWidth, WindHeight - StatusBarHeight);
}
///////////////////////////////////////////////////////////////////////////////////
void GUI::DrawCastleArea() const
{
	int L = CastleWidth / 2;

	// 1- Drawing the brown square of the Castle
	pWind->SetPen(BROWN);
	pWind->SetBrush(BROWN);
	pWind->DrawRectangle(CastleStartX, CastleStartY, CastleEndX, CastleEndY);

	// 2- Drawing the 2 brown crossed lines (for making 4 regions)
	pWind->SetPen(BROWN, 3);
	pWind->DrawLine(0, YHalfDrawingArea, WindWidth, YHalfDrawingArea);
	pWind->DrawLine(WindWidth/2, MenuBarHeight, WindWidth/2, WindHeight-StatusBarHeight);

	// 3- Drawing the 2 white crossed lines (inside the Castle)
	pWind->SetPen(WHITE);
	pWind->DrawLine(WindWidth/2, YHalfDrawingArea - CastleWidth/2, WindWidth/2, YHalfDrawingArea + CastleWidth/2);
	pWind->DrawLine(WindWidth/2 - CastleWidth/2, YHalfDrawingArea, WindWidth/2 + CastleWidth/2, YHalfDrawingArea);

	// 5- Writing regions labels
	pWind->SetPen(WHITE);
	pWind->SetFont(20, BOLD , BY_NAME, "Arial");
	pWind->DrawString(CastleStartX + (int)(0.1*L), CastleStartY + 5*L/12, RegionsLabels[INAC]);

	pWind->DrawString(WindWidth/2 + (int)(0.1*L), CastleStartY + 5*L/12, RegionsLabels[ACTV]);

	pWind->DrawString(WindWidth/2 + (int)(0.1*L), YHalfDrawingArea + 5*L/12, RegionsLabels[FRST]); 

	pWind->DrawString(CastleStartX + (int)(0.1*L), YHalfDrawingArea + 5*L/12, RegionsLabels[KILD]);
}
//////////////////////////////////////////////////////////////////////////////////////////
//Draws the passed item in its region
//region count in the numbers of items drawn so far in that item's region
void GUI::DrawSingleItem(const DrawingItem* pDitem, int RegionCount) const       // It is a private function
{

	if (RegionCount > MaxRegionEnemyCount) 
		return; //no more items can be drawn in this region

	int DrawDistance = RegionCount;
	int YPos = 1;
	if(RegionCount>=MaxHorizEnemies )	//max no. of enemies to draw in one line
	{
		DrawDistance = (RegionCount-1)%MaxHorizEnemies + 1;
		YPos = (RegionCount-1) / MaxHorizEnemies + 1; 
	}

	GUI_REGION Region = pDitem->region;

	color pixelClr = BackgroundClr;
	int VertCount;	//Verical enemy count at same distance

	int x, y, refX, refY;
	//First calculate x,y position of the enemy on the output screen
	//It depends on the region and the enemy distance
	switch (Region)
	{
	case INAC_REG:
		refX = (WindWidth/2 - CastleWidth/2);
		refY = YHalfDrawingArea - EnemyHeight; //
		x = refX - DrawDistance*EnemyWidth - DrawDistance; //(Distance)
		y = refY - YPos*EnemyHeight - YPos; // YPos
		break;
	case ACTV_REG:		
		DrawDistance = pDitem->distance;
		refX = (WindWidth/2 + CastleWidth/2);
		refY = YHalfDrawingArea - EnemyWidth - 1 ; //
		x = refX + (DrawDistance-1)*EnemyWidth + DrawDistance; //(Distance)
		y = refY - YPos*EnemyHeight - YPos; // YPos
		
		
		pixelClr = pWind->GetColor(x+1, y+1);
		VertCount = 0;
		while (pixelClr != BackgroundClr && VertCount<MaxVerticalEnemies) 		
		{ 
			y -= (EnemyHeight+1);
			VertCount++;
			pixelClr = pWind->GetColor(x+1, y+1);
		}
		if(VertCount == MaxVerticalEnemies)	//max enemies that can be drawn at same distance exceeded
			return;		//don't draw this enemy

		break;
	case FRST_REG:
		DrawDistance = pDitem->distance;
		refX = (WindWidth/2 + CastleWidth/2);
		refY = YHalfDrawingArea + EnemyHeight; //
		x = refX + (DrawDistance-1)*EnemyWidth + DrawDistance; //(Distance)
		y = refY + (YPos-1)*EnemyHeight + YPos; // YPos

		pixelClr = pWind->GetColor(x+1, y+1);
		VertCount = 0;
		while (pixelClr != BackgroundClr && VertCount<MaxVerticalEnemies) 		
		{ 
			y += (EnemyHeight+1);
			VertCount++;
			pixelClr = pWind->GetColor(x+1, y+1);
		}
		if(VertCount == MaxVerticalEnemies)	//max enemies that can be drawn at same distance exceeded
			return;		//don't draw this enemy

		break;
	case KILD_REG:
		refX = (WindWidth/2 - CastleWidth/2);
		refY = YHalfDrawingArea + EnemyHeight; //
		x = refX - DrawDistance*EnemyWidth - DrawDistance; //(Distance)
		y = refY + (YPos-1)*EnemyHeight + YPos; // YPos
		break;
	default:
		break;
	}

	// Drawing the item
	pWind->SetPen(pDitem->clr);
	pWind->SetBrush(pDitem->clr);
	pWind->DrawRectangle(x, y, x + EnemyWidth, y + EnemyHeight);
	
}


//////////////////////////////////////////////////////////////////////////////////////////
/* A function to draw all items in DrawingList and ensure there is no overflow in the drawing*/
void GUI::DrawAllItems() 
{
	//Prepare counter for each region
	int RegionsCounts[REG_CNT]={0};	//initlaize all counters to zero

	DrawingItem* pDitem;
	for(int i=0; i<DrawingItemsCount; i++)
	{
		pDitem = DrawingList[i];
		RegionsCounts[pDitem->region]++;
		DrawSingleItem(DrawingList[i],RegionsCounts[pDitem->region]);
	}
}

//prints info on status bar. 
//Info includes timestep and other enemies info as illustrated in project doc
void GUI::UpdateStatusBar(int CurrentTimeStep,double CastleHealth, ENMY_STATUS Castlestatus,int actv_fighters,int actv_freezer,int actv_healer, int frez_fighters, int frez_freezer, int frez_healer, int kld_fighters, int kld_freezer, int kld_healer)
{
	int total_actv = actv_fighters + actv_freezer + actv_healer;
	int total_frez= frez_fighters + frez_freezer + frez_healer;
	int total_kld= kld_fighters + kld_freezer + kld_healer;
	string status;
		//print current timestep
		/*char strTimestep[10];
		itoa(CurrentTimeStep,strTimestep,10);*/	
	string s1 = to_string(CurrentTimeStep);

		//PrintMessage(to_string(CurrentTimeStep),0);
		string health=to_string(CastleHealth);
		
	//PrintMessage("Castle Health: " + health,1);
		if (Castlestatus == ACTV)
			status = "Active";
		else if (Castlestatus == FRST)
			status = "Frozen";
		else if (Castlestatus == KILD)
			status = "Killed";
		string string2 = "Castle Health: " + health + "   Castle Status: " + status;
		string string3 = "Actv_fighter: " + to_string(actv_fighters) + "    Actv_healer: " + to_string(actv_healer) + "    Actv_freezer: " + to_string(actv_freezer) + "    Total: " + to_string(total_actv);
		string string4 = "Frez_fighter: " + to_string(frez_fighters) + "    Frez_healer: " + to_string(frez_healer) + "    Frez_freezer: " + to_string(frez_freezer) + "    Total: " + to_string(total_frez);
		string string5 = "Kild_fighter: " + to_string(kld_fighters) + "    Kild_healer: " + to_string(kld_healer) + "    Frez_freezer: " + to_string(kld_freezer) + "    Total: " + to_string(total_kld);
		PrintMessage(s1, string2, string3,string4,string5);
		//PrintMessage("Actv_fighter: " + to_string(actv_fighters) + "Actv_healer: " + to_string(actv_healer) + "Actv_freezer: " + to_string(actv_freezer) + "Total: " + to_string(total_actv));
		//PrintMessage("Frez_fighter: " + to_string(frez_fighters) + "Frez_healer: " + to_string(frez_healer) + "Frez_freezer: " + to_string(frez_freezer) + "Total: " + to_string(total_frez));
		//PrintMessage("Kild_fighter: " + to_string(kld_fighters) + "Kild_healer: " + to_string(kld_healer) + "Frez_freezer: " + to_string(kld_freezer) + "Total: " + to_string(total_kld));
}

void GUI::UpdateInterface(int CurrentTimeStep, double CastleHealth, ENMY_STATUS Castlestatus, int actv_fighters, int actv_freezer, int actv_healer, int frez_fighters, int frez_freezer, int frez_healer, int kld_fighters, int kld_freezer, int kld_healer)
{
	ClearDrawingArea();
	UpdateStatusBar(CurrentTimeStep, CastleHealth, Castlestatus, actv_fighters, actv_freezer, actv_healer,  frez_fighters, frez_freezer,  frez_healer,  kld_fighters,  kld_freezer,  kld_healer);
	DrawCastleArea();
	DrawAllItems();
}

/*
	AddOrderForDrawing: Adds a new item related to the passed Enemy to the drawing list
*/
//menna
void GUI::AddToDrawingList( Enemy* pE)
{
	DrawingItem *pDitem=new DrawingItem;
	pDitem->ID = pE->GetID();
	pDitem->distance = pE->GetDistance();
	pDitem->region= (GUI_REGION) (pE->GetStatus());	//map status to drawing region	
	Fighter* fighter = dynamic_cast<Fighter*>(pE);
	Healer* healer = dynamic_cast<Healer*>(pE);
	Freezer* freezer  = dynamic_cast<Freezer*>(pE);
	if (fighter)
		pDitem->type = 0;
	else if (healer)
		pDitem->type = 1;
	else if (freezer)
		pDitem->type = 2;

	// IMPORTANT [TO DO]
	// enemy type has been generated randomly here because enemy classes are not written yet
	// in next phases, to know enemy type, you should apply dynamic_cast to pE pointer
	int eType = pDitem->type;
	pDitem->clr = DrawingColors[eType];
	/////////////
	/////////////
	
	DrawingList[DrawingItemsCount++]=pDitem;	//add item to the list
}
//menna
/*void GUI::AddToDrawingListFighter(Fighter* pF)
{
	DrawingItem* pDitem = new DrawingItem;
	pDitem->ID = pF->GetID();
	pDitem->distance = pF->GetDistance();
	pDitem->region = (GUI_REGION)(pF->GetStatus());	

	int eType = pDitem->ID;
	pDitem->clr = DrawingColors[eType];
	
	DrawingList[DrawingItemsCount++] = pDitem;	
}
//menna
void GUI::AddToDrawingListHealer(Healer* pH)
{
	DrawingItem* pDitem = new DrawingItem;
	pDitem->ID = pH->GetID();
	pDitem->distance = pH->GetDistance();
	pDitem->region = (GUI_REGION)(pH->GetStatus());

	int eType = pDitem->ID;
	pDitem->clr = DrawingColors[eType];

	DrawingList[DrawingItemsCount++] = pDitem;
}
//menna
void GUI::AddToDrawingListFreezer(Freezer* pFr)
{
	DrawingItem* pDitem = new DrawingItem;
	pDitem->ID = pFr->GetID();
	pDitem->distance = pFr->GetDistance();
	pDitem->region = (GUI_REGION)(pFr->GetStatus());

	int eType = pDitem->ID;
	pDitem->clr = DrawingColors[eType];

	DrawingList[DrawingItemsCount++] = pDitem;
}*/
void GUI::ResetDrawingList()
{
	for(int i=0; i<DrawingItemsCount; i++)
		delete DrawingList[i];

	DrawingItemsCount = 0;
}

PROG_MODE	GUI::getGUIMode() const
{
	PROG_MODE Mode;
	do
	{
		PrintMessage("Please select GUI mode: (1)Interactive, (2)StepByStep, (3)Silent ");
		string S = GetString();
		Mode = (PROG_MODE) (atoi(S.c_str())-1);
	}
	while(Mode< 0 || Mode >= MODE_CNT);
	return Mode;
}
