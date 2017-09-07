//============================================================================
//----------------------------------------------------------------------------
//								MainWindow.c
//----------------------------------------------------------------------------
//============================================================================


#include <NumberFormatting.h>
#include <ToolUtils.h>
#include "Externs.h"
#include "Environ.h"
#include "House.h"
#include "RectUtils.h"


#define kMainWindowID			128
#define kEditWindowID			129
#define kMenuWindowID			130


void DrawOnSplash (void);
void SetPaletteToGrays (void);
void HardDrawMainWindow (void);
void RestoreColorsSlam (void);


CTabHandle		theCTab;
PixMapHandle	thePMap;
ColorSpec *		wasColors;
ColorSpec *		newColors;
CursHandle		handCursorH, beamCursorH, vertCursorH, horiCursorH;
CursHandle		diagCursorH;
Cursor			handCursor, beamCursor, vertCursor, horiCursor;
Cursor			diagCursor;
Rect			workSrcRect;
GWorldPtr		workSrcMap;
Rect			mainWindowRect;
WindowPtr		mainWindow, menuWindow;
short			isEditH, isEditV;
short			playOriginH, playOriginV;
short			splashOriginH, splashOriginV;
short			theMode;
Boolean			fadeGraysOut, isDoColorFade, splashDrawn;

extern	GDHandle	thisGDevice;
extern	short		toolSelected;
extern	Boolean		noRoomAtAll, isUseSecondScreen;
extern	Boolean		quickerTransitions, houseIsReadOnly;


//==============================================================  Functions
//--------------------------------------------------------------  DrawOnSplash

// Draws additional text on top of splash screen.

void DrawOnSplash (void)
{
	Str255		houseLoadedStr;
	
	PasStringCopy("\pHouse: ", houseLoadedStr);
	PasStringConcat(houseLoadedStr, thisHouseName);
	if ((thisMac.hasQT) && (hasMovie))
		PasStringConcat(houseLoadedStr, "\p (QT)");
	TextSize(9);
	TextFace(1);
	TextFont(applFont);
	MoveTo(splashOriginH + 436, splashOriginV + 314);
	if (thisMac.isDepth == 4)
	{
		ForeColor(whiteColor);
		DrawString(houseLoadedStr);
		ForeColor(blackColor);
	}
	else
	{
		if (houseIsReadOnly)
			ColorText(houseLoadedStr, 5L);
		else
			ColorText(houseLoadedStr, 28L);
	}
	
	#if defined(powerc) || defined(__powerc)
	TextSize(12);
	TextFace(0);
	TextFont(systemFont);
	ForeColor(blackColor);
	MoveTo(splashOriginH + 5, splashOriginV + 457);
	DrawString("\pPowerPC Native!");
	ForeColor(whiteColor);
	MoveTo(splashOriginH + 4, splashOriginV + 456);
	DrawString("\pPowerPC Native!");
	ForeColor(blackColor);
	#endif
}

//--------------------------------------------------------------  RedrawSplashScreen

void RedrawSplashScreen (void)
{
	Rect		tempRect;
	
	SetPort((GrafPtr)workSrcMap);
	PaintRect(&workSrcRect);
	QSetRect(&tempRect, 0, 0, 640, 460);
	QOffsetRect(&tempRect, splashOriginH, splashOriginV);
	LoadScaledGraphic(kSplash8BitPICT, &tempRect);
	DrawOnSplash();
	SetPortWindowPort(mainWindow);
//	if (quickerTransitions)
//		DissBitsChunky(&workSrcRect);
//	else
//		DissBits(&workSrcRect);
	CopyRectMainToWork(&workSrcRect);
}

//--------------------------------------------------------------  UpdateMainWindow

// Redraws the main window (depends on mode were in - splash, editing, playing).

