//============================================================================
//----------------------------------------------------------------------------
//									About.c
//----------------------------------------------------------------------------
//============================================================================


#include <NumberFormatting.h>
#include <Resources.h>
#include <Sound.h>
#include "About.h"
#include "DialogUtils.h"
#include "Environ.h"
#include "Externs.h"


static void HiLiteOkayButton (void);
static void UnHiLiteOkayButton (void);
static void UpdateMainPict (DialogPtr);
static pascal Boolean AboutFilter (DialogPtr, EventRecord *theEvent, short *hit);


static RgnHandle		okayButtRgn;
static Rect				okayButtonBounds, mainPICTBounds;
static Boolean			okayButtIsHiLit, clickedDownInOkay;


//==============================================================  Functions
//--------------------------------------------------------------  DoAbout
// Brings up the About dialog box.

void DoAbout (void)
{
	#define			kAboutDialogID			150		// res ID of About dialog
	#define			kTextItemVers			2		// item number of version text
	#define			kPictItemMain			4		// item number of main PICT
	
	DialogPtr		aboutDialog;
	Str255			longVersion;
	StringPtr		messagePtr;
	VersRecHndl		version;
	Handle			itemHandle;
	short			itemType, hit, wasResFile;
	ModalFilterUPP	aboutFilterUPP;
	
	aboutFilterUPP = NewModalFilterUPP(AboutFilter);
	
	wasResFile = CurResFile();
	UseResFile(thisMac.thisResFile);
	
	aboutDialog = GetNewDialog(kAboutDialogID, nil, (WindowRef)-1L);
//	if (aboutDialog == nil)
//		RedAlert(kErrDialogDidntLoad);
	
	version = (VersRecHndl)GetResource('vers', 1);
	if (version != nil)
	{
		messagePtr = (StringPtr)(((UInt32)&(**version).shortVersion[1]) 
				+ ((**version).shortVersion[0]));
		BlockMove((Ptr)messagePtr, &longVersion, ((UInt8)*messagePtr) + 1);
		SetDialogString(aboutDialog, kTextItemVers, longVersion);
	}
	
	GetDialogItem(aboutDialog, kOkayButton, &itemType, &itemHandle, &okayButtonBounds);
	okayButtRgn = NewRgn();					// Create diagonal button region
	OpenRgn();
	MoveTo(okayButtonBounds.left + 1, okayButtonBounds.top + 45);
	Line(44, -44);							// These lines define the region
	Line(16, 16);
	Line(-44, 44);
	Line(-16, -16);
	CloseRgn(okayButtRgn);
	okayButtIsHiLit = false;				// Initially, button is not hilit
	clickedDownInOkay = false;				// Initially, didn't click in okay button
	GetDialogItem(aboutDialog, kPictItemMain, &itemType, &itemHandle, &mainPICTBounds);
	
	do										// Loop until user wants to exit
	{
		ModalDialog(aboutFilterUPP, &hit);
	}
	while ((hit != kOkayButton) && (okayButtRgn != nil));
	
	if (okayButtRgn != nil)
		DisposeRgn(okayButtRgn);			// Clean up!
	DisposeDialog(aboutDialog);
	DisposeModalFilterUPP(aboutFilterUPP);
	
	UseResFile(wasResFile);
}

//==============================================================  Static Functions
//--------------------------------------------------------------  HiLiteOkayButton
// Draws my pseudo-button to appear as though it is clicked on.

static void HiLiteOkayButton (void)
{
	#define		kOkayButtPICTHiLit		151		// res ID of unhilit button PICT
	PicHandle	thePict;
	
	if (!okayButtIsHiLit)
	{
		thePict = GetPicture(kOkayButtPICTHiLit);
		if (thePict != nil)
		{
			DrawPicture(thePict, &okayButtonBounds);
			ReleaseResource((Handle)thePict);
			
			okayButtIsHiLit = true;
		}
	}
}

//--------------------------------------------------------------  UnHiLiteOkayButton

// Draws my pseudo-button normal (not clicked on).

