/*============================================================*/
/*============================================================*/
/*==														==*/
/*==				Debugging Utility Routines				==*/
/*==														==*/
/*============================================================*/
/*============================================================*/


#include <NumberFormatting.h>
#include "Externs.h"


short			barGraphHori = 0;


//==============================================================  Functions
//--------------------------------------------------------------  MonitorWait

void MonitorWait (void)
{
	GrafPtr			wasPort, tempPort;
	Rect			tempRect;
	
	GetPort(&wasPort);
	
	tempPort = (GrafPtr)NewPtrClear(sizeof(GrafPort));
	OpenPort(tempPort);
	
	SetRect(&tempRect, 8, 28, 16, 36);
	InvertRect(&tempRect);
	
	ClosePort(tempPort);
	
	SetPort((GrafPtr)wasPort);
}

//--------------------------------------------------------------  DisplayRect

void DisplayRect (Rect *theRect)
{
	GrafPtr			wasPort, tempPort;
	
	GetPort(&wasPort);
	
	tempPort = (GrafPtr)NewPtrClear(sizeof(GrafPort));
	OpenPort(tempPort);
	
	InvertRect(theRect);
	
	ClosePort(tempPort);
	SetPort((GrafPtr)wasPort);
}


//--------------------------------------------------------------  FlashRect

void FlashRect (Rect *theRect)

{
	GrafPtr			wasPort, tempPort;
	
	GetPort(&wasPort);
	
	tempPort = (GrafPtr)NewPtrClear(sizeof(GrafPort));
	OpenPort(tempPort);
	
	InvertRect(theRect);
	InvertRect(theRect);
	InvertRect(theRect);
	InvertRect(theRect);
	
	ClosePort(tempPort);
	SetPort((GrafPtr)wasPort);
}

//--------------------------------------------------------------  CheckLegitRect

void CheckLegitRect(Rect *srcRect, Rect *inRect)
{
	Rect			dummyRect;
	
	if ((srcRect->left > srcRect->right) || (srcRect->top > srcRect->bottom))
		DebugStr("\pSource Rectangle not dimensional");
	
	if (!SectRect(srcRect, inRect, &dummyRect))
		DebugStr("\pSource Rectangle not Secting Target Rectangle");
}


/*==============================================================  DisplayLong  */

void DisplayLong (long theValue)

{
	GrafPtr			wasPort, tempPort;
	Str255			tempStr;
	Rect			tempRect;
	
	GetPort(&wasPort);
	
	tempPort = (GrafPtr)NewPtrClear(sizeof(GrafPort));
	OpenPort(tempPort);
	
	NumToString(theValue, tempStr);
	MoveTo(20,40);
	SetRect(&tempRect, 18, 20, 122, 42);
	EraseRect(&tempRect);
	DrawString(tempStr);
	
	while (Button())
	{
	}
	while (!Button())
	{
	}
	
	ClosePort(tempPort);
	SetPort((GrafPtr)wasPort);
}


/*==============================================================  DisplayShort  */

void DisplayShort(short theValue)

{
	GrafPtr			wasPort, tempPort;
	Str255			tempStr;
	Rect			tempRect;
	
	GetPort(&wasPort);
	
	tempPort = (GrafPtr)NewPtrClear(sizeof(GrafPort));
	OpenPort(tempPort);
	
	NumToString((long)theValue, tempStr);
	MoveTo(20,40);
	SetRect(&tempRect, 18, 20, 122, 42);
	EraseRect(&tempRect);
	DrawString(tempStr);
	
	while (Button())
	{
	}
	while (!Button())
	{
	}
	
	ClosePort(tempPort);
	SetPort((GrafPtr)wasPort);
}


/*==============================================================  FlashLong  */

void FlashLong(long theValue)
{
	GrafPtr			wasPort, tempPort;
	Str255			tempStr;
	Rect			tempRect;
	
	GetPort(&wasPort);
	
	tempPort = (GrafPtr)NewPtrClear(sizeof(GrafPort));
	OpenPort(tempPort);
	
	NumToString(theValue, tempStr);
	MoveTo(20,40);
	SetRect(&tempRect, 18, 20, 122, 42);
	EraseRect(&tempRect);
	DrawString(tempStr);
	
	ClosePort(tempPort);
	SetPort((GrafPtr)wasPort);
}


/*==============================================================  FlashShort  */

