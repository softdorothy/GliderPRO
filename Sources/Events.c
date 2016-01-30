
//============================================================================
//----------------------------------------------------------------------------
//									Events.c
//----------------------------------------------------------------------------
//============================================================================


#include <AppleEvents.h>
#include <ToolUtils.h>
#include "Externs.h"
#include "Environ.h"
#include "House.h"
#include "ObjectEdit.h"


short BitchAboutColorDepth (void);
void HandleMouseEvent (EventRecord *);
void HandleKeyEvent (EventRecord *);
void HandleUpdateEvent (EventRecord *);
void HandleOSEvent (EventRecord *);
void HandleHighLevelEvent (EventRecord *);
void HandleIdleTask (void);
void IncrementMode (void);


long			lastUp, incrementModeTime;
UInt32			doubleTime;
Point			lastWhere;
short			idleMode;
Boolean			doAutoDemo, switchedOut;

extern	WindowPtr	mapWindow, toolsWindow, linkWindow;
extern	WindowPtr	menuWindow;
extern	short		isEditH, isEditV, isMapH, isMapV, isToolsH, isToolsV;
extern	short		isLinkH, isLinkV, isCoordH, isCoordV;
extern	Boolean		quitting, isMusicOn, failedMusic;
extern	Boolean		autoRoomEdit, newRoomNow, isPlayMusicIdle;


//==============================================================  Functions
//--------------------------------------------------------------  BitchAboutColorDepth
// Display a dialog that alerts the user that they have switched the bit…
// depth of the monitor under our noses.  They must return it to previous.

short BitchAboutColorDepth (void)
{
	#define		kColorSwitchedAlert		1042
	short		sheSaid;
	
//	CenterAlert(kColorSwitchedAlert);
	sheSaid = Alert(kColorSwitchedAlert, nil);
	
	return (sheSaid);
}

//--------------------------------------------------------------  HandleMouseEvent
// Handle a mouse click event.

void HandleMouseEvent (EventRecord *theEvent)
{
	WindowPtr	whichWindow;
	long		menuChoice, newSize;
	short		thePart, hDelta, vDelta;
	Boolean		isDoubleClick;
	
	thePart = FindWindow(theEvent->where, &whichWindow);
	
	switch (thePart)
	{
		case inSysWindow:
//		SystemClick(theEvent, whichWindow);
		break;
		
		case inMenuBar:
		menuChoice = MenuSelect(theEvent->where);
		DoMenuChoice(menuChoice);
		break;
		
		case inDrag:
		DragWindow(whichWindow, theEvent->where, &thisMac.screen);
		if (whichWindow == mainWindow)
		{
			SendBehind(mainWindow, (WindowPtr)0L);
			GetWindowLeftTop(whichWindow, &isEditH, &isEditV);
		}
		else if (whichWindow == mapWindow)
			GetWindowLeftTop(whichWindow, &isMapH, &isMapV);
		else if (whichWindow == toolsWindow)
			GetWindowLeftTop(whichWindow, &isToolsH, &isToolsV);
		else if (whichWindow == linkWindow)
			GetWindowLeftTop(whichWindow, &isLinkH, &isLinkV);
		else if (whichWindow == coordWindow)
			GetWindowLeftTop(whichWindow, &isCoordH, &isCoordV);
		HiliteAllWindows();
		break;
		
		case inGoAway:
		if (TrackGoAway(whichWindow,theEvent->where))
		{
			if (whichWindow == mapWindow)
				ToggleMapWindow();
			else if (whichWindow == toolsWindow)
				ToggleToolsWindow();
			else if (whichWindow == linkWindow)
				CloseLinkWindow();
			else if (whichWindow == coordWindow)
				ToggleCoordinateWindow();
		}
		break;
		
		case inGrow:
		if (whichWindow == mapWindow)
		{
			newSize = GrowWindow(mapWindow, theEvent->where, &thisMac.gray);
			ResizeMapWindow(LoWord(newSize), HiWord(newSize));
		}
		break;
		
		case inZoomIn:
		case inZoomOut:
		if (TrackBox(whichWindow, theEvent->where, thePart))
			ZoomWindow(whichWindow, thePart, true);
		break;
		
		case inContent:
		if (whichWindow == mainWindow)
		{
			hDelta = theEvent->where.h - lastWhere.h;
			if (hDelta < 0)
				hDelta = -hDelta;
			vDelta = theEvent->where.v - lastWhere.v;
			if (vDelta < 0)
				vDelta = -vDelta;
			if (((theEvent->when - lastUp) < doubleTime) && (hDelta < 5) && 
					(vDelta < 5))
				isDoubleClick = true;
			else
			{
				isDoubleClick = false;
				lastUp = theEvent->when;
				lastWhere = theEvent->where;
			}
			HandleMainClick(theEvent->where, isDoubleClick);
		}
		else if (whichWindow == mapWindow)
			HandleMapClick(theEvent);
		else if (whichWindow == toolsWindow)
			HandleToolsClick(theEvent->where);
		else if (whichWindow == linkWindow)
			HandleLinkClick(theEvent->where);
		break;
		
		default:
		break;
	}
}

