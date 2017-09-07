//============================================================================
//----------------------------------------------------------------------------
//									Input.c
//----------------------------------------------------------------------------
//============================================================================


#include <ToolUtils.h>
#include "Externs.h"
#include "MainWindow.h"
#include "RectUtils.h"


#define kNormalThrust		5
#define kHyperThrust		8
#define kHeliumLift			4
#define kEscPausePictID		1015
#define kTabPausePictID		1016
#define	kSavingGameDial		1042


void LogDemoKey (char);
void DoCommandKey (void);
void DoPause (void);
void DoBatteryEngaged (gliderPtr);
void DoHeliumEngaged (gliderPtr);
Boolean QuerySaveGame (void);


demoPtr		demoData;
KeyMap		theKeys;
DialogPtr	saveDial;
short		demoIndex, batteryFrame;
Boolean		isEscPauseKey, paused, batteryWasEngaged;

extern	long		gameFrame;
extern	short		otherPlayerEscaped;
extern	Boolean		quitting, playing, onePlayerLeft, twoPlayerGame, demoGoing;


//==============================================================  Functions
//--------------------------------------------------------------  LogDemoKey

void LogDemoKey (char keyIs)
{
	demoData[demoIndex].frame = gameFrame;
	demoData[demoIndex].key = keyIs;
	demoIndex++;
}

//--------------------------------------------------------------  DoCommandKey

void DoCommandKey (void)
{	
	if (BitTst(&theKeys, kQKeyMap))
	{
		playing = false;
		paused = false;
		if ((!twoPlayerGame) && (!demoGoing))
		{
			if (QuerySaveGame())
				SaveGame2();		// New save game.
		}
	}
	else if ((BitTst(&theKeys, kSKeyMap)) && (!twoPlayerGame))
	{
		RefreshScoreboard(kSavingTitleMode);
		SaveGame2();				// New save game.
		HideCursor();
		CopyRectWorkToMain(&workSrcRect);
		RefreshScoreboard(kNormalTitleMode);
	}
}

//--------------------------------------------------------------  DoPause

void DoPause (void)
{
	Rect		bounds;
	
	SetPort((GrafPtr)mainWindow);
	QSetRect(&bounds, 0, 0, 214, 54);
	CenterRectInRect(&bounds, &houseRect);
	if (isEscPauseKey)
		LoadScaledGraphic(kEscPausePictID, &bounds);
	else
		LoadScaledGraphic(kTabPausePictID, &bounds);
	
	do
	{
		GetKeys(theKeys);
	}
	while ((isEscPauseKey && BitTst(&theKeys, kEscKeyMap)) || 
			(!isEscPauseKey && BitTst(&theKeys, kTabKeyMap)));
	
	paused = true;
	while (paused)
	{
		GetKeys(theKeys);
		if ((isEscPauseKey && BitTst(&theKeys, kEscKeyMap)) || 
				(!isEscPauseKey && BitTst(&theKeys, kTabKeyMap)))
			paused = false;
		else if (BitTst(&theKeys, kCommandKeyMap))
			DoCommandKey();
	}
	
	CopyBits((BitMap *)*GetGWorldPixMap(workSrcMap), 
			GetPortBitMapForCopyBits(GetWindowPort(mainWindow)), 
			&bounds, &bounds, srcCopy, nil);
	
	do
	{
		GetKeys(theKeys);
	}
	while ((isEscPauseKey && BitTst(&theKeys, kEscKeyMap)) || 
			(!isEscPauseKey && BitTst(&theKeys, kTabKeyMap)));
}

//--------------------------------------------------------------  DoBatteryEngaged

