//============================================================================
//----------------------------------------------------------------------------
//								  Utilities.c
//----------------------------------------------------------------------------
//============================================================================


#include <Icons.h>
#include <NumberFormatting.h>
#include <Resources.h>
#include <Sound.h>
#include <TextUtils.h>
#include <ToolUtils.h>
#include "Externs.h"
#include "Utilities.h"


GDHandle	thisGDevice;
UInt32		theSeed;


extern	Boolean		switchedOut;


//==============================================================  Functions
//--------------------------------------------------------------  MyGetGlobalMouse
// Returns the position of the mouse in global coordinates.

Point MyGetGlobalMouse (void)
{
	Point	localWhere;
	
	GetMouse(&localWhere);
	LocalToGlobal(&localWhere);
	return (localWhere);
}

//--------------------------------------------------------------  ToolBoxInit

// The standard ToolBox intialization that must happen when any Mac…
// program first launches.

void ToolBoxInit (void)
{
#if !TARGET_CARBON
	InitGraf(&qd.thePort);
	InitFonts();
	FlushEvents(everyEvent, 0);
	InitWindows();
	InitMenus();
	TEInit();
	InitDialogs(nil);
	
	MaxApplZone();
	
	MoreMasters();
	MoreMasters();
	MoreMasters();
	MoreMasters();
	
	GetDateTime((UInt32 *)&qd.randSeed);
	
#endif
	
	InitCursor();
	switchedOut = false;
}

//--------------------------------------------------------------  RandomInt
// Returns a random integer (short) within "range".

short RandomInt (short range)
{
	register long	rawResult;
	
	rawResult = Random();
	if (rawResult < 0L)
		rawResult *= -1L;
	rawResult = (rawResult * (long)range) / 32768L;
	
	return ((short)rawResult);
}

//--------------------------------------------------------------  RandomLong

// Returns a random long interger within "range".

long RandomLong (long range)
{
	register long	highWord, lowWord;
	register long	rawResultHi, rawResultLo;
	
	highWord = (range & 0xFFFF0000) >> 16;
	lowWord = range & 0x0000FFFF;
	
	rawResultHi = Random();
	if (rawResultHi < 0L)
		rawResultHi *= -1L;
	rawResultHi = (rawResultHi * highWord) / 32768L;
	
	rawResultLo = Random();
	if (rawResultLo < 0L)
		rawResultLo *= -1L;
	rawResultLo = (rawResultLo * lowWord) / 32768L;
	
	rawResultHi = (rawResultHi << 16) + rawResultLo;
	
	return (rawResultHi);
}

//--------------------------------------------------------------  InitRandomLongQUS

// Initializes random seed for quick & dirty long random number function (below).

void InitRandomLongQUS (void)
{
	GetDateTime(&theSeed);
}

//--------------------------------------------------------------  RandomLongQUS

// Very simple (but fast) pseudo-random number generator.

UInt32 RandomLongQUS (void)
{
	theSeed = theSeed * 1103515245 + 12345;
	return (theSeed);
}

//--------------------------------------------------------------  RedAlert

// Called when we must quit app.  Brings up a dialog informing user…
// of the problem and the exits to shell.

void RedAlert (short errorNumber)
{
	#define			rDeathAlertID	170		// alert res. ID for death error
	#define			rErrTitleID		170		// string ID for death error title
	#define			rErrMssgID		171		// string ID for death error message
	short			dummyInt;
	Str255			errTitle, errMessage, errNumberString;
	
	InitCursor();
	
	if (errorNumber > 1)		// <= 0 is unaccounted for
	{
		GetIndString(errTitle, rErrTitleID, errorNumber);
		GetIndString(errMessage, rErrMssgID, errorNumber);
	}
	else
	{
		GetIndString(errTitle, rErrTitleID, 1);
		GetIndString(errMessage, rErrMssgID, 1);
	}
	NumToString((long)errorNumber, errNumberString);
	ParamText(errTitle, errMessage, errNumberString, "\p");
//	CenterAlert(rDeathAlertID);
	
	dummyInt = Alert(rDeathAlertID, nil);
	ExitToShell();
}

