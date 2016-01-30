
//============================================================================
//----------------------------------------------------------------------------
//									 Play.c
//----------------------------------------------------------------------------
//============================================================================


#include <Resources.h>
#include "Externs.h"
#include "Environ.h"
#include "House.h"
#include "MainWindow.h"
#include "RectUtils.h"
#include "Scoreboard.h"


#define kHouseBannerAlert		1009
#define kInitialGliders			2
#define kRingDelay				90
#define kRingSpread				25000	// 25000
#define kRingBaseDelay			5000	// 5000
#define kChimeDelay				180


typedef struct
{
	short		nextRing;
	short		rings;
	short		delay;
} phoneType, *phonePtr;


void InitGlider (gliderPtr, short);
void SetHouseToFirstRoom (void);
void SetHouseToSavedRoom (void);
void HandlePlayEvent (void);
void PlayGame (void);
void HandleRoomVisitation (void);
void SetObjectsToDefaults (void);
void InitTelephone (void);
void HandleTelephone (void);
Boolean DoesStarCodeExist (short);
short GetNumStarsRemaining (short, short);


phoneType	thePhone, theChimes;
Rect		glidSrcRect, justRoomsRect;
GWorldPtr	glidSrcMap, glid2SrcMap;
GWorldPtr	glidMaskMap;
long		gameFrame;
short		batteryTotal, bandsTotal, foilTotal, mortals;
Boolean		playing, evenFrame, twoPlayerGame, showFoil, demoGoing;
Boolean		doBackground, playerSuicide, phoneBitSet, tvOn;

extern	WindowPtr	menuWindow;
extern	FSSpecPtr	theHousesSpecs;
extern	demoPtr		demoData;
extern	gameType	smallGame;
extern	Rect		gliderSrc[kNumGliderSrcRects];
extern	Rect		boardDestRect, boardSrcRect;
extern	long		incrementModeTime;
extern	short		numBands, otherPlayerEscaped, demoIndex, demoHouseIndex;
extern	short		splashOriginH, splashOriginV, countDown, thisHouseIndex;
extern	short		numStarsRemaining, numChimes, saidFollow;
extern	Boolean		quitting, isMusicOn, gameOver, hasMirror, onePlayerLeft;
extern	Boolean		isPlayMusicIdle, failedMusic, quickerTransitions;
extern	Boolean		switchedOut;


//==============================================================  Functions
//--------------------------------------------------------------  NewGame

