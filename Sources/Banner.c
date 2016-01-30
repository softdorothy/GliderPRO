//============================================================================
//----------------------------------------------------------------------------
//									Banner.c
//----------------------------------------------------------------------------
//============================================================================


#include <NumberFormatting.h>
#include "Externs.h"
#include "Environ.h"
#include "MainWindow.h"
#include "RectUtils.h"
#include "Room.h"
#include "Utilities.h"


#define kBannerPageTopPICT		1993
#define kBannerPageBottomPICT	1992
#define kBannerPageBottomMask	1991
#define kStarsRemainingPICT		1017
#define kStarRemainingPICT		1018


void DrawBanner (Point *);
void DrawBannerMessage (Point);


short		numStarsRemaining;
Boolean		bannerStarCountOn;

extern	Rect		justRoomsRect;
extern	Boolean		quickerTransitions, demoGoing, isUseSecondScreen;


//==============================================================  Functions
//--------------------------------------------------------------  DrawBanner
// Displays opening banner (when a new game is begun).  The banner looks…
// like a sheet of notebook paper.  The text printed on it is specified…
// by the author of the house.

void DrawBanner (Point *topLeft)
{
	CGrafPtr	wasCPort;
	GDHandle	wasWorld;
	Rect		wholePage, partPage, mapBounds;
	GWorldPtr	tempMap;
	GWorldPtr	tempMask;
	OSErr		theErr;
	
	GetGWorld(&wasCPort, &wasWorld);
	
	QSetRect(&wholePage, 0, 0, 330, 220);
	mapBounds = thisMac.screen;
	ZeroRectCorner(&mapBounds);
	CenterRectInRect(&wholePage, &mapBounds);
	topLeft->h = wholePage.left;
	topLeft->v = wholePage.top;
	partPage = wholePage;
	partPage.bottom = partPage.top + 190;	
	SetGWorld(workSrcMap, nil);
	LoadScaledGraphic(kBannerPageTopPICT, &partPage);
	
	partPage = wholePage;
	partPage.top = partPage.bottom - 30;
	mapBounds = partPage;
	ZeroRectCorner(&mapBounds);
	theErr = CreateOffScreenGWorld(&tempMap, &mapBounds, kPreferredDepth);
	SetGWorld(tempMap, nil);
	LoadGraphic(kBannerPageBottomPICT);
	
	theErr = CreateOffScreenGWorld(&tempMask, &mapBounds, 1);	
	SetGWorld(tempMask, nil);
	LoadGraphic(kBannerPageBottomMask);
	
	CopyMask((BitMap *)*GetGWorldPixMap(tempMap), 
			(BitMap *)*GetGWorldPixMap(tempMask), 
			(BitMap *)*GetGWorldPixMap(workSrcMap), 
			&mapBounds, &mapBounds, &partPage);
	SetPort((GrafPtr)workSrcMap);
	
	SetGWorld(wasCPort, wasWorld);
	DisposeGWorld(tempMap);
	DisposeGWorld(tempMask);
}

//--------------------------------------------------------------  CountStarsInHouse
// Goes through the current house and counts the total number of stars within.

short CountStarsInHouse (void)
{
	short		i, h, numRooms, numStars;
	char		wasState;
	
	numStars = 0;
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	numRooms = (*thisHouse)->nRooms;
	for (i = 0; i < numRooms; i++)
	{
		if ((*thisHouse)->rooms[i].suite != kRoomIsEmpty)
			for (h = 0; h < kMaxRoomObs; h++)
			{
				if ((*thisHouse)->rooms[i].objects[h].what == kStar)
					numStars++;
			}
	}
	HSetState((Handle)thisHouse, wasState);
	
	return (numStars);
}

//--------------------------------------------------------------  DrawBannerMessage

// This function prints the author's message acorss the notebook paper banner.

void DrawBannerMessage (Point topLeft)
{
	Str255		bannerStr, subStr;
	short		count;
	char		wasState;
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	PasStringCopy((*thisHouse)->banner, bannerStr);
	HSetState((Handle)thisHouse, wasState);
	
	TextFont(applFont);
	TextFace(bold);
	TextSize(12);
	ForeColor(blackColor);
	count = 0;
	do
	{
		GetLineOfText(bannerStr, count, subStr);
		MoveTo(topLeft.h + 16, topLeft.v + 32 + (count * 20));
		DrawString(subStr);
		count++;
	}
	while (subStr[0] > 0);
	
	if (bannerStarCountOn)
	{
		if (numStarsRemaining != 1)
			GetLocalizedString(1, bannerStr);
		else
			GetLocalizedString(2, bannerStr);
		
		NumToString((long)numStarsRemaining, subStr);
		PasStringConcat(bannerStr, subStr);
		
		if (numStarsRemaining != 1)
			GetLocalizedString(3, subStr);
		else
			GetLocalizedString(4, subStr);
		PasStringConcat(bannerStr, subStr);
		
		ForeColor(redColor);
		MoveTo(topLeft.h + 16, topLeft.v + 164);
		DrawString(bannerStr);
		MoveTo(topLeft.h + 16, topLeft.v + 180);
		GetLocalizedString(5, subStr);
		DrawString(subStr);
	}
	ForeColor(blackColor);
}

//--------------------------------------------------------------  BringUpBanner
// Handles bringing up displaying and disposing of the banner.

void BringUpBanner (void)
{
	Rect		wholePage;
	Point		topLeft;
	
	DrawBanner(&topLeft);
	DrawBannerMessage(topLeft);
//	if (quickerTransitions)
//		DissBitsChunky(&justRoomsRect);		// was workSrcRect
//	else
//		DissBits(&justRoomsRect);
	QSetRect(&wholePage, 0, 0, 330, 220);
	QOffsetRect(&wholePage, topLeft.h, topLeft.v);
	
	CopyBits((BitMap *)*GetGWorldPixMap(backSrcMap), 
			(BitMap *)*GetGWorldPixMap(workSrcMap), 
			&wholePage, &wholePage, srcCopy, nil);
	
	if (demoGoing)
		WaitForInputEvent(4);
	else
		WaitForInputEvent(15);
	
//	if (quickerTransitions)
//		DissBitsChunky(&justRoomsRect);
//	else
//		DissBits(&justRoomsRect);
}

//--------------------------------------------------------------  DisplayStarsRemaining
// This brings up a small message indicating the number of stars remaining…
// in a house.  It comes up when the player gets a star (the game is paused…
// and the user informed as to how many remain).

void DisplayStarsRemaining (void)
{
	Rect		src, bounds;
	Str255		theStr;
	
	SetPortWindowPort(mainWindow);
	QSetRect(&bounds, 0, 0, 256, 64);
	CenterRectInRect(&bounds, &thisMac.screen);
	QOffsetRect(&bounds, -thisMac.screen.left, -thisMac.screen.top);
	src = bounds;
	InsetRect(&src, 64, 32);
	
	TextFont(applFont);
	TextFace(bold);
	TextSize(12);
	NumToString((long)numStarsRemaining, theStr);
	
	QOffsetRect(&bounds, 0, -20);
	if (numStarsRemaining < 2)
		LoadScaledGraphic(kStarRemainingPICT, &bounds);
	else
	{
		LoadScaledGraphic(kStarsRemainingPICT, &bounds);
		MoveTo(bounds.left + 102 - (StringWidth(theStr) / 2), bounds.top + 23);
		ColorText(theStr, 4L);
	}
	
	DelayTicks(60);
	if (WaitForInputEvent(30))
		RestoreEntireGameScreen();
	CopyRectWorkToMain(&bounds);
}