//--------------------------------------------------------------  FindOurDevice

// Finds the main device (monitor with the menu bar on it).

void FindOurDevice (void)
{
	thisGDevice = GetMainDevice();
	if (thisGDevice == nil)
		RedAlert(kErrFailedGetDevice);
}

//--------------------------------------------------------------  CreateOffScreenBitMap
// Creates an offscreen bit map (b&w - 1 bit depth).

/*
void CreateOffScreenBitMap (Rect *theRect, GrafPtr *offScreen)
{
	GrafPtr		theBWPort;
	BitMap		theBitMap;	
	long		theRowBytes;
	
	theBWPort = (GrafPtr)(NewPtr(sizeof(GrafPort)));
	OpenPort(theBWPort);
	theRowBytes = (long)((theRect->right - theRect->left + 15L) / 16L) * 2L;
	theBitMap.rowBytes = (short)theRowBytes;
	theBitMap.baseAddr = NewPtr((long)theBitMap.rowBytes * 
		(theRect->bottom - theRect->top));
	if (theBitMap.baseAddr == nil)
		RedAlert(kErrNoMemory);
	theBitMap.bounds = *theRect;
	if (MemError() != noErr)
		RedAlert(kErrNoMemory);
	SetPortBits(&theBitMap);
	ClipRect(theRect);
	RectRgn(theBWPort->visRgn, theRect);
	EraseRect(theRect);
	*offScreen = theBWPort;
}
*/
//--------------------------------------------------------------  CreateOffScreenPixMap
// Creates an offscreen pix map using the depth of the current device.
/*
void CreateOffScreenPixMap (Rect *theRect, CGrafPtr *offScreen)
{
	CTabHandle	thisColorTable;
	GDHandle	oldDevice;
	CGrafPtr	newCGrafPtr;
	Ptr			theseBits;
	long		sizeOfOff, offRowBytes;
	OSErr		theErr;
	short		thisDepth;
	char		wasState;
	
	oldDevice = GetGDevice();
	SetGDevice(thisGDevice);
	newCGrafPtr = nil;
	newCGrafPtr = (CGrafPtr)NewPtr(sizeof(CGrafPort));
	if (newCGrafPtr != nil)
	{
		OpenCPort(newCGrafPtr);
		thisDepth = (**(*newCGrafPtr).portPixMap).pixelSize;
		offRowBytes = ((((long)thisDepth * 
				(long)(theRect->right - theRect->left)) + 15L) >> 4L) << 1L;
		sizeOfOff = (long)(theRect->bottom - theRect->top + 1) * offRowBytes;
	//	sizeOfOff = (long)(theRect->bottom - theRect->top) * offRowBytes;
		OffsetRect(theRect, -theRect->left, -theRect->top);
		theseBits = NewPtr(sizeOfOff);
		if (theseBits != nil)
		{								// workaround
			(**(*newCGrafPtr).portPixMap).baseAddr = theseBits + offRowBytes;
		//	(**(*newCGrafPtr).portPixMap).baseAddr = theseBits;
			(**(*newCGrafPtr).portPixMap).rowBytes = (short)offRowBytes + 0x8000;
			(**(*newCGrafPtr).portPixMap).bounds = *theRect;
			wasState = HGetState((Handle)thisGDevice);
			HLock((Handle)thisGDevice);
			thisColorTable = (**(**thisGDevice).gdPMap).pmTable;
			HSetState((Handle)thisGDevice, wasState);
			theErr = HandToHand((Handle *)&thisColorTable);
			(**(*newCGrafPtr).portPixMap).pmTable = thisColorTable;
			ClipRect(theRect);
			RectRgn(newCGrafPtr->visRgn, theRect);
			ForeColor(blackColor);
			BackColor(whiteColor);
			EraseRect(theRect);
		}
		else
		{
			CloseCPort(newCGrafPtr);		
			DisposePtr((Ptr)newCGrafPtr);
			newCGrafPtr = nil;
			RedAlert(kErrNoMemory);
		}
	}
	else
		RedAlert(kErrNoMemory);
	
	*offScreen = newCGrafPtr;
	SetGDevice(oldDevice);
}
*/
//--------------------------------------------------------------------  CreateOffScreenGWorld
// Creates an offscreen GWorld using the depth passed in.