void DoBatteryEngaged (gliderPtr thisGlider)
{
	if (thisGlider->facing == kFaceLeft)
	{
		if (thisGlider->tipped)
			thisGlider->hVel += kHyperThrust;
		else
			thisGlider->hVel -= kHyperThrust;
	}
	else
	{
		if (thisGlider->tipped)
			thisGlider->hVel -= kHyperThrust;
		else
			thisGlider->hVel += kHyperThrust;
	}
	
	batteryTotal--;
	
	if (batteryTotal == 0)
	{
		QuickBatteryRefresh(false);
		PlayPrioritySound(kFizzleSound, kFizzlePriority);
	}
	else
	{
		if (!batteryWasEngaged)
			batteryFrame = 0;
		if (batteryFrame == 0)
			PlayPrioritySound(kThrustSound, kThrustPriority);
		batteryFrame++;
		if (batteryFrame >= 4)
			batteryFrame = 0;
		batteryWasEngaged = true;
	}
}

//--------------------------------------------------------------  DoHeliumEngaged

void DoHeliumEngaged (gliderPtr thisGlider)
{
	thisGlider->vDesiredVel = -kHeliumLift;
	batteryTotal++;
	
	if (batteryTotal == 0)
	{
		QuickBatteryRefresh(false);
		PlayPrioritySound(kFizzleSound, kFizzlePriority);
		batteryWasEngaged = false;
	}
	else
	{
		if (!batteryWasEngaged)
			batteryFrame = 0;
		if (batteryFrame == 0)
			PlayPrioritySound(kHissSound, kHissPriority);
		batteryFrame++;
		if (batteryFrame >= 4)
			batteryFrame = 0;
		batteryWasEngaged = true;
	}
}

//--------------------------------------------------------------  GetDemoInput

 void GetDemoInput (gliderPtr thisGlider)
 {
 	if (thisGlider->which == kPlayer1)
	{
		GetKeys(theKeys);
		
#if BUILD_ARCADE_VERSION
		
		if ((BitTst(&theKeys, thisGlider->leftKey)) || 
				(BitTst(&theKeys, thisGlider->rightKey)) || 
				(BitTst(&theKeys, thisGlider->battKey)) || 
				(BitTst(&theKeys, thisGlider->bandKey)))
		{
			playing = false;
			paused = false;
		}
		
#else
		
		if (BitTst(&theKeys, kCommandKeyMap))
			DoCommandKey();
		
#endif
	}
	
	if (thisGlider->mode == kGliderBurning)
	{
		if (thisGlider->facing == kFaceLeft)
			thisGlider->hDesiredVel -= kNormalThrust;
		else
			thisGlider->hDesiredVel += kNormalThrust;
	}
	else
	{
		thisGlider->heldLeft = false;
		thisGlider->heldRight = false;
		thisGlider->tipped = false;
		
	 	if (gameFrame == (long)demoData[demoIndex].frame)
	 	{
	 		switch (demoData[demoIndex].key)
	 		{
	 			case 0:		// left key
	 			thisGlider->hDesiredVel += kNormalThrust;
				thisGlider->tipped = (thisGlider->facing == kFaceLeft);
				thisGlider->heldRight = true;
				thisGlider->fireHeld = false;
	 			break;
	 			
	 			case 1:		// right key
	 			thisGlider->hDesiredVel -= kNormalThrust;
				thisGlider->tipped = (thisGlider->facing == kFaceRight);
				thisGlider->heldLeft = true;
				thisGlider->fireHeld = false;
	 			break;
	 			
	 			case 2:		// battery key
		 		if (batteryTotal > 0)
					DoBatteryEngaged(thisGlider);
				else
					DoHeliumEngaged(thisGlider);
	 			thisGlider->fireHeld = false;
	 			break;
	 			
	 			case 3:		// rubber band key
	 			if (!thisGlider->fireHeld)
				{
					if (AddBand(thisGlider, thisGlider->dest.left + 24, 
							thisGlider->dest.top + 10, thisGlider->facing))
					{
						bandsTotal--;
						if (bandsTotal <= 0)
							QuickBandsRefresh(false);
						
						thisGlider->fireHeld = true;
					}
				}
	 			break;
	 		}
	 		
	 		demoIndex++;
	 	}
	 	else
	 		thisGlider->fireHeld = false;
	 	
		if ((isEscPauseKey && BitTst(&theKeys, kEscKeyMap)) || 
				(!isEscPauseKey && BitTst(&theKeys, kTabKeyMap)))
		{
			DoPause();
		}
 	}
 }
 