void NewGame (short mode)
{
	Rect		tempRect;
	Size		freeBytes, growBytes;
	OSErr		theErr;
	Boolean		wasPlayMusicPref;
	
	AdjustScoreboardHeight();
	gameOver = false;
	theMode = kPlayMode;
	if (isPlayMusicGame)
	{
		if (!isMusicOn)
		{
			theErr = StartMusic();
			if (theErr != noErr)
			{
				YellowAlert(kYellowNoMusic, theErr);
				failedMusic = true;
			}
		}
		SetMusicalMode(kPlayGameScoreMode);
	}
	else
	{
		if (isMusicOn)
			StopTheMusic();
	}
	if (mode != kResumeGameMode)
		SetObjectsToDefaults();
	HideCursor();
	if (mode == kResumeGameMode)
		SetHouseToSavedRoom();
	else if (mode == kNewGameMode)
		SetHouseToFirstRoom();
	DetermineRoomOpenings();
	NilSavedMaps();
	
	gameFrame = 0L;
	numBands = 0;
	demoIndex = 0;
	saidFollow = 0;
	otherPlayerEscaped = kNoOneEscaped;
	onePlayerLeft = false;
	playerSuicide = false;
	
	if (twoPlayerGame)					// initialize glider(s)
	{
		InitGlider(&theGlider, kNewGameMode);
		InitGlider(&theGlider2, kNewGameMode);
		SetPort((GrafPtr)glidSrcMap);
		LoadGraphic(kGliderPictID);
		SetPort((GrafPtr)glid2SrcMap);
		LoadGraphic(kGlider2PictID);
	}
	else
	{
		InitGlider(&theGlider, mode);
		SetPort((GrafPtr)glidSrcMap);
		LoadGraphic(kGliderPictID);
		SetPort((GrafPtr)glid2SrcMap);
		LoadGraphic(kGliderFoilPictID);
	}
	
#if !BUILD_ARCADE_VERSION
//	HideMenuBarOld();		// TEMP
#endif
	
	SetPort((GrafPtr)mainWindow);		// paint strip on screen black
	tempRect = thisMac.screen;
	tempRect.top = tempRect.bottom - 20;	// thisMac.menuHigh
	PaintRect(&tempRect);
	
#ifdef COMPILEQT
	if ((thisMac.hasQT) && (hasMovie))
	{
		SetMovieGWorld(theMovie, (CGrafPtr)mainWindow, nil);
	}
#endif
	
	SetPort((GrafPtr)workSrcMap);
	PaintRect(&workSrcRect);
//	if (quickerTransitions)
//		DissBitsChunky(&workSrcRect);
//	else
//		DissBits(&workSrcRect);
	
//	DebugStr("\pIf screen isn't black, exit to shell.");	// TEMP TEMP TEMP
	
	DrawLocale();
	RefreshScoreboard(kNormalTitleMode);
//	if (quickerTransitions)
//		DissBitsChunky(&justRoomsRect);
//	else
//		DissBits(&justRoomsRect);
	if (mode == kNewGameMode)
	{
		BringUpBanner();
		DumpScreenOn(&justRoomsRect);
	}
	else if (mode == kResumeGameMode)
	{
		DisplayStarsRemaining();
		DumpScreenOn(&justRoomsRect);
	}
	else
	{
		DumpScreenOn(&justRoomsRect);
	}
	
	InitGarbageRects();
	StartGliderFadingIn(&theGlider);
	if (twoPlayerGame)
	{
		StartGliderFadingIn(&theGlider2);
		TagGliderIdle(&theGlider2);
		theGlider2.dontDraw = true;
	}
	InitTelephone();
	wasPlayMusicPref = isPlayMusicGame;
	
	freeBytes = MaxMem(&growBytes);
	
#ifdef CREATEDEMODATA
	SysBeep(1);
#endif
	
#ifdef COMPILEQT
	if ((thisMac.hasQT) && (hasMovie) && (tvInRoom))
	{
		SetMovieActive(theMovie, true);
		if (tvOn)
		{
			StartMovie(theMovie);
			MoviesTask(theMovie, 0);
		}
	}
#endif
	
	playing = true;		// everything before this line is game set-up
	PlayGame();			// everything following is after a game has ended
	
#ifdef CREATEDEMODATA
	DumpToResEditFile((Ptr)demoData, sizeof(demoType) * (long)demoIndex);
#endif
	
	isPlayMusicGame = wasPlayMusicPref;
	ZeroMirrorRegion();
	
#ifdef COMPILEQT
	if ((thisMac.hasQT) && (hasMovie) && (tvInRoom))
	{
		tvInRoom = false;
		StopMovie(theMovie);
		SetMovieActive(theMovie, false);
	}
#endif
	
	twoPlayerGame = false;
	theMode = kSplashMode;
	InitCursor();
	if (isPlayMusicIdle)
	{
		if (!isMusicOn)
		{
			theErr = StartMusic();
			if (theErr != noErr)
			{
				YellowAlert(kYellowNoMusic, theErr);
				failedMusic = true;
			}
		}
		SetMusicalMode(kPlayWholeScoreMode);
	}
	else
	{
		if (isMusicOn)
			StopTheMusic();
	}
	NilSavedMaps();
	SetPortWindowPort(mainWindow);
	BlackenScoreboard();
	UpdateMenus(false);
	
	if (!gameOver)
	{
		CGrafPtr	wasCPort;
		GDHandle	wasWorld;
		
		GetGWorld(&wasCPort, &wasWorld);
		
		InvalWindowRect(mainWindow, &mainWindowRect);
		
		SetGWorld(workSrcMap, nil);
		PaintRect(&workSrcRect);
		QSetRect(&tempRect, 0, 0, 640, 460);
		QOffsetRect(&tempRect, splashOriginH, splashOriginV);
		LoadScaledGraphic(kSplash8BitPICT, &tempRect);
		
		SetGWorld(wasCPort, wasWorld);
	}
	WaitCommandQReleased();
	demoGoing = false;
	incrementModeTime = TickCount() + kIdleSplashTicks;
}