void UpdateMainWindow (void)
{
	Rect		tempRect;
	RgnHandle	dummyRgn;
	
	dummyRgn = NewRgn();
	SetPortWindowPort(mainWindow);
	
	if (theMode == kEditMode)
	{
		PauseMarquee();
		CopyBits((BitMap *)*GetGWorldPixMap(workSrcMap), 
				GetPortBitMapForCopyBits(GetWindowPort(mainWindow)), 
				&mainWindowRect, &mainWindowRect, srcCopy, 
				GetPortVisibleRegion(GetWindowPort(mainWindow), dummyRgn));
		ResumeMarquee();
	}
	else if ((theMode == kSplashMode) || (theMode == kPlayMode))
	{
		SetPort((GrafPtr)workSrcMap);
		PaintRect(&workSrcRect);
		QSetRect(&tempRect, 0, 0, 640, 460);
		QOffsetRect(&tempRect, splashOriginH, splashOriginV);
		LoadScaledGraphic(kSplash8BitPICT, &tempRect);
		CopyBits((BitMap *)*GetGWorldPixMap(workSrcMap), 
				GetPortBitMapForCopyBits(GetWindowPort(mainWindow)), 
				&workSrcRect, &mainWindowRect, srcCopy, 
				GetPortVisibleRegion(GetWindowPort(mainWindow), dummyRgn));
		SetPortWindowPort(mainWindow);
		
		DrawOnSplash();
	}
	
	DisposeRgn(dummyRgn);
	splashDrawn = true;
}

//--------------------------------------------------------------  UpdateMenuBarWindow
// Ugly kludge to cover over the menu bar when playing game on 2nd monitor.

void UpdateMenuBarWindow (void)
{
	Rect		bounds;
	
	if (menuWindow == nil)
		return;
	
	GetLocalWindowRect(menuWindow, &bounds);
	PaintRect(&bounds);
}

//--------------------------------------------------------------  OpenMainWindow
// Opens up the main window (how it does this depends on mode were in).

void OpenMainWindow (void)
{
//	long		wasSeed;
	short		whichRoom;
	
	if (mainWindow != nil)
	{
		YellowAlert(kYellowUnaccounted, 6);
		return;
	}
	
	if (theMode == kEditMode)
	{
		if (menuWindow != nil)
			DisposeWindow(menuWindow);
		menuWindow = nil;
		
		QSetRect(&mainWindowRect, 0, 0, 512, 322);
		mainWindow = GetNewCWindow(kEditWindowID, nil, kPutInFront);
		SizeWindow(mainWindow, mainWindowRect.right, 
				mainWindowRect.bottom, false);
		
		if (OptionKeyDown())
		{
			isEditH = 3;
			isEditV = 41;
		}
		MoveWindow(mainWindow, isEditH, isEditV, true);
		ShowWindow(mainWindow);
		SetPortWindowPort(mainWindow);
		ClipRect(&mainWindowRect);
		ForeColor(blackColor);
		BackColor(whiteColor);
		
		whichRoom = GetFirstRoomNumber();
		CopyRoomToThisRoom(whichRoom);
		ReflectCurrentRoom(false);
	}
	else
	{
		if (menuWindow == nil)
		{
			menuWindow = GetNewCWindow(kMenuWindowID, nil, kPutInFront);
			SizeWindow(menuWindow, RectWide(&thisMac.screen), 20, false);
			MoveWindow(menuWindow, thisMac.screen.left, 
					thisMac.screen.top, true);
			ShowWindow(menuWindow);
		}
		mainWindowRect = thisMac.screen;
		ZeroRectCorner(&mainWindowRect);
		mainWindowRect.bottom -= 20;		// thisMac.menuHigh
		mainWindow = GetNewCWindow(kMainWindowID, nil, kPutInFront);
		SizeWindow(mainWindow, mainWindowRect.right - mainWindowRect.left, 
				mainWindowRect.bottom - mainWindowRect.top, false);
		MoveWindow(mainWindow, thisMac.screen.left, 
				thisMac.screen.top + 20, true);	// thisMac.menuHigh
		ShowWindow(mainWindow);
		SetPortWindowPort(mainWindow);
		ClipRect(&mainWindowRect);
//		CopyRgn(mainWindow->clipRgn, mainWindow->visRgn);
		ForeColor(blackColor);
		BackColor(whiteColor);
		PaintRect(&mainWindowRect);
		
		splashOriginH = ((thisMac.screen.right - thisMac.screen.left) - 640) / 2;
		if (splashOriginH < 0)
			splashOriginH = 0;
		splashOriginV = ((thisMac.screen.bottom - thisMac.screen.top) - 480) / 2;
		if (splashOriginV < 0)
			splashOriginV = 0;
		
		SetPort((GrafPtr)workSrcMap);
		PaintRect(&workSrcRect);
		LoadGraphic(kSplash8BitPICT);
		
//		if ((fadeGraysOut) && (isDoColorFade))
//		{
//			wasSeed = ExtractCTSeed((CGrafPtr)mainWindow);
//			SetPortWindowPort(mainWindow);
//			SetPaletteToGrays();
//			HardDrawMainWindow();
//			fadeGraysOut = false;
//			ForceCTSeed((CGrafPtr)mainWindow, wasSeed);
//		}
		
		SetPortWindowPort(mainWindow);
	}
}