//--------------------------------------------------------------  HandleKeyEvent
// Handle a key-down event.

void HandleKeyEvent (EventRecord *theEvent)
{
	char		theChar;
	Boolean		shiftDown, commandDown, optionDown;
	
	theChar = theEvent->message & charCodeMask;
	shiftDown = ((theEvent->modifiers & shiftKey) != 0);
	commandDown = ((theEvent->modifiers & cmdKey) != 0);
	optionDown = ((theEvent->modifiers & optionKey) != 0);
	
	if ((commandDown) && (!optionDown))
		DoMenuChoice(MenuKey(theChar));
	else 
	{
		switch (theChar)
		{
			case kHelpKeyASCII:
			break;
			
			case kPageUpKeyASCII:
			if (houseUnlocked)
				PrevToolMode();
			break;
			
			case kPageDownKeyASCII:
			if (houseUnlocked)
				NextToolMode();
			break;
			
#if BUILD_ARCADE_VERSION
			
			case kLeftArrowKeyASCII:
			DoOptionsMenu(iHighScores);
			break;
			
			case kRightArrowKeyASCII:
			DoOptionsMenu(iHelp);
			break;
			
			case kUpArrowKeyASCII:
			DoGameMenu(iNewGame);
			break;
			
			case kDownArrowKeyASCII:
			DoGameMenu(iNewGame);
			break;
			
#else
			
			case kLeftArrowKeyASCII:
			if (houseUnlocked)
			{
				if (objActive == kNoObjectSelected)
					SelectNeighborRoom(kRoomToLeft);
				else
					MoveObject(kBumpLeft, shiftDown);
			}
			break;
			
			case kRightArrowKeyASCII:
			if (houseUnlocked)
			{
				if (objActive == kNoObjectSelected)
					SelectNeighborRoom(kRoomToRight);
				else
					MoveObject(kBumpRight, shiftDown);
			}
			break;
			
			case kUpArrowKeyASCII:
			if (houseUnlocked)
			{
				if (objActive == kNoObjectSelected)
					SelectNeighborRoom(kRoomAbove);
				else
					MoveObject(kBumpUp, shiftDown);
			}
			break;
			
			case kDownArrowKeyASCII:
			if (houseUnlocked)
			{
				if (objActive == kNoObjectSelected)
					SelectNeighborRoom(kRoomBelow);
				else
					MoveObject(kBumpDown, shiftDown);
			}
			break;
			
#endif
			
			case kDeleteKeyASCII:
			if (houseUnlocked)
			{
				if (objActive == kNoObjectSelected)
					DeleteRoom(true);
				else
					DeleteObject();
			}
			break;
			
			case kTabKeyASCII:
			if ((theMode == kEditMode) && (houseUnlocked))
			{
				if (shiftDown)
					SelectPrevObject();
				else
					SelectNextObject();
			}
			break;
			
			case kEscapeKeyASCII:
			if ((theMode == kEditMode) && (houseUnlocked))
				DeselectObject();
			break;
			
			case kAKeyASCII:
			case kCapAKeyASCII:
			if ((theMode == kEditMode) && (houseUnlocked))
				SetSpecificToolMode(kApplianceMode);
			break;
			
			case kBKeyASCII:
			case kCapBKeyASCII:
			if ((theMode == kEditMode) && (houseUnlocked))
				SetSpecificToolMode(kBlowerMode);
			break;
			
			case kCKeyASCII:
			case kCapCKeyASCII:
			if ((theMode == kEditMode) && (houseUnlocked))
				SetSpecificToolMode(kClutterMode);
			break;
			
			case kEKeyASCII:
			case kCapEKeyASCII:
			if ((theMode == kEditMode) && (houseUnlocked))
				SetSpecificToolMode(kEnemyMode);
			break;
			
			case kFKeyASCII:
			case kCapFKeyASCII:
			if ((theMode == kEditMode) && (houseUnlocked))
				SetSpecificToolMode(kFurnitureMode);
			break;
			
			case kLKeyASCII:
			case kCapLKeyASCII:
			if ((theMode == kEditMode) && (houseUnlocked))
				SetSpecificToolMode(kLightMode);
			break;
			
			case kPKeyASCII:
			case kCapPKeyASCII:
			if ((theMode == kEditMode) && (houseUnlocked))
				SetSpecificToolMode(kBonusMode);
			break;
			
			case kSKeyASCII:
			case kCapSKeyASCII:
			if ((theMode == kEditMode) && (houseUnlocked))
				SetSpecificToolMode(kSwitchMode);
			break;
			
			case kTKeyASCII:
			case kCapTKeyASCII:
			if ((theMode == kEditMode) && (houseUnlocked))
				SetSpecificToolMode(kTransportMode);
			break;
			
			default:
			break;
		}
	}
}

