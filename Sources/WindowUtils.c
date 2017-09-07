
//============================================================================
//----------------------------------------------------------------------------
//								WindowUtils.c
//----------------------------------------------------------------------------
//============================================================================


#include "Externs.h"
#include "Environ.h"
#include "RectUtils.h"


#define kFloatingKind		2048
#define kMessageWindowTall	48


WindowPtr	mssgWindow;


//==============================================================  Functions
//--------------------------------------------------------------  GetWindowTopLeft

// Returns the top left coordinate of the specified window.  Corrdinates…
// are (of course) global (local coordinates would always be (0, 0)).

void GetWindowLeftTop (WindowPtr theWindow, short *left, short *top)
{
	Point		thePoint;
	Rect		bounds;
	
	*left = 0;
	*top = 0;
	
	if (theWindow != nil)
	{
		SetPortWindowPort(theWindow);
		GetWindowBounds(theWindow, kWindowContentRgn, &bounds);
		thePoint.h = bounds.left;
		thePoint.v = bounds.top;
		LocalToGlobal(&thePoint);
		*left = thePoint.h;
		*top = thePoint.v;
	}
}

//--------------------------------------------------------------  GetWindowRect

// Returns bounding rectangle of the specified window in global coords.

void GetWindowRect (WindowPtr theWindow, Rect *bounds)
{
	if (theWindow != nil)
	{
		SetPortWindowPort(theWindow);
		GetWindowBounds(theWindow, kWindowContentRgn, bounds);
		LocalToGlobalRect(bounds);
	}
}

//--------------------------------------------------------------  GetLocalWindowRect
// Returns bounding rectangle of the specified window in local coords.
// (When you just need its width and height.)

void GetLocalWindowRect (WindowPtr theWindow, Rect *bounds)
{
	if (theWindow != nil)
	{
		SetPortWindowPort(theWindow);
		GetWindowBounds(theWindow, kWindowContentRgn, bounds);
	}
}

//--------------------------------------------------------------  FlagWindowFloating
// Sets the specified window's windowKind field to my own kFloatingKind…
// variable.  This way I can examine a window later and determine if…
// it's supposed to "float" above all other windows.
/*
void FlagWindowFloating (WindowPtr theWindow)
{
	if (theWindow != nil)
	{
		((WindowPeek)theWindow)->windowKind = kFloatingKind;
		BringToFront(theWindow);
	}
}

//--------------------------------------------------------------  IsWindowFloating

// Tests a specific window to see if it is supposed to "float" above all…
// other windows.

Boolean	IsWindowFloating (WindowPtr theWindow)
{
	if (theWindow != nil)
	{
		return (((WindowPeek)theWindow)->windowKind == kFloatingKind);
	}
	else
		return (false);
}
*/
//--------------------------------------------------------------  OpenMessageWindow
// Brings up a simple message window.  Nice sort of utility function.
// Anytime you want a small, quick message to come up, call this.

void OpenMessageWindow (StringPtr title)
{
	Rect		mssgWindowRect;
	
	SetRect(&mssgWindowRect, 0, 0, 256, kMessageWindowTall);
	if (thisMac.hasColor)
		mssgWindow = NewCWindow(nil, &mssgWindowRect, 
				title, false, noGrowDocProc, kPutInFront, false, 0L);
	else
		mssgWindow = NewWindow(nil, &mssgWindowRect, 
				title, false, noGrowDocProc, kPutInFront, false, 0L);
	
	if (mssgWindow != nil)
	{
		ShowWindow(mssgWindow);
		SetPort((GrafPtr)mssgWindow);
		ClipRect(&mssgWindowRect);
		ForeColor(blackColor);
		BackColor(whiteColor);
		TextFont(systemFont);
	}
}

//--------------------------------------------------------------  SetMessageWindowMessage

// For the above message window, this function displays a string of text…
// in the center of the window.

void SetMessageWindowMessage (StringPtr message)
{
	Rect		mssgWindowRect;
	
	if (mssgWindow != nil)
	{
		SetPort((GrafPtr)mssgWindow);
		SetRect(&mssgWindowRect, 0, 0, 256, kMessageWindowTall);
		InsetRect(&mssgWindowRect, 16, 16);
		EraseRect(&mssgWindowRect);
		MoveTo(mssgWindowRect.left, mssgWindowRect.bottom - 6);
		DrawString(message);
	}
}

//--------------------------------------------------------------  CloseMessageWindow

// Closes the previously referred to "message window".

void CloseMessageWindow (void)
{
	if (mssgWindow != nil)
		DisposeWindow(mssgWindow);
	mssgWindow = nil;
}

//--------------------------------------------------------------  CloseThisWindow

// Given a specific window, this function will close it and set the window…
// pointer to null.

void CloseThisWindow (WindowPtr *theWindow)
{
	if (*theWindow != nil)
		DisposeWindow(*theWindow);
	*theWindow = nil;
}