//--------------------------------------------------------------  CloseMainWindow

// Closes the main window.

void CloseMainWindow (void)
{
	if (mainWindow != nil)
		DisposeWindow(mainWindow);
	mainWindow = nil;
}

//--------------------------------------------------------------  ZoomBetweenWindows
// Zooms from one window size to another.  Just for effect.

/*
#ifndef COMPILEDEMO
void ZoomBetweenWindows (void)
{
	Rect		aRect;
	short		h, v;
	
	if (theMode == kEditMode)
	{
		QSetRect(&aRect, 0, 0, 512, 342);
		QOffsetRect(&aRect, isEditH, isEditV);
		ZoomRectToRect(&(thisMac.screen), &aRect);
	}
	else
	{
		aRect = mainWindow->portRect;
		GetWindowLeftTop(mainWindow, &h, &v);
		QOffsetRect(&aRect, h, v);
		ZoomRectToRect(&aRect, &(thisMac.screen));
	}
}
#endif
*/

//--------------------------------------------------------------  UpdateEditWindowTitle
// Handles changing the title across the top of the main window.  OnlyÉ
// relevant when editing a house (room title displayed in window title).

#ifndef COMPILEDEMO
void UpdateEditWindowTitle (void)
{
	Str255		newTitle, tempStr;
	
	if (mainWindow == nil)
		return;
	
	PasStringCopy(thisHouseName, newTitle);
	PasStringConcat(newTitle, "\p - ");
	if (noRoomAtAll)
		PasStringConcat(newTitle, "\pNo rooms");
	else if (houseUnlocked)
	{
		PasStringConcat(newTitle, thisRoom->name);
		PasStringConcat(newTitle, "\p (");
		NumToString((long)thisRoom->floor, tempStr);
		PasStringConcat(newTitle, tempStr);
		PasStringConcat(newTitle, "\p, ");
		NumToString((long)thisRoom->suite, tempStr);
		PasStringConcat(newTitle, tempStr);
		PasStringConcat(newTitle, "\p)");
	}
	else
		PasStringConcat(newTitle, "\pHouse Locked");
	SetWTitle(mainWindow, newTitle);
}
#endif

//--------------------------------------------------------------  HandleMainClick

// Handle a mouse click in the main window (relevant only when editing).

void HandleMainClick (Point wherePt, Boolean isDoubleClick)
{
	KeyMap		theseKeys;
	
	if ((theMode != kEditMode) || (mainWindow == nil) || 
			(!houseUnlocked))
		return;
	
	SetPortWindowPort(mainWindow);
	GlobalToLocal(&wherePt);
	
	if (toolSelected == kSelectTool)
		DoSelectionClick(wherePt, isDoubleClick);
	else
		DoNewObjectClick(wherePt);
	
	GetKeys(theseKeys);
	if (!BitTst(&theseKeys, kShiftKeyMap))
	{
		EraseSelectedTool();
		SelectTool(kSelectTool);
	}
}