OSErr CreateOffScreenGWorld (GWorldPtr *theGWorld, Rect *bounds, short depth)
{
	OSErr		theErr;
	
	theErr = NewGWorld(theGWorld, depth, bounds, nil, nil, useTempMem);
	
	if (theErr)
		theErr = NewGWorld(theGWorld, depth, bounds, nil, nil, 0);
	
	LockPixels(GetGWorldPixMap(*theGWorld));
	
	return theErr;
}


//--------------------------------------------------------------  KillOffScreenPixMap
// Destroys memory allocated by an offscreen pix map.
/*
void KillOffScreenPixMap (CGrafPtr offScreen)
{
	Ptr		theseBits;
	
	if (offScreen != nil)
	{
		theseBits = (**(*offScreen).portPixMap).baseAddr;
		theseBits -= (**(*offScreen).portPixMap).rowBytes & 0x7FFF;	// workaround
		DisposePtr(theseBits);
		DisposeHandle((Handle)(**(*offScreen).portPixMap).pmTable);
		CloseCPort(offScreen);
		DisposePtr((Ptr)offScreen);
	}
}
*/
//--------------------------------------------------------------  KillOffScreenBitMap
// Destroys memory allocated by an offscreen bit map.
/*
void KillOffScreenBitMap (GrafPtr offScreen)
{
	if (offScreen != nil)
	{
		DisposePtr((Ptr)(offScreen->portBits.baseAddr));
		ClosePort(offScreen);
		DisposePtr((Ptr)offScreen);
	}
}
*/
//--------------------------------------------------------------  LoadGraphic
// Function loads the specified 'PICT' from disk and draws it to…
// the current port (no scaling, clipping, etc, are done).  Always…
// draws in the upper left corner of current port.

void LoadGraphic (short resID)
{
	Rect		bounds;
	PicHandle	thePicture;
	
	thePicture = GetPicture(resID);
	if (thePicture == nil)
		RedAlert(kErrFailedGraphicLoad);
	
	HLock((Handle)thePicture);
	bounds = (*thePicture)->picFrame;
	HUnlock((Handle)thePicture);
	OffsetRect(&bounds, -bounds.left, -bounds.top);
	DrawPicture(thePicture, &bounds);
	
	ReleaseResource((Handle)thePicture);
}

//--------------------------------------------------------------  LoadScaledGraphic
// Loads the specified 'PICT' and draws it mapped to the rectangle…
// specified.  If this rect isn't the same size of the 'PICT', scaling…
// will occur.

void LoadScaledGraphic (short resID, Rect *theRect)
{
	PicHandle	thePicture;
	
	thePicture = GetPicture(resID);
	if (thePicture == nil)
		RedAlert(kErrFailedGraphicLoad);
	DrawPicture(thePicture, theRect);
	ReleaseResource((Handle)thePicture);
}