//--------------------------------------------------------------  HandleUpdateEvent
// Handle an update event.

void HandleUpdateEvent (EventRecord *theEvent)
{	
	if ((WindowPtr)theEvent->message == mainWindow)
	{
		SetPort((GrafPtr)mainWindow);
		BeginUpdate(mainWindow);
		UpdateMainWindow();
		EndUpdate(mainWindow);
	}
	else if ((WindowPtr)theEvent->message == mapWindow)
	{
		SetPort((GrafPtr)mapWindow);
		BeginUpdate(mapWindow);
		UpdateMapWindow();
		EndUpdate(mapWindow);
	}
	else if ((WindowPtr)theEvent->message == toolsWindow)
	{
		SetPort((GrafPtr)toolsWindow);
		BeginUpdate(toolsWindow);
		UpdateToolsWindow();
		EndUpdate(toolsWindow);
	}
	else if ((WindowPtr)theEvent->message == linkWindow)
	{
		SetPort((GrafPtr)linkWindow);
		BeginUpdate(linkWindow);
		UpdateLinkWindow();
		EndUpdate(linkWindow);
	}
	else if ((WindowPtr)theEvent->message == coordWindow)
	{
		SetPort((GrafPtr)coordWindow);
		BeginUpdate(coordWindow);
		UpdateCoordWindow();
		EndUpdate(coordWindow);
	}
	else if ((WindowPtr)theEvent->message == menuWindow)
	{
		SetPort((GrafPtr)menuWindow);
		BeginUpdate(menuWindow);
		UpdateMenuBarWindow();
		EndUpdate(menuWindow);
	}
}

//--------------------------------------------------------------  HandleOSEvent
// Handle an OS Event (MultiFinder - user has switched in or out of app).

void HandleOSEvent (EventRecord *theEvent)
{
	OSErr		theErr;
	short		buttonHit;
	
	if (theEvent->message & 0x01000000)		// suspend or resume event
	{
		if (theEvent->message & 0x00000001)	// resume event
		{
			if (WhatsOurDepth() != thisMac.isDepth)
			{
				buttonHit = BitchAboutColorDepth();
				if (buttonHit == 1)			// player wants to Quit
				{
#ifndef COMPILEDEMO
					if (QuerySaveChanges())
						quitting = true;
#else
					quitting = true;
#endif
				}
				else
				{
					SwitchToDepth(thisMac.isDepth, thisMac.wasColorOrGray);
				}
			}
			switchedOut = false;
			InitCursor();
			if ((isPlayMusicIdle) && (theMode != kEditMode))
			{
				theErr = StartMusic();
				if (theErr != noErr)
				{
					YellowAlert(kYellowNoMusic, theErr);
					failedMusic = true;
				}
			}
			incrementModeTime = TickCount() + kIdleSplashTicks;
			
#ifndef COMPILEDEMO
//			if (theMode == kEditMode)
//				SeeIfValidScrapAvailable(true);
#endif
		}
		else								// suspend event
		{
			switchedOut = true;
			InitCursor();
			if ((isMusicOn) && (theMode != kEditMode))
				StopTheMusic();
		}
	}
}