//--------------------------------------------------------------  DoDemoGame

void DoDemoGame (void)
{
	short		wasHouseIndex;
	Boolean		whoCares;
	
	wasHouseIndex = thisHouseIndex;
	whoCares = CloseHouse();
	thisHouseIndex = demoHouseIndex;
	PasStringCopy(theHousesSpecs[thisHouseIndex].name, thisHouseName);
	if (OpenHouse())
	{
		whoCares = ReadHouse();
		demoGoing = true;
		NewGame(kNewGameMode);
	}
	whoCares = CloseHouse();
	thisHouseIndex = wasHouseIndex;
	PasStringCopy(theHousesSpecs[thisHouseIndex].name, thisHouseName);
	if (OpenHouse())
		whoCares = ReadHouse();
	incrementModeTime = TickCount() + kIdleSplashTicks;
}

//--------------------------------------------------------------  InitGlider

void InitGlider (gliderPtr thisGlider, short mode)
{	
	WhereDoesGliderBegin(&thisGlider->dest, mode);
	
	if (mode == kResumeGameMode)
		numStarsRemaining = smallGame.wasStarsLeft;
	else if (mode == kNewGameMode)
		numStarsRemaining = CountStarsInHouse();
	
	if (mode == kResumeGameMode)
	{
		theScore = smallGame.score;
		mortals = smallGame.numGliders;
		batteryTotal = smallGame.energy;
		bandsTotal = smallGame.bands;
		foilTotal = smallGame.foil;
		thisGlider->mode = smallGame.gliderState;
		thisGlider->facing = smallGame.facing;
		showFoil = smallGame.showFoil;
		
		switch (thisGlider->mode)
		{
			case kGliderBurning:
			FlagGliderBurning(thisGlider);
			break;
			
			default:
			FlagGliderNormal(thisGlider);
			break;
		}
	}
	else
	{
		theScore = 0L;
		mortals = kInitialGliders;
		if (twoPlayerGame)
			mortals += kInitialGliders;
		batteryTotal = 0;
		bandsTotal = 0;
		foilTotal = 0;
		thisGlider->mode = kGliderNormal;
		thisGlider->facing = kFaceRight;
		thisGlider->src = gliderSrc[0];
		thisGlider->mask = gliderSrc[0];
		showFoil = false;
	}
	
	QSetRect(&thisGlider->destShadow, 0, 0, kGliderWide, kShadowHigh);
	QOffsetRect(&thisGlider->destShadow, thisGlider->dest.left, kShadowTop);
	thisGlider->wholeShadow = thisGlider->destShadow;
	
	thisGlider->hVel = 0;
	thisGlider->vVel = 0;
	thisGlider->hDesiredVel = 0;
	thisGlider->vDesiredVel = 0;
	
	thisGlider->tipped = false;
	thisGlider->sliding = false;
	thisGlider->dontDraw = false;
}

//--------------------------------------------------------------  SetHouseToFirstRoom

void SetHouseToFirstRoom (void)
{
	short		firstRoom;
	
	firstRoom = GetFirstRoomNumber();
	ForceThisRoom(firstRoom);
}

//--------------------------------------------------------------  SetHouseToSavedRoom

void SetHouseToSavedRoom (void)
{
	ForceThisRoom(smallGame.roomNumber);
}

//--------------------------------------------------------------  HandlePlayEvent