//--------------------------------------------------------------  GetInput

void GetInput (gliderPtr thisGlider)
{
	if (thisGlider->which == kPlayer1)
	{
		GetKeys(theKeys);
		if (BitTst(&theKeys, kCommandKeyMap))
			DoCommandKey();
	}
	
	if (thisGlider->mode == kGliderBurning)
	{
		if (thisGlider->facing == kFaceLeft)
			thisGlider->hDesiredVel -= kNormalThrust;
		else
			thisGlider->hDesiredVel += kNormalThrust;
	}
	else
	{
		thisGlider->heldLeft = false;
		thisGlider->heldRight = false;
		if (BitTst(&theKeys, thisGlider->rightKey))			// right key
		{
		#ifdef CREATEDEMODATA
			LogDemoKey(0);
		#endif
			if (BitTst(&theKeys, thisGlider->leftKey))
			{
				ToggleGliderFacing(thisGlider);
				thisGlider->heldLeft = true;
			}
			else
			{
				thisGlider->hDesiredVel += kNormalThrust;
				thisGlider->tipped = (thisGlider->facing == kFaceLeft);
				thisGlider->heldRight = true;
			}
		}
		else if (BitTst(&theKeys, thisGlider->leftKey))		// left key
		{
		#ifdef CREATEDEMODATA
			LogDemoKey(1);
		#endif
			thisGlider->hDesiredVel -= kNormalThrust;
			thisGlider->tipped = (thisGlider->facing == kFaceRight);
			thisGlider->heldLeft = true;
		}
		else
			thisGlider->tipped = false;
		
		if ((BitTst(&theKeys, thisGlider->battKey)) && (batteryTotal != 0) && 
				(thisGlider->mode == kGliderNormal))
		{
		#ifdef CREATEDEMODATA
			LogDemoKey(2);
		#endif
			if (batteryTotal > 0)
				DoBatteryEngaged(thisGlider);
			else
				DoHeliumEngaged(thisGlider);
		}
		else
			batteryWasEngaged = false;
		
		if ((BitTst(&theKeys, thisGlider->bandKey)) && (bandsTotal > 0) && 
				(thisGlider->mode == kGliderNormal))
		{
		#ifdef CREATEDEMODATA
			LogDemoKey(3);
		#endif
			if (!thisGlider->fireHeld)
			{
				if (AddBand(thisGlider, thisGlider->dest.left + 24, 
						thisGlider->dest.top + 10, thisGlider->facing))
				{
					bandsTotal--;
					if (bandsTotal <= 0)
						QuickBandsRefresh(false);
					
					thisGlider->fireHeld = true;
				}
			}
		}
		else
			thisGlider->fireHeld = false;
		
		if ((otherPlayerEscaped != kNoOneEscaped) && 
				(BitTst(&theKeys, kDeleteKeyMap)) && 
				(thisGlider->which) && (!onePlayerLeft))
		{
			ForceKillGlider();
		}
		
		if ((isEscPauseKey && BitTst(&theKeys, kEscKeyMap)) || 
				(!isEscPauseKey && BitTst(&theKeys, kTabKeyMap)))
		{
			DoPause();
		}
	}
}

//--------------------------------------------------------------  QuerySaveGame

Boolean QuerySaveGame (void)
{
	#define		kSaveGameAlert		1041
	#define		kYesSaveGameButton	1
	short		hitWhat;
	
	InitCursor();
	FlushEvents(everyEvent, 0);
//	CenterAlert(kSaveGameAlert);
	hitWhat = Alert(kSaveGameAlert, nil);
	if (hitWhat == kYesSaveGameButton)
		return (true);
	else
		return (false);
}