static void UnHiLiteOkayButton (void)
{
	#define		kOkayButtPICTNotHiLit	150		// res ID of hilit button PICT
	PicHandle	thePict;
	
	if (okayButtIsHiLit)
	{
		thePict = GetPicture(kOkayButtPICTNotHiLit);
		if (thePict != nil)
		{
			DrawPicture(thePict, &okayButtonBounds);
			ReleaseResource((Handle)thePict);
			
			okayButtIsHiLit = false;
		}
	}
}

//--------------------------------------------------------------  UpdateMainPict
// Redraws the main graphic in the dialog (in response to an update event).

static void UpdateMainPict (DialogPtr theDial)
{
	Str255		theStr, theStr2;
	long		totalSize, contigSize;
	
	DrawDialog(theDial);
	
	PasStringCopy("\pMemory:   ", theStr);		// display free memory
	PurgeSpace(&totalSize, &contigSize);
	totalSize /= 1024;
	NumToString(totalSize, theStr2);
	PasStringConcat(theStr, theStr2);
	PasStringConcat(theStr, "\pK");
	DrawDialogUserText2(theDial, 7, theStr);
	
	PasStringCopy("\pScreen:   ", theStr);		// display screen size/depth
	NumToString((long)(thisMac.screen.right - thisMac.screen.left), theStr2);
	PasStringConcat(theStr, theStr2);
	PasStringConcat(theStr, "\px");
	NumToString((long)(thisMac.screen.bottom - thisMac.screen.top), theStr2);
	PasStringConcat(theStr, theStr2);
	PasStringConcat(theStr, "\px");
	NumToString((long)thisMac.isDepth, theStr2);
	PasStringConcat(theStr, theStr2);
	DrawDialogUserText2(theDial, 8, theStr);
}

//--------------------------------------------------------------  AboutFilter
// Dialog filter for the About dialog.

static pascal Boolean AboutFilter (DialogPtr theDial, EventRecord *theEvent, short *hit)
{
	Point		mousePt;
	UInt32		dummyLong;
	Boolean		handledIt;
	
	if (Button() && clickedDownInOkay)
	{
		GetMouse(&mousePt);
		if(PtInRgn(mousePt, okayButtRgn))
			HiLiteOkayButton();
		else
			UnHiLiteOkayButton();
	}
	
	switch (theEvent->what)
	{
		case keyDown:
		switch ((theEvent->message) & charCodeMask)
		{
			case kReturnKeyASCII:
			case kEnterKeyASCII:
			HiLiteOkayButton();
			Delay(8, &dummyLong);
			UnHiLiteOkayButton();
			*hit = kOkayButton;
			handledIt = true;
			break;
			
			default:
			handledIt = false;
		}
		break;
		
		case mouseDown:
		mousePt = theEvent->where;
		GlobalToLocal(&mousePt);
		if(PtInRgn(mousePt, okayButtRgn))
		{
			clickedDownInOkay = true;
			handledIt = false;
		}
		else
			handledIt = false;
		break;
		
		case mouseUp:
		mousePt = theEvent->where;
		GlobalToLocal(&mousePt);
		if(PtInRgn(mousePt, okayButtRgn) && clickedDownInOkay)
		{
			UnHiLiteOkayButton();
			*hit = kOkayButton;
			handledIt = true;
		}
		else
		{
			clickedDownInOkay = false;
			handledIt = false;
		}
		break;
		
		case updateEvt:
		if ((WindowPtr)theEvent->message == mainWindow)
		{
			SetPort((GrafPtr)mainWindow);
			BeginUpdate((WindowPtr)theEvent->message);
			UpdateMainWindow();
			EndUpdate((WindowPtr)theEvent->message);
			SetPortDialogPort(theDial);
			handledIt = true;
		}
		else if ((WindowPtr)theEvent->message == (WindowPtr)theDial)
		{
			SetPortDialogPort(theDial);
			BeginUpdate((WindowPtr)theEvent->message);
			UpdateMainPict(theDial);
			EndUpdate((WindowPtr)theEvent->message);
			handledIt = false;
		}
		break;
		
		default:
		handledIt = false;
		break;
	}
	
	return (handledIt);
}