void HandlePlayEvent (void)
{
	EventRecord	theEvent;
	GrafPtr		wasPort;
	long		sleep = 2;
	
	if (WaitNextEvent(everyEvent, &theEvent, sleep, nil))
	{
		if ((theEvent.what == updateEvt) && 
				((WindowPtr)theEvent.message == mainWindow))
		{
			GetPort(&wasPort);
			SetPortWindowPort(mainWindow);
			BeginUpdate(mainWindow);
			CopyBits((BitMap *)*GetGWorldPixMap(workSrcMap), 
					GetPortBitMapForCopyBits(GetWindowPort(mainWindow)), 
					&justRoomsRect, &justRoomsRect, srcCopy, nil);
			RefreshScoreboard(kNormalTitleMode);
			EndUpdate(mainWindow);
			SetPort(wasPort);
		}
		else if ((theEvent.what == osEvt) && (theEvent.message & 0x01000000))
		{
			if (theEvent.message & 0x00000001)	// resume event
			{
				switchedOut = false;
				ToggleMusicWhilePlaying();
				HideCursor();
//				HideMenuBarOld();	// TEMP
			}
			else								// suspend event
			{
				InitCursor();
				switchedOut = true;
				ToggleMusicWhilePlaying();
//				ShowMenuBarOld();		// TEMP replace with Carbon calls
			}
		}
	}
}

//--------------------------------------------------------------  PlayGame

void PlayGame (void)
{
	while ((playing) && (!quitting))
	{
		gameFrame++;
		evenFrame = !evenFrame;
		
		if (doBackground)
		{
			do
			{
				HandlePlayEvent();
			} while (switchedOut);
		}
		
		HandleTelephone();
		
		if (twoPlayerGame)
		{
			HandleDynamics();
			if (!gameOver)
			{
				GetInput(&theGlider);
				GetInput(&theGlider2);
				HandleInteraction();
			}
			HandleTriggers();
			HandleBands();
			if (!gameOver)
			{
				HandleGlider(&theGlider);
				HandleGlider(&theGlider2);
			}
			if (playing)
			{
#ifdef COMPILEQT
				if ((thisMac.hasQT) && (hasMovie) && (tvInRoom) && (tvOn))
						MoviesTask(theMovie, 0);
#endif
				RenderFrame();
				HandleDynamicScoreboard();
			}
		}
		else
		{
			HandleDynamics();
			if (!gameOver)
			{
				if (demoGoing)
					GetDemoInput(&theGlider);
				else
					GetInput(&theGlider);
				HandleInteraction();
			}
			HandleTriggers();
			HandleBands();
			if (!gameOver)
				HandleGlider(&theGlider);
			if (playing)
			{
#ifdef COMPILEQT
				if ((thisMac.hasQT) && (hasMovie) && (tvInRoom) && (tvOn))
						MoviesTask(theMovie, 0);
#endif
				RenderFrame();
				HandleDynamicScoreboard();
			}
		}
		
		if (gameOver)
		{
			countDown--;
			if (countDown <= 0)
			{
				CGrafPtr	wasCPort;
				GDHandle	wasWorld;
				
				GetGWorld(&wasCPort, &wasWorld);
				
				HideGlider(&theGlider);
				RefreshScoreboard(kNormalTitleMode);
				
#if BUILD_ARCADE_VERSION
			// Need to paint over the scoreboard black.
				
				SetGWorld(boardSrcMap, nil);
				PaintRect(&boardSrcRect);
				
				CopyBits((BitMap *)*GetGWorldPixMap(boardSrcMap), 
						GetPortBitMapForCopyBits(GetWindowPort(mainWindow)), 
						&boardSrcRect, &boardDestRect, srcCopy, 0L);
				
				{
					Rect		bounds;
					PicHandle	thePicture;
					SInt16		hOffset;
					
					if (boardSrcRect.right >= 640)
						hOffset = (RectWide(&boardSrcRect) - kMaxViewWidth) / 2;
					else
						hOffset = -576;
					thePicture = GetPicture(kScoreboardPictID);
					if (!thePicture)
						RedAlert(kErrFailedGraphicLoad);
					HLock((Handle)thePicture);
					bounds = (*thePicture)->picFrame;
					HUnlock((Handle)thePicture);
					QOffsetRect(&bounds, -bounds.left, -bounds.top);
					QOffsetRect(&bounds, hOffset, 0);
					DrawPicture(thePicture, &bounds);
					ReleaseResource((Handle)thePicture);
				}
#else
//				ShowMenuBarOld();	// TEMP
#endif
				
				if (mortals < 0)
					DoDiedGameOver();
				else
					DoGameOver();
				
				SetGWorld(wasCPort, wasWorld);
			}
		}
	}
	
#if BUILD_ARCADE_VERSION
	{
		CGrafPtr	wasCPort;
		GDHandle	wasWorld;
		
		GetGWorld(&wasCPort, &wasWorld);
		
		SetGWorld(boardSrcMap, nil);
		PaintRect(&boardSrcRect);
		
		CopyBits((BitMap *)*GetGWorldPixMap(boardSrcMap), 
				GetPortBitMapForCopyBits(GetWindowPort(mainWindow)), 
				&boardSrcRect, &boardDestRect, srcCopy, 0L);
		
		SetGWorld(wasCPort, wasWorld);
	}
	
	{
		Rect		bounds;
		PicHandle	thePicture;
		SInt16		hOffset;
		
		if (boardSrcRect.right >= 640)
			hOffset = (RectWide(&boardSrcRect) - kMaxViewWidth) / 2;
		else
			hOffset = -576;
		thePicture = GetPicture(kScoreboardPictID);
		if (!thePicture)
			RedAlert(kErrFailedGraphicLoad);
		HLock((Handle)thePicture);
		bounds = (*thePicture)->picFrame;
		HUnlock((Handle)thePicture);
		QOffsetRect(&bounds, -bounds.left, -bounds.top);
		QOffsetRect(&bounds, hOffset, 0);
		DrawPicture(thePicture, &bounds);
		ReleaseResource((Handle)thePicture);
	}
	
#else
	
//	ShowMenuBarOld();	// TEMP
	
#endif
}