//--------------------------------------------------------------  PlotSICN
// Draws a small icon (16 x 16 pixels).
/*
void PlotSICN (Rect *theRect, SICNHand theSICN, long theIndex)
{
	char		state;
	BitMap		srcBits;
	
	if ((theSICN != nil) && 
			((GetHandleSize((Handle)theSICN) / sizeof(SICN)) > theIndex))
	{
		state = HGetState((Handle)theSICN);
		HLock((Handle)theSICN);
		
		srcBits.baseAddr = (Ptr)(*theSICN)[theIndex];
		srcBits.rowBytes = 2;
		SetRect(&srcBits.bounds, 0, 0, 16, 16);
		
		CopyBits(&srcBits,&(*qd.thePort).portBits, 
				&srcBits.bounds, theRect, srcCopy, nil);
		
		HSetState((Handle) theSICN, state);
	}
}
*/
//--------------------------------------------------------------  LargeIconPlot
// Draws a standard b&w icon (32 x 32) - resource is an 'ICON'.

void LargeIconPlot (Rect *theRect, short theID)
{
	OSErr		theErr;
	Handle		theSuite;
	
	theErr = GetIconSuite(&theSuite, theID, svAllLargeData);
	if (theErr == noErr)
		theErr = PlotIconSuite(theRect, atNone, ttNone, theSuite);
}

//--------------------------------------------------------------  DrawCIcon

// Draws a standard color icon (32 x 32) - resource is a 'CICN'.

void DrawCIcon (short theID, short h, short v)
{
	CIconHandle	theIcon;
	Rect		theRect;
	
	theIcon = GetCIcon(theID);
	if (theIcon != nil)
	{
		SetRect(&theRect, 0, 0, 32, 32);
		OffsetRect(&theRect, h, v);
		PlotCIcon(&theRect, theIcon);
		DisposeCIcon(theIcon);
	}
}

//--------------------------------------------------------------  LongSquareRoot

// This is a quick and dirty square root function that returns pretty…
// accurate long integer results.  It uses no transcendental functions or…
// floating point.

long LongSquareRoot (long theNumber)
{
	long		currentAnswer;
	long		nextTrial;
	
	if (theNumber <= 1L)
		return (theNumber);
	
	nextTrial = theNumber / 2;
	
	do
	{
		currentAnswer = nextTrial;
		nextTrial = (nextTrial + theNumber / nextTrial) / 2;
	}
	while (nextTrial < currentAnswer);
	
	return(currentAnswer);
}

//--------------------------------------------------------------  WaitForInputEvent

// Wait for either a key to be hit or the mouse button to be clicked.
// Also has a "timeout" parameter ("seconds").

Boolean WaitForInputEvent (short seconds)
{
	EventRecord	theEvent;
	KeyMap		theKeys;
	long		timeToBail;
	Boolean		waiting, didResume;
	
	timeToBail = TickCount() + 60L * (long)seconds;
	FlushEvents(everyEvent, 0);
	waiting = true;
	didResume = false;
	
	while (waiting)
	{
		GetKeys(theKeys);
		if ((BitTst(&theKeys, kCommandKeyMap)) || (BitTst(&theKeys, kOptionKeyMap)) || 
				(BitTst(&theKeys, kShiftKeyMap)) || (BitTst(&theKeys, kControlKeyMap)))
			waiting = false;
		if (GetNextEvent(everyEvent, &theEvent))
		{
			if ((theEvent.what == mouseDown) || (theEvent.what == keyDown))
				waiting = false;
			else if ((theEvent.what == osEvt) && (theEvent.message & 0x01000000)) 
			{
				if (theEvent.message & 0x00000001)		// resuming
				{
					didResume = true;
					waiting = false;
				}
				else									// suspending
				{
					InitCursor();
				}
			}
		}
		if ((seconds != -1) && (TickCount() >= timeToBail))
			waiting = false;
	}
	FlushEvents(everyEvent, 0);
	return (didResume);
}

//--------------------------------------------------------------  WaitCommandQReleased

// Waits until the Command-Q key combination is released.

void WaitCommandQReleased (void)
{
	KeyMap		theKeys;
	Boolean		waiting;
	
	waiting = true;
	
	while (waiting)
	{
		GetKeys(theKeys);
		if ((!BitTst(&theKeys, kCommandKeyMap)) || (!BitTst(&theKeys, kQKeyMap)))
			waiting = false;
	}
	FlushEvents(everyEvent, 0);
}

