//============================================================================
//----------------------------------------------------------------------------
//									GameOver.c
//----------------------------------------------------------------------------
//============================================================================


#include <ToolUtils.h>
#include "Externs.h"
#include "Environ.h"
#include "MainWindow.h"
#include "Objects.h"
#include "RectUtils.h"
#include "Utilities.h"


#define kNumCountDownFrames		16
#define kPageFrames				14
#define kPagesPictID			1990
#define kPagesMaskID			1989
#define kLettersPictID			1988
#define kMilkywayPictID			1021


typedef struct
{
	Rect		dest, was;
	short		frame, counter;
	Boolean		stuck;
} pageType, *pagePtr;


void DoGameOverStarAnimation (void);
void SetUpFinalScreen (void);
void InitDiedGameOver (void);
void HandlePages (void);
void DrawPages (void);


pageType	pages[8];
Rect		pageSrcRect, pageSrc[kPageFrames], lettersSrc[8], angelSrcRect;
RgnHandle	roomRgn;
GWorldPtr	pageSrcMap, gameOverSrcMap, angelSrcMap;
GWorldPtr	pageMaskMap, angelMaskMap;
short		countDown, stopPages, pagesStuck;
Boolean		gameOver;

extern	Rect		justRoomsRect;
extern	short		splashOriginH, splashOriginV, numWork2Main;
extern	short		numBack2Work;
extern	Boolean		playing, shadowVisible, demoGoing;


//==============================================================  Functions
//--------------------------------------------------------------  DoGameOver

// Handles a game over.  This is a game over where the player has…
// completed the house.

void DoGameOver (void)
{	
	playing = false;
	SetUpFinalScreen();
	SetPort((GrafPtr)mainWindow);
	ColorRect(&mainWindowRect, 244);
	DoGameOverStarAnimation();
	if (!TestHighScore())
		RedrawSplashScreen();
}

//--------------------------------------------------------------  SetUpFinalScreen

// This sets up the game over screen (again, this function is for when…
// the player completes the house).

void SetUpFinalScreen (void)
{
	Rect		tempRect;
	Str255		tempStr, subStr;
	short		count, offset, i, textDown;
	char		wasState;
	
	SetPort((GrafPtr)workSrcMap);
	ColorRect(&workSrcRect, 244);
	QSetRect(&tempRect, 0, 0, 640, 460);
	CenterRectInRect(&tempRect, &workSrcRect);
	LoadScaledGraphic(kMilkywayPictID, &tempRect);
	textDown = tempRect.top;
	if (textDown < 0)
		textDown = 0;
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	PasStringCopy((*thisHouse)->trailer, tempStr);
	HSetState((Handle)thisHouse, wasState);
	
	count = 0;
	do
	{
		GetLineOfText(tempStr, count, subStr);
		offset = ((thisMac.screen.right - thisMac.screen.left) - 
				TextWidth(subStr, 1, subStr[0])) / 2;
		TextFont(applFont);
		TextFace(bold);
		TextSize(12);
		ForeColor(blackColor);
		MoveTo(offset + 1, textDown + 33 + (count * 20));
		DrawString(subStr);
		ForeColor(whiteColor);
		MoveTo(offset, textDown + 32 + (count * 20));
		DrawString(subStr);
		ForeColor(blackColor);
		count++;
	}
	while (subStr[0] > 0);
	
	CopyRectWorkToBack(&workSrcRect);
	
	for (i = 0; i < 5; i++)		// initialize the falling stars
	{
		pages[i].dest = starSrc[0];
		QOffsetRect(&pages[i].dest, 
				workSrcRect.right + RandomInt(workSrcRect.right / 5) + 
				(workSrcRect.right/ 4) * i, 
				RandomInt(workSrcRect.bottom) - workSrcRect.bottom / 2);
		pages[i].was = pages[i].dest;
		pages[i].frame = RandomInt(6);
	}
}

//--------------------------------------------------------------  DoGameOverStarAnimation

// This handles the falling stars and the flying angel when a player…
// completes a house.