//--------------------------------------------------------------  SetObjectsToDefaults

void SetObjectsToDefaults (void)
{
	houseType	*thisHousePtr;
	short		numRooms;
	short		r, i;
	char		wasState;
	Boolean		initState;
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	thisHousePtr = *thisHouse;
	
	numRooms = thisHousePtr->nRooms;
	
	for (r = 0; r < numRooms; r++)
	{
		thisHousePtr->rooms[r].visited = false;
		for (i = 0; i < kMaxRoomObs; i++)
		{
			switch (thisHousePtr->rooms[r].objects[i].what)
			{
				case kFloorVent:
				case kCeilingVent:
				case kFloorBlower:
				case kCeilingBlower:
				case kLeftFan:
				case kRightFan:
				case kSewerGrate:
				case kInvisBlower:
				case kGrecoVent:
				case kSewerBlower:
				case kLiftArea:
				thisHousePtr->rooms[r].objects[i].data.a.state = 
					thisHousePtr->rooms[r].objects[i].data.a.initial;
				break;
				
				case kRedClock:
				case kBlueClock:
				case kYellowClock:
				case kCuckoo:
				case kPaper:
				case kBattery:
				case kBands:
				case kGreaseRt:
				case kGreaseLf:
				case kFoil:
				case kInvisBonus:
				case kStar:
				case kSparkle:
				case kHelium:
				thisHousePtr->rooms[r].objects[i].data.c.state = 
					thisHousePtr->rooms[r].objects[i].data.c.initial;
				break;
				
				case kDeluxeTrans:
				initState = (thisHousePtr->rooms[r].objects[i].data.d.wide & 0xF0) >> 4;
				thisHousePtr->rooms[r].objects[i].data.d.wide &= 0xF0;
				thisHousePtr->rooms[r].objects[i].data.d.wide += initState;
				break;
				
				case kCeilingLight:
				case kLightBulb:
				case kTableLamp:
				case kHipLamp:
				case kDecoLamp:
				case kFlourescent:
				case kTrackLight:
				case kInvisLight:
				thisHousePtr->rooms[r].objects[i].data.f.state = 
					thisHousePtr->rooms[r].objects[i].data.f.initial;
				break;
				
				case kStereo:
				thisHousePtr->rooms[r].objects[i].data.g.state = isPlayMusicGame;
				break;
				
				case kShredder:
				case kToaster:
				case kMacPlus:
				case kGuitar:
				case kTV:
				case kCoffee:
				case kOutlet:
				case kVCR:
				case kMicrowave:
				thisHousePtr->rooms[r].objects[i].data.g.state = 
					thisHousePtr->rooms[r].objects[i].data.g.initial;
				break;
				
				case kBalloon:
				case kCopterLf:
				case kCopterRt:
				case kDartLf:
				case kDartRt:
				case kBall:
				case kDrip:
				case kFish:
				thisHousePtr->rooms[r].objects[i].data.h.state = 
					thisHousePtr->rooms[r].objects[i].data.h.initial;
				break;
				
			}
		}
	}
	HSetState((Handle)thisHouse, wasState);
}