//--------------------------------------------------------------  KeyMapOffsetFromRawKey
// Converts a raw key code to keymap offset (ugly stuff).

char KeyMapOffsetFromRawKey (char rawKeyCode)
{
	char		hiByte, loByte, theOffset;
	
	hiByte = rawKeyCode & 0xF0;
	loByte = rawKeyCode & 0x0F;
	
	if (loByte <= 0x07)
		theOffset = hiByte + (0x07 - loByte);
	else
		theOffset = hiByte + (0x17 - loByte);
	
	return (theOffset);
}

//--------------------------------------------------------------  GetKeyMapFromMessage
// Gets the key map offset from a keyDown event's message field.

char GetKeyMapFromMessage (long message)
{
	long		theVirtual;
	char		offset;
	
	theVirtual = (message & keyCodeMask) >> 8;
	offset = KeyMapOffsetFromRawKey((char)theVirtual);
	return (offset);
}

//--------------------------------------------------------------  GetKeyName
// Given a keyDown event (it's message field), this function returns…
// a string with that key's name (so we get "Shift" and "Esc", etc.).

void GetKeyName (long message, StringPtr theName)
{
	long		theASCII, theVirtual;
	
	theASCII = message & charCodeMask;
	theVirtual = (message & keyCodeMask) >> 8;
	
	if ((theASCII >= kExclamationASCII) && (theASCII <= kZKeyASCII))
	{
		
		if ((theVirtual >= 0x0041) && (theVirtual <= 0x005C))
		{
			PasStringCopy("\p( )", theName);
			theName[2] = (char)theASCII;
		}
		else
		{
			PasStringCopy("\p  key", theName);
			theName[1] = (char)theASCII;
		}
	}
	else
	{
		switch (theASCII)
		{
			case kHomeKeyASCII:
			PasStringCopy("\phome", theName);
			break;
			
			case kEnterKeyASCII:
			PasStringCopy("\penter", theName);
			break;
			
			case kEndKeyASCII:
			PasStringCopy("\pend", theName);
			break;
			
			case kHelpKeyASCII:
			PasStringCopy("\phelp", theName);
			break;
			
			case kDeleteKeyASCII:
			PasStringCopy("\pdelete", theName);
			break;
			
			case kTabKeyASCII:
			PasStringCopy("\ptab", theName);
			break;
			
			case kPageUpKeyASCII:
			PasStringCopy("\ppg up", theName);
			break;
			
			case kPageDownKeyASCII:
			PasStringCopy("\ppg dn", theName);
			break;
			
			case kReturnKeyASCII:
			PasStringCopy("\preturn", theName);
			break;
			
			case kFunctionKeyASCII:
			switch (theVirtual)
			{
				case 0x0060:
				PasStringCopy("\pF5", theName);
				break;
				case 0x0061:
				PasStringCopy("\pF6", theName);
				break;
				case 0x0062:
				PasStringCopy("\pF7", theName);
				break;
				case 0x0063:
				PasStringCopy("\pF3", theName);
				break;
				case 0x0064:
				PasStringCopy("\pF8", theName);
				break;
				case 0x0065:
				PasStringCopy("\pF9", theName);
				break;
				case 0x0067:
				PasStringCopy("\pF11", theName);
				break;
				case 0x0069:
				PasStringCopy("\pF13", theName);
				break;
				case 0x006B:
				PasStringCopy("\pF14", theName);
				break;
				case 0x006D:
				PasStringCopy("\pF10", theName);
				break;
				case 0x006F:
				PasStringCopy("\pF12", theName);
				break;
				case 0x0071:
				PasStringCopy("\pF15", theName);
				break;
				case 0x0076:
				PasStringCopy("\pF4", theName);
				break;
				case 0x0078:
				PasStringCopy("\pF2", theName);
				break;
				case 0x007A:
				PasStringCopy("\pF1", theName);
				break;
				default:
				NumToString(theVirtual, theName);
				break;
			}
			break;
			
			case kClearKeyASCII:
			PasStringCopy("\pclear", theName);
			break;
			
			case kEscapeKeyASCII:
			if (theVirtual == 0x0047)
				PasStringCopy("\pclear", theName);
			else
				PasStringCopy("\pesc", theName);
			break;
			
			case kLeftArrowKeyASCII:
			PasStringCopy("\plf arrow", theName);
			break;
			
			case kRightArrowKeyASCII:
			PasStringCopy("\prt arrow", theName);
			break;
			
			case kUpArrowKeyASCII:
			PasStringCopy("\pup arrow", theName);
			break;
			
			case kDownArrowKeyASCII:
			PasStringCopy("\pdn arrow", theName);
			break;
			
			case kSpaceBarASCII:
			PasStringCopy("\pspace", theName);
			break;
			
			case kForwardDeleteASCII:
			PasStringCopy("\pfrwd del", theName);
			break;
			
			default:
			PasStringCopy("\p????", theName);
			break;
		}
	}
}