void DoGameOverStarAnimation (void)
{
	#define		kStarFalls	8
	EventRecord	theEvent;
	KeyMap		theKeys;
	Rect		angelDest;
	long		nextLoop;
	short		which, i, count, pass;
	Boolean		noInteruption;
	
	angelDest = angelSrcRect;
	QOffsetRect(&angelDest, -96, 0);
	noInteruption = true;
	nextLoop = TickCount() + 2;
	count = 0;
	pass = 0;
	FlushEvents(everyEvent, 0);
	
	while (noInteruption)
	{
		if ((angelDest.left % 32) == 0)		// add a star
		{
			PlayPrioritySound(kMysticSound, kMysticPriority);
			which = angelDest.left / 32;
			which = which % 5;
			ZeroRectCorner(&pages[which].dest);
			QOffsetRect(&pages[which].dest, angelDest.left, angelDest.bottom);
			if (count < (which + 1))
				count = which + 1;
		}
		
		for (i = 0; i < count; i++)
		{
			pages[i].frame++;
			if (pages[i].frame >= 6)
				pages[i].frame = 0;
			
			CopyMask((BitMap *)*GetGWorldPixMap(bonusSrcMap), 
					(BitMap *)*GetGWorldPixMap(bonusMaskMap), 
					(BitMap *)*GetGWorldPixMap(workSrcMap), 
					&starSrc[pages[i].frame], 
					&starSrc[pages[i].frame], 
					&pages[i].dest);
			
			pages[i].was = pages[i].dest;
			pages[i].was.top -= kStarFalls;
			
			AddRectToWorkRectsWhole(&pages[i].was);
			AddRectToBackRects(&pages[i].dest);
			
			if (pages[i].dest.top < workSrcRect.bottom)
				QOffsetRect(&pages[i].dest, 0, kStarFalls);
		}
		
		if (angelDest.left <= (workSrcRect.right + 2))
		{
			CopyMask((BitMap *)*GetGWorldPixMap(angelSrcMap), 
					(BitMap *)*GetGWorldPixMap(angelMaskMap), 
					(BitMap *)*GetGWorldPixMap(workSrcMap), 
					&angelSrcRect, &angelSrcRect, &angelDest);
			angelDest.left -= 2;
			AddRectToWorkRectsWhole(&angelDest);
			angelDest.left += 2;
			AddRectToBackRects(&angelDest);
			QOffsetRect(&angelDest, 2, 0);
			pass = 0;
		}
		
		CopyRectsQD();
		
		numWork2Main = 0;
		numBack2Work = 0;
		
		do
		{
			GetKeys(theKeys);
			if ((BitTst(&theKeys, kCommandKeyMap)) || (BitTst(&theKeys, kOptionKeyMap)) || 
					(BitTst(&theKeys, kShiftKeyMap)) || (BitTst(&theKeys, kControlKeyMap)))
				noInteruption = false;
			if (GetNextEvent(everyEvent, &theEvent))
				if ((theEvent.what == mouseDown) || (theEvent.what == keyDown))
					noInteruption = false;
		}
		while (TickCount() < nextLoop);
		nextLoop = TickCount() + 2;
		
		if (pass < 80)
			pass++;
		else
		{
			WaitForInputEvent(5);
			noInteruption = false;
		}
	}
}

//--------------------------------------------------------------  FlagGameOver

// Called to indicate (flag) that a game is over.  Actual game over…
// sequence comes up after a short delay.

void FlagGameOver (void)
{
	gameOver = true;
	countDown = kNumCountDownFrames;
	SetMusicalMode(kPlayWholeScoreMode);
}

//--------------------------------------------------------------  InitDiedGameOver
// This is called when a game is over due to the fact that the player…
// lost their last glider (died), not due to getting through the entire…
// house.  This function initializes the strucures/variables.