//--------------------------------------------------------------  ShowMenuBarOld
// Displays the menu bar (after having been hidden).
/*
void ShowMenuBarOld (void)
{
	Rect			theRect;
	GrafPtr			wasPort, tempPort;
	RgnHandle		worldRgn, menuBarRgn;
	
	if (LMGetMBarHeight() == 0)
	{
		GetPort(&wasPort);
		tempPort = (GrafPtr)NewPtrClear(sizeof(GrafPort));
		OpenPort(tempPort);
		SetPort((GrafPtr)tempPort);
		
		LMSetMBarHeight(thisMac.menuHigh);
		
		theRect = (**GetGrayRgn()).rgnBBox;
		UnionRect(&theRect, &qd.screenBits.bounds, &theRect);
		worldRgn = NewRgn();
		OpenRgn();
		FrameRoundRect(&theRect, 16, 16);
		CloseRgn(worldRgn);
		
		theRect = qd.screenBits.bounds;
		theRect.bottom = theRect.top + thisMac.menuHigh;
		menuBarRgn = NewRgn();
		RectRgn(menuBarRgn, &theRect);
		
		SectRgn(worldRgn, menuBarRgn, menuBarRgn);	//	/------------------\	
		DisposeRgn(worldRgn);						//	|__________________|	
		
		UnionRgn(tempPort->visRgn, menuBarRgn, tempPort->visRgn);
		DiffRgn(tempPort->visRgn, menuBarRgn, tempPort->visRgn);
		DisposeRgn(menuBarRgn);
		
		ClosePort(tempPort);
		SetPort((GrafPtr)wasPort);
		
		DrawMenuBar();
	}
}
*/
//--------------------------------------------------------------  HideMenuBarOld
// Hides the menu bar - completely erasing it from the screen.
/*
void HideMenuBarOld (void)
{
	Rect			theRect;
	RgnHandle		worldRgn, menuBarRgn;
	GrafPtr			wasPort, tempPort;
	
	if (LMGetMBarHeight() != 0)
	{
		GetPort(&wasPort);
		tempPort = (GrafPtr)NewPtrClear(sizeof(GrafPort));
		OpenPort(tempPort);
		SetPort((GrafPtr)tempPort);
		
		LMSetMBarHeight(0);
		
		theRect = (**GetGrayRgn()).rgnBBox;
		UnionRect(&theRect, &qd.screenBits.bounds, &theRect);
		worldRgn = NewRgn();
		OpenRgn();
		FrameRoundRect(&theRect, 16, 16);
		CloseRgn(worldRgn);
		
		theRect = qd.screenBits.bounds;
		theRect.bottom = theRect.top + thisMac.menuHigh;
		menuBarRgn = NewRgn();
		RectRgn(menuBarRgn, &theRect);
		
		SectRgn(worldRgn, menuBarRgn, menuBarRgn);	//	/------------------\	
		DisposeRgn(worldRgn);						//	|__________________|	
		
		UnionRgn(tempPort->visRgn, menuBarRgn, tempPort->visRgn);
		DisposeRgn(menuBarRgn);
		
		PaintRect(&theRect);
		
		ClosePort(tempPort);
		SetPort((GrafPtr)wasPort);
	}
}
*/
//--------------------------------------------------------------  SetPaletteToGrays