//--------------------------------------------------------------  HandleHighLevelEvent
// Handle an AppleEvent (Open Document, Quit Application, etc.).

void HandleHighLevelEvent (EventRecord *theEvent)
{
	OSErr		theErr;
	
	theErr = AEProcessAppleEvent(theEvent);
	if ((theErr != noErr) && (theErr != errAEEventNotHandled))
		YellowAlert(kYellowAppleEventErr, theErr);
}

//--------------------------------------------------------------  HandleIdleTask
// Handle some processing during event lulls.

void HandleIdleTask (void)
{
	if (theMode == kEditMode)
	{
		SetPort((GrafPtr)mainWindow);
		DoMarquee();
		
		if ((autoRoomEdit) && (newRoomNow))
		{
			if (theMode == kEditMode)
				DoRoomInfo();
			newRoomNow = false;
		}
	}
}

//--------------------------------------------------------------  HandleEvent
// "Master" function that tests for events and calls the above functions to…
// handle each event type.  Not called during and actual game.

void HandleEvent (void)
{
	KeyMap		eventKeys;
	EventRecord	theEvent;
	long		sleep = 2;
	Boolean		itHappened;
	
	GetKeys(eventKeys);
	if ((BitTst(&eventKeys, kCommandKeyMap)) && 
			(BitTst(&eventKeys, kOptionKeyMap)))
	{
		HiliteAllObjects();
	}
	else if ((BitTst(&eventKeys, kOptionKeyMap)) && (theMode == kEditMode) && 
			(houseUnlocked))
	{
		EraseSelectedTool();
		SelectTool(kSelectTool);
	}
	
	if (thisMac.hasWNE)
		itHappened = WaitNextEvent(everyEvent, &theEvent, sleep, nil);
	else
	{
//		SystemTask();
		itHappened = GetNextEvent(everyEvent, &theEvent);
	}
	
	if (itHappened)
	{
		switch (theEvent.what)
		{
			case mouseDown:
			HandleMouseEvent(&theEvent);
			break;
			
			case keyDown:
			case autoKey:
			HandleKeyEvent(&theEvent);
			break;
			
			case updateEvt:
			HandleUpdateEvent(&theEvent);
			break;
			
			case osEvt:
			HandleOSEvent(&theEvent);
			break;
			
			case kHighLevelEvent:
			HandleHighLevelEvent(&theEvent);
			break;
		}
	}
	else
		HandleIdleTask();
	
	if ((theMode == kSplashMode) && doAutoDemo && !switchedOut)
	{
		if (TickCount() >= incrementModeTime)
			DoDemoGame();
	}
}

//--------------------------------------------------------------  HiliteAllWindows

// Ugly kludge in order to keep "floating windows" (palettes) on top of…
// the main window.

void HiliteAllWindows (void)
{
	if (mainWindow != nil)
		HiliteWindow(mainWindow, true);
	if (mapWindow != nil)
		HiliteWindow(mapWindow, true);
	if (toolsWindow != nil)
		HiliteWindow(toolsWindow, true);
	if (coordWindow != nil)
		HiliteWindow(coordWindow, true);
	if (linkWindow != nil)
		HiliteWindow(linkWindow, true);
}

//--------------------------------------------------------------  IgnoreThisClick

// Another inelegant kludge designed to temporarily prevent an unwanted…
// double-click to be registered.

void IgnoreThisClick (void)
{
	lastUp -= doubleTime;
	lastWhere.h = -100;
	lastWhere.v = -100;
}

