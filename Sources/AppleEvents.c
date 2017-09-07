
//============================================================================
//----------------------------------------------------------------------------
//								  AppleEvents.c
//----------------------------------------------------------------------------
//============================================================================


#include "Externs.h"
#include <AppleEvents.h>
#include "House.h"


#define kNoPrintingAlert			1031


pascal OSErr DoOpenAppAE (const AppleEvent *, AppleEvent *, UInt32);
pascal OSErr DoOpenDocAE (const AppleEvent *, AppleEvent *, UInt32);
pascal OSErr DoPrintDocAE (const AppleEvent *, AppleEvent *, UInt32);
pascal OSErr DoQuitAE (const AppleEvent *, AppleEvent *, UInt32);
pascal OSErr MyGotRequiredParams (const AppleEvent *);


AEEventHandlerUPP	openAppAEUPP, openDocAEUPP, printDocAEUPP, quitAEUPP;


extern	FSSpecPtr	theHousesSpecs;
extern	long		incrementModeTime;
extern	short		thisHouseIndex, splashOriginH, splashOriginV;
extern	Boolean		quitting;


//==============================================================  Functions
//--------------------------------------------------------------  DoOpenAppAE
// Handles an "Open Application" Apple Event.

pascal OSErr DoOpenAppAE (const AppleEvent *theAE, AppleEvent *reply, UInt32 ref)
{
#pragma unused (reply, ref)
	OSErr		theErr;
	
	theErr = MyGotRequiredParams(theAE);
	return (theErr);
}

//--------------------------------------------------------------  DoOpenDocAE
// Handles an "Open Document" Apple Event.

pascal OSErr DoOpenDocAE (const AppleEvent *theAE, AppleEvent *reply, UInt32 ref)
{
#pragma unused (reply, ref)
	FSSpec			oneFSS;
	FInfo			finderInfo;
	AEDescList		docList;
	long			itemsInList;
	Size			actualSize;
	AEKeyword		keywd;
	DescType		returnedType;
	OSErr			theErr, whoCares;
	short			i;
	
	theErr = AEGetParamDesc(theAE, keyDirectObject, typeAEList, &docList);
	if (theErr != noErr)
	{
		YellowAlert(kYellowAppleEventErr, theErr);
		return (theErr);
	}
	
	theErr = MyGotRequiredParams(theAE);
	if (theErr != noErr)
	{
		whoCares = AEDisposeDesc(&docList);
		return (theErr);
	}
	
	theErr = AECountItems(&docList, &itemsInList);
	if (theErr != noErr)
	{
		whoCares = AEDisposeDesc(&docList);
		return (theErr);
	}
	
#ifndef COMPILEDEMO	
	for (i = 1; i <= itemsInList; i++)
	{
		theErr = AEGetNthPtr(&docList, i, typeFSS, &keywd, &returnedType,
				&oneFSS, sizeof(oneFSS), &actualSize);
		if (theErr == noErr)
		{
			theErr = FSpGetFInfo(&oneFSS, &finderInfo);
			if ((theErr == noErr) && (finderInfo.fdType == 'gliH'))
				AddExtraHouse(&oneFSS);
		}
	}
	if (itemsInList > 0)
	{
		theErr = AEGetNthPtr(&docList, 1, typeFSS, &keywd, &returnedType,
				&oneFSS, sizeof(oneFSS), &actualSize);
		if (theErr == noErr)
		{
			theErr = FSpGetFInfo(&oneFSS, &finderInfo);
			if ((theErr == noErr) && (finderInfo.fdType == 'gliH'))
			{
				whoCares = CloseHouse();
				PasStringCopy(oneFSS.name, thisHouseName);
				BuildHouseList();
				if (OpenHouse())
					whoCares = ReadHouse();
				PasStringCopy(theHousesSpecs[thisHouseIndex].name, thisHouseName);
				OpenCloseEditWindows();
				incrementModeTime = TickCount() + kIdleSplashTicks;
				if ((theMode == kSplashMode) || (theMode == kPlayMode))
				{
					Rect		updateRect;
					
					SetRect(&updateRect, splashOriginH + 474, splashOriginV + 304, splashOriginH + 474 + 166, splashOriginV + 304 + 12);
					InvalWindowRect(mainWindow, &updateRect);
				}
			}
		}
		InitCursor();
	}
#endif
	whoCares = AEDisposeDesc(&docList);
	
	return theErr;
}

//--------------------------------------------------------------  DoPrintDocAE
// Handles a "Print Document" Apple Event.

pascal OSErr DoPrintDocAE (const AppleEvent *theAE, AppleEvent *reply, UInt32 ref)
{
#pragma unused (theAE, reply, ref)

	short		hitWhat;
	
//	CenterAlert(kNoPrintingAlert);
	hitWhat = Alert(kNoPrintingAlert, nil);
	
	return errAEEventNotHandled;
}

//--------------------------------------------------------------  DoQuitAE
// Handles a "Quit Application" Apple Event.

pascal OSErr DoQuitAE (const AppleEvent *theAE, AppleEvent *reply, UInt32 ref)
{
#pragma unused (reply, ref)
	OSErr			isHuman;
	
	isHuman = MyGotRequiredParams(theAE);
	if (isHuman == noErr)
		quitting = true;
	
	return isHuman;
}

//--------------------------------------------------------------  MyGotRequiredParams
// Have no clue!  :)

pascal OSErr MyGotRequiredParams (const AppleEvent *theAE)
{
	DescType		returnedType;
	Size			actualSize;
	
	return (AEGetAttributePtr(theAE, keyMissedKeywordAttr, typeWildCard,
			&returnedType, 0L, 0, &actualSize) == errAEDescNotFound) ? noErr :
			errAEParamMissed;
}

//--------------------------------------------------------------  SetUpAppleEvents
// Initializes all handlers, etc. for dealing with Apple Events.

void SetUpAppleEvents (void)
{
	OSErr		theErr;
	
	openAppAEUPP = NewAEEventHandlerProc(DoOpenAppAE);
	openDocAEUPP = NewAEEventHandlerProc(DoOpenDocAE);
	printDocAEUPP = NewAEEventHandlerProc(DoPrintDocAE);
	quitAEUPP = NewAEEventHandlerProc(DoQuitAE);
	
	theErr = AEInstallEventHandler(kCoreEventClass,		// install oapp 
			kAEOpenApplication, openAppAEUPP, 0, false);
	if (theErr != noErr)
		YellowAlert(kYellowAppleEventErr, theErr);
	
	theErr = AEInstallEventHandler(kCoreEventClass, 	// install odoc 
			kAEOpenDocuments, openDocAEUPP, 0, false);
	if (theErr != noErr)
		YellowAlert(kYellowAppleEventErr, theErr);
	
	theErr = AEInstallEventHandler(kCoreEventClass, 	// install pdoc 
			kAEPrintDocuments, printDocAEUPP, 0, false);
	if (theErr != noErr)
		YellowAlert(kYellowAppleEventErr, theErr);
	
	theErr = AEInstallEventHandler(kCoreEventClass, 	// install quit 
			kAEQuitApplication, quitAEUPP, 0, false);
	if (theErr != noErr)
		YellowAlert(kYellowAppleEventErr, theErr);
	
	theErr = AESetInteractionAllowed(kAEInteractWithAll);
	if (theErr != noErr)
		YellowAlert(kYellowAppleEventErr, theErr);
}