//--------------------------------------------------------------  OptionKeyDown
// Returns true is the Option key is being held down.

Boolean OptionKeyDown (void)
{
	KeyMap		theKeys;
	
	GetKeys(theKeys);
	if (BitTst(&theKeys, kOptionKeyMap))
		return (true);
	else
		return (false);
}

//--------------------------------------------------------------  ExtractCTSeed
// Very esoteric - gets the "color table seed" from a specified graf port.
/*
long ExtractCTSeed (CGrafPtr porter)
{
	long		theSeed;
	
	theSeed = (**((**(porter->portPixMap)).pmTable)).ctSeed;
	return(theSeed);
}
*/
//--------------------------------------------------------------  ForceCTSeed
// Forces  the "color table seed" from a specified graf port to a…
// specified value.
/*
void ForceCTSeed (CGrafPtr porter, long newSeed)
{
	(**((**(porter->portPixMap)).pmTable)).ctSeed = newSeed;
}
*/
//--------------------------------------------------------------  DelayTicks
// Lil' function that just sits and waits a specified number of…
// Ticks (1/60 of a second).

void DelayTicks (long howLong)
{
	UInt32		whoCares;
	
	Delay(howLong, &whoCares);
}

//--------------------------------------------------------------  UnivGetSoundVolume
// Returns the speaker volume (as set by the user) in the range of…
// zero to seven (handles Sound Manager 3 case as well).

void UnivGetSoundVolume (short *volume, Boolean hasSM3)
{
#pragma unused (hasSM3)
	long		longVol;
	OSErr		theErr;
	
//	if (hasSM3)
//	{
		theErr = GetDefaultOutputVolume(&longVol);
		*volume = LoWord(longVol) / 0x0024;
//	}
//	else
//		GetSoundVol(volume);
	
	if (*volume > 7)
		*volume = 7;
	else if (*volume < 0)
		*volume = 0;
}

//--------------------------------------------------------------  UnivSetSoundVolume
// Sets the speaker volume to a specified value (in the range of…
// zero to seven (handles Sound Manager 3 case as well).

void  UnivSetSoundVolume (short volume, Boolean hasSM3)
{
#pragma unused (hasSM3)
	long		longVol;
	OSErr		theErr;
	
	if (volume > 7)
		volume = 7;
	else if (volume < 0)
		volume = 0;
	
//	if (hasSM3)
//	{
		longVol = (long)volume * 0x0025;
		if (longVol > 0x00000100)
			longVol = 0x00000100;
		longVol = longVol + (longVol << 16);
		theErr = SetDefaultOutputVolume(longVol);
//	}
//	else
//		SetSoundVol(volume);
}