void InitDiedGameOver (void)
{
	#define		kPageSpacing		40
	#define		kPageRightOffset	128
	#define		kPageBackUp			128
	short		i;
	CGrafPtr	wasCPort;
	GDHandle	wasWorld;
	OSErr		theErr;
	
	GetGWorld(&wasCPort, &wasWorld);
	
	QSetRect(&pageSrcRect, 0, 0, 25, 32 * 8);
	theErr = CreateOffScreenGWorld(&gameOverSrcMap, &pageSrcRect, kPreferredDepth);
	SetGWorld(gameOverSrcMap, nil);
	LoadGraphic(kLettersPictID);
	
	QSetRect(&pageSrcRect, 0, 0, 32, 32 * kPageFrames);
	theErr = CreateOffScreenGWorld(&pageSrcMap, &pageSrcRect, kPreferredDepth);
	SetGWorld(pageSrcMap, nil);
	LoadGraphic(kPagesPictID);
	
	theErr = CreateOffScreenGWorld(&pageMaskMap, &pageSrcRect, 1);	
	SetGWorld(pageMaskMap, nil);
	LoadGraphic(kPagesMaskID);
	
	for (i = 0; i < kPageFrames; i++)	// initialize src page rects
	{
		QSetRect(&pageSrc[i], 0, 0, 32, 32);
		QOffsetRect(&pageSrc[i], 0, 32 * i);
	}
	
	for (i = 0; i < 8; i++)				// initialize dest page rects
	{
		QSetRect(&pages[i].dest, 0, 0, 32, 32);
		CenterRectInRect(&pages[i].dest, &thisMac.screen);
		QOffsetRect(&pages[i].dest, -thisMac.screen.left, -thisMac.screen.top);
		if (i < 4)
			QOffsetRect(&pages[i].dest, -kPageSpacing * (4 - i), 0);
		else
			QOffsetRect(&pages[i].dest, kPageSpacing * (i - 3), 0);
		QOffsetRect(&pages[i].dest, (thisMac.screen.right - thisMac.screen.left) / -2, 
				(thisMac.screen.right - thisMac.screen.left) / -2);
		if (pages[i].dest.left % 2 == 1)
			QOffsetRect(&pages[i].dest, 1, 0);
		pages[i].was = pages[i].dest;
		pages[i].frame = 0;
		pages[i].counter = RandomInt(32);
		pages[i].stuck = false;
	}
	
	for (i = 0; i < 8; i++)
	{
		QSetRect(&lettersSrc[i], 0, 0, 25, 32);
		QOffsetRect(&lettersSrc[i], 0, 32 * i);
	}
	
	roomRgn = NewRgn();
	RectRgn(roomRgn, &justRoomsRect);
	pagesStuck = 0;
	stopPages = ((thisMac.screen.bottom - thisMac.screen.top) / 2) - 16;
}

//--------------------------------------------------------------  HandlePages

// This handles the pieces of paper that blow across the screen.

void HandlePages (void)
{
	short		i;
	
	for (i = 0; i < 8; i++)
	{
		if ((pages[i].dest.bottom + RandomInt(8)) > stopPages)
		{
			pages[i].frame = 0;
			if (!pages[i].stuck)
			{
				pages[i].dest.right = pages[i].dest.left + 25;
				pages[i].stuck = true;
				pagesStuck++;
			}
		}
		else
		{
			if (pages[i].frame == 0)
			{
				pages[i].counter--;
				if (pages[i].counter <= 0)
					pages[i].frame = 1;
			}
			else if (pages[i].frame == 7)
			{
				pages[i].counter--;
				if (pages[i].counter <= 0)
				{
					pages[i].frame = 8;
					if (RandomInt(2) == 0)
						PlayPrioritySound(kPaper3Sound, kPapersPriority);
					else
						PlayPrioritySound(kPaper4Sound, kPapersPriority);
				}
				else
					QOffsetRect(&pages[i].dest, 10, 10);
			}
			else
			{
				pages[i].frame++;
				switch (pages[i].frame)
				{
					case 5:
					QOffsetRect(&pages[i].dest, 6, 6);
					break;
					
					case 6:
					QOffsetRect(&pages[i].dest, 8, 8);
					break;
					
					case 7:
					QOffsetRect(&pages[i].dest, 8, 8);
					pages[i].counter = RandomInt(4) + 4;
					break;
					
					case 8:
					case 9:
					QOffsetRect(&pages[i].dest, 8, 8);
					break;
					
					case 10:
					QOffsetRect(&pages[i].dest, 6, 6);
					break;
					
					case kPageFrames:
					QOffsetRect(&pages[i].dest, 8, 0);
					pages[i].frame = 0;
					pages[i].counter = RandomInt(8) + 8;
					if (RandomInt(2) == 0)
						PlayPrioritySound(kPaper1Sound, kPapersPriority);
					else
						PlayPrioritySound(kPaper2Sound, kPapersPriority);
					break;
				}
			}
		}
	}
}