//--------------------------------------------------------------  HideGlider

void HideGlider (gliderPtr thisGlider)
{
	Rect		tempRect;
	
	tempRect = thisGlider->whole;
	QOffsetRect(&tempRect, playOriginH, playOriginV);
	CopyRectWorkToMain(&tempRect);
	
	if (hasMirror)
	{
		QOffsetRect(&tempRect, -20, -16);
		CopyRectWorkToMain(&tempRect);
	}
	
	tempRect = thisGlider->wholeShadow;
	QOffsetRect(&tempRect, playOriginH, playOriginV);
	CopyRectWorkToMain(&tempRect);
}

//--------------------------------------------------------------  InitTelephone

void InitTelephone (void)
{
	thePhone.nextRing = RandomInt(kRingSpread) + kRingBaseDelay;
	thePhone.rings = RandomInt(3) + 3;
	thePhone.delay = kRingDelay;
	
	theChimes.nextRing = RandomInt(kChimeDelay) + 1;
}

//--------------------------------------------------------------  HandleTelephone

void HandleTelephone (void)
{
	short		delayTime;
	
	if (!phoneBitSet)
	{
		if (thePhone.nextRing == 0)
		{
			if (thePhone.delay == 0)
			{
				thePhone.delay = kRingDelay;
				PlayPrioritySound(kPhoneRingSound, kPhoneRingPriority);
				thePhone.rings--;
				if (thePhone.rings == 0)
				{
					thePhone.nextRing = RandomInt(kRingSpread) + kRingBaseDelay;
					thePhone.rings = RandomInt(3) + 3;
				}
			}
			else
				thePhone.delay--;
		}
		else
			thePhone.nextRing--;
	}
	// handle also the wind chimes (if they are present)
	
	if (numChimes > 0)
	{
		if (theChimes.nextRing == 0)
		{
			if (RandomInt(2) == 0)
				PlayPrioritySound(kChime1Sound, kChime1Priority);
			else
				PlayPrioritySound(kChime2Sound, kChime2Priority);
			
			delayTime = kChimeDelay / numChimes;
			if (delayTime < 2)
				delayTime = 2;
			
			theChimes.nextRing = RandomInt(delayTime) + 1;
		}
		else
			theChimes.nextRing--;
	}
}

//--------------------------------------------------------------  StrikeChime

void StrikeChime (void)
{
	theChimes.nextRing = 0;
}

//--------------------------------------------------------------  RestoreEntireGameScreen

void RestoreEntireGameScreen (void)
{
	Rect		tempRect;
	
	HideCursor();
	
#if !BUILD_ARCADE_VERSION
//	HideMenuBarOld();		// TEMP
#endif
	
	SetPort((GrafPtr)mainWindow);
	tempRect = thisMac.screen;
	PaintRect(&tempRect);
	
	DrawLocale();
	RefreshScoreboard(kNormalTitleMode);
//	if (quickerTransitions)
//		DissBitsChunky(&justRoomsRect);
//	else
//		DissBits(&justRoomsRect);
}