// Sets up a gray palette corresponding in luminance to the standard colorÉ
// palette.  This is to facilitate the gray->color fade when the game comes up.
/*
void SetPaletteToGrays (void)
{
	GDHandle	theDevice;
	long		longGray;
	short		i;
	char		wasState;
	
	wasState = HGetState((Handle)thisGDevice);
	HLock((Handle)thisGDevice);
	thePMap = (*thisGDevice)->gdPMap;
	HSetState((Handle)thisGDevice, wasState);
	
	theCTab = (*thePMap)->pmTable;
	wasColors = nil;
	wasColors = (ColorSpec*)NewPtr(sizeof(ColorSpec) * 256);
	if (wasColors == nil)
		RedAlert(kErrNoMemory);
	
	newColors = nil;
	newColors = (ColorSpec*)NewPtr(sizeof(ColorSpec) * 256);
	if (newColors == nil)
		RedAlert(kErrNoMemory);
	
	for (i = 0; i < 256; i++)
	{
		wasColors[i] = (*theCTab)->ctTable[i];
		newColors[i] = (*theCTab)->ctTable[i];
		
		if (i != 5)
		{
			longGray = ((long)newColors[i].rgb.red * 3L) / 10L + 
				((long)newColors[i].rgb.green * 6L) / 10L + 
				((long)newColors[i].rgb.blue * 1L) / 10L;
			
			newColors[i].rgb.red = (unsigned short)longGray;
			newColors[i].rgb.green = (unsigned short)longGray;
			newColors[i].rgb.blue = (unsigned short)longGray;
		}
	}
	
	theDevice = GetGDevice();
	SetGDevice(thisGDevice);
	SetEntries(0, 255, newColors);
	SetGDevice(theDevice);
}
*/
//--------------------------------------------------------------  HardDrawMainWindow
// Ignores the ToolBox - this function draws direct to screen in order toÉ
// circumvent the Toolbox's attempt to color-match to the current palette.
/*
void HardDrawMainWindow (void)
{
	PixMapHandle	pixMapH;
	Point			offsetPt;
	long			srcRowBytes, destRowBytes;
	long			src;
	long			dest;
	short			i, w;
	SInt8			mode;
	char			wasState;
	
	wasState = HGetState((Handle)thisGDevice);
	HLock((Handle)thisGDevice);
	pixMapH = (**thisGDevice).gdPMap;
	HSetState((Handle)thisGDevice, wasState);
	
	srcRowBytes = (long)((*(workSrcMap->portPixMap))->rowBytes & 0x7FFF);
	destRowBytes = (**pixMapH).rowBytes & 0x7FFF;
	src = (long)((*(workSrcMap->portPixMap))->baseAddr);
	dest = (long)((**pixMapH).baseAddr) + splashOriginH + 
			((splashOriginV + thisMac.menuHigh) * destRowBytes);
	
	offsetPt.h = 0;
	offsetPt.v = 0;
	ShieldCursor(&mainWindowRect, offsetPt);
	mode = true32b;
	SwapMMUMode(&mode);
	for (i = 0; i < 460; i++)
	{
		for (w = 0; w < 160; w++)
		{
			*(long *)dest = *(long *)src;
			dest += 4L;
			src += 4L;
		}
		src -= 640;
		dest -= 640;
		src += srcRowBytes;
		dest += destRowBytes;
	}
	SwapMMUMode(&mode);
	ShowCursor();
}
*/
//--------------------------------------------------------------  WashColorIn
// Slowly walks the palette from its gray luminance state to the full colorÉ
// palette.  In this way, color appears to slowly wash in.
/*
void WashColorIn (void)
{
	#define		kGray2ColorSteps	180
	GDHandle	theDevice;
	long		longDelta;
	short		i, c;
	
	theDevice = GetGDevice();
	SetGDevice(thisGDevice);
	
	for (i = 0; i < kGray2ColorSteps; i++)
	{
		for (c = 0; c < 256; c++)
		{
			if (c != 5)
			{
				longDelta = (((long)wasColors[c].rgb.red - 
						(long)newColors[c].rgb.red) / 
						(long)(kGray2ColorSteps - i)) + (long)newColors[c].rgb.red;
				newColors[c].rgb.red = (unsigned short)longDelta;
				
				longDelta = (((long)wasColors[c].rgb.green - 
						(long)newColors[c].rgb.green) / 
						(long)(kGray2ColorSteps - i)) + 
						(long)newColors[c].rgb.green;
				newColors[c].rgb.green = (unsigned short)longDelta;
				
				longDelta = (((long)wasColors[c].rgb.blue - 
						(long)newColors[c].rgb.blue) / 
						(long)(kGray2ColorSteps - i)) + 
						(long)newColors[c].rgb.blue;
				newColors[c].rgb.blue = (unsigned short)longDelta;
			}
		}
		SetEntries(0, 255, newColors);
		if (Button())
			break;
	}
	
	SetEntries(0, 255, wasColors);
	SetGDevice(theDevice);
	
	RestoreColorsSlam();
	
	if (wasColors != nil)
		DisposePtr((Ptr)wasColors);
	if (newColors != nil)
		DisposePtr((Ptr)newColors);
}
*/