//--------------------------------------------------------------  DrawPages

// This function does the drawing for the pieces of paper that blow… 
// across the screen.

void DrawPages (void)
{
	short		i;
	
	for (i = 0; i < 8; i++)
	{
		if (pages[i].stuck)
		{
			CopyBits((BitMap *)*GetGWorldPixMap(gameOverSrcMap), 
					(BitMap *)*GetGWorldPixMap(workSrcMap), 
					&lettersSrc[i], &pages[i].dest, 
					srcCopy, roomRgn);
		}
		else
		{
			CopyMask((BitMap *)*GetGWorldPixMap(pageSrcMap), 
					(BitMap *)*GetGWorldPixMap(pageMaskMap), 
					(BitMap *)*GetGWorldPixMap(workSrcMap), 
					&pageSrc[pages[i].frame], 
					&pageSrc[pages[i].frame], 
					&pages[i].dest);
		}
		
		QUnionSimilarRect(&pages[i].dest, &pages[i].was, &pages[i].was);
		AddRectToWorkRects(&pages[i].was);
		AddRectToBackRects(&pages[i].dest);
		
		CopyRectsQD();
		
		numWork2Main = 0;
		numBack2Work = 0;
		
		pages[i].was = pages[i].dest;
	}
}

//--------------------------------------------------------------  DoDiedGameOver

// This is called when a game is over due to the fact that the player…
// lost their last glider (died), not due to getting through the entire…
// house.

void DoDiedGameOver (void)
{
	EventRecord	theEvent;
	KeyMap		theKeys;
	long		nextLoop;
	Boolean		userAborted;
	
	userAborted = false;
	InitDiedGameOver();
	CopyRectMainToWork(&workSrcRect);
	CopyRectMainToBack(&workSrcRect);
	FlushEvents(everyEvent, 0);
	
	nextLoop = TickCount() + 2;
	while (pagesStuck < 8)
	{
		HandlePages();
		DrawPages();
		do
		{
			GetKeys(theKeys);
			if ((BitTst(&theKeys, kCommandKeyMap)) || (BitTst(&theKeys, kOptionKeyMap)) || 
					(BitTst(&theKeys, kShiftKeyMap)) || (BitTst(&theKeys, kControlKeyMap)))
			{
				pagesStuck = 8;
				userAborted = true;
			}
			if (GetNextEvent(everyEvent, &theEvent))
				if ((theEvent.what == mouseDown) || (theEvent.what == keyDown))
				{
					pagesStuck = 8;
					userAborted = true;
				}
		}
		while (TickCount() < nextLoop);
		nextLoop = TickCount() + 2;
	}
	
	if (roomRgn != nil)
		DisposeRgn(roomRgn);
	
	DisposeGWorld(pageSrcMap);
	pageSrcMap = nil;
	
	DisposeGWorld(pageMaskMap);
	pageMaskMap = nil;
	
	DisposeGWorld(gameOverSrcMap);
	gameOverSrcMap = nil;
	playing = false;
	
	if (demoGoing)
	{
		if (!userAborted)
			WaitForInputEvent(1);
	}
	else
	{
		if (!userAborted)
			WaitForInputEvent(10);
		TestHighScore();
	}
	RedrawSplashScreen();
}