void FlashShort (short theValue)
{
	GrafPtr			wasPort, tempPort;
	Str255			tempStr;
	Rect			tempRect;
	
	GetPort(&wasPort);
	
	tempPort = (GrafPtr)NewPtrClear(sizeof(GrafPort));
	OpenPort(tempPort);
	
	NumToString((long)theValue, tempStr);
	MoveTo(20,40);
	SetRect(&tempRect, 18, 20, 122, 42);
	EraseRect(&tempRect);
	DrawString(tempStr);
	
	ClosePort(tempPort);
	SetPort((GrafPtr)wasPort);
}


/*==============================================================  DoBarGraph  */

void DoBarGraph (short theValue, short downScreen, short maxDown, short scaleIt)

{
	GrafPtr			wasPort, tempPort;
	
	GetPort(&wasPort);
	
	tempPort = (GrafPtr)NewPtrClear(sizeof(GrafPort));
	OpenPort(tempPort);
	
	theValue *= scaleIt;
	
	PenPat(&qd.white);
	MoveTo(barGraphHori, 0);
	Line(0, maxDown);
	MoveTo(barGraphHori, downScreen);
	PenPat(&qd.black);
	if (theValue == 0)
	{
		theValue = 1;
		ForeColor(greenColor);
	}
	else if (theValue < 0)
	{
		if (theValue < -downScreen)
		{
			theValue = -downScreen;
			ForeColor(magentaColor);
		}
		else
			ForeColor(redColor);
	}
	else
	{
		if (theValue > downScreen)
		{
			ForeColor(cyanColor);
			theValue = downScreen;
		}
		else
			ForeColor(blueColor);
	}
	Line(0, -theValue);
	ForeColor(blackColor);
	
	barGraphHori++;
	if (barGraphHori >= 512)
		barGraphHori = 0;
	
	ClosePort(tempPort);
	SetPort((GrafPtr)wasPort);
}


/*==============================================================  BetaOkay  */

short BetaOkay (void)

{
	DateTimeRec		dateRecord;
	UInt32	theseSeconds;
	Boolean			stillCool;
	
	GetDateTime(&theseSeconds);
	SecondsToDate(theseSeconds, &dateRecord);
	
	if (dateRecord.month < 8)
		stillCool = true;
	else
		stillCool = false;
	
	return((short)stillCool);
}

//--------------------------------------------------------------  DebugNum

void DebugNum (long theNum)
{
	Str255		theStr;
	NumToString(theNum, theStr);
	DebugStr(theStr);
}

//--------------------------------------------------------------  DisplayCTSeed

void DisplayCTSeed (CGrafPtr who)
{
	long		theSeed;
	
	theSeed = (**((**(who->portPixMap)).pmTable)).ctSeed;
	DisplayLong(theSeed);
}

//--------------------------------------------------------------  FillScreenRed

void FillScreenRed (void)
{
	GrafPtr			wasPort, tempPort;
	
	GetPort(&wasPort);
	tempPort = (GrafPtr)NewPtrClear(sizeof(GrafPort));
	OpenPort(tempPort);
	PenNormal();
	ForeColor(redColor);
	PaintRect(&qd.screenBits.bounds);
	ClosePort(tempPort);
	SetPort((GrafPtr)wasPort);
}

//--------------------------------------------------------------  DumpToResEditFile

void DumpToResEditFile (Ptr data, long dataSize)
{
	DateTimeRec		timeRec;
	Str32			filesName, dateStr;
	Handle			newResource;
	UInt32			dateTime;
	long			tempLong;
	OSErr			theErr;
	short			iFileRef;
	
	PasStringCopy("\pTerrain ", filesName);
	
	GetDateTime(&dateTime);
	SecondsToDate(dateTime, &timeRec);
	tempLong = (long)timeRec.hour;
	NumToString(tempLong, dateStr);
	PasStringConcat(filesName, dateStr);
	PasStringConcat(filesName, "\p-");
	tempLong = (long)timeRec.minute;
	NumToString(tempLong, dateStr);
	PasStringConcat(filesName, dateStr);
	
	theErr = Create(filesName, 0, 'RSED', 'rsrc');
	if (theErr != noErr)
		DebugStr("\p Create");
	
	CreateResFile(filesName);
	if (ResError() != noErr)
		DebugStr("\p CreateResFile");
	
	iFileRef = OpenResFile(filesName);
	if ((ResError() != noErr) || (iFileRef == -1))
		DebugStr("\p OpenResFile");
	
	if (PtrToHand(data, &newResource, dataSize) != noErr)
		DebugStr("\pPtrToHand");
	AddResource(newResource, 'demo', 128, "\p");
	ChangedResource(newResource);
}

