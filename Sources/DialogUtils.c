//============================================================================
//----------------------------------------------------------------------------
//								DialogUtils.c
//----------------------------------------------------------------------------
//============================================================================


#include <ControlDefinitions.h>
#include <LowMem.h>
#include <NumberFormatting.h>
#include "DialogUtils.h"
#include "Externs.h"


#define kActive						0
#define kInactive					255


//==============================================================  Functions
//--------------------------------------------------------------  BringUpDialog
// Given a dialog pointer and a resource ID, this function brings it up…
// centered, visible, and with the default button outlined.

void BringUpDialog (DialogPtr *theDialog, short dialogID)
{
//	CenterDialog(dialogID);
	*theDialog = GetNewDialog(dialogID, nil, kPutInFront);
	if (*theDialog == nil)
		RedAlert(kErrDialogDidntLoad);
	SetPort((GrafPtr)*theDialog);
	ShowWindow(GetDialogWindow(*theDialog));
	DrawDefaultButton(*theDialog);
}

//--------------------------------------------------------------  GetPutDialogCorner
// Determines the upper left corner coordinates needed to properly center…
// the standard Mac PutFile dialog (when you save files).
/*
void GetPutDialogCorner (Point *theCorner)
{
	DialogTHndl	dlogHandle;
	Rect		theScreen, dlogBounds;
	Byte		wasState;
	
	theCorner->h = 64;
	theCorner->v = 64;
	theScreen = qd.screenBits.bounds;
	theScreen.top += LMGetMBarHeight();
	OffsetRect(&theScreen, -theScreen.left, -theScreen.top);
	
	dlogHandle = (DialogTHndl)GetResource('DLOG', sfPutDialogID);
	if (dlogHandle != nil)
	{
		wasState = HGetState((Handle)dlogHandle);
		HLock((Handle)dlogHandle);
		
		dlogBounds = (**dlogHandle).boundsRect;
		OffsetRect(&dlogBounds, -dlogBounds.left, -dlogBounds.top);
		
		theCorner->h = (theScreen.right - dlogBounds.right) / 2;
		theCorner->v = (theScreen.bottom - dlogBounds.bottom) / 3;
		
		HSetState((Handle)dlogHandle, wasState);
	}
	theCorner->v += LMGetMBarHeight();
}
*/

//--------------------------------------------------------------  GetPutDialogCorner
// Determines the upper left corner coordinates needed to properly center…
// the standard Mac GetFile dialog (when you open files).
/*
void GetGetDialogCorner (Point *theCorner)
{
	DialogTHndl	dlogHandle;
	Rect		theScreen, dlogBounds;
	Byte		wasState;
	
	theCorner->h = 64;
	theCorner->v = 64;
	theScreen = qd.screenBits.bounds;
	theScreen.top += LMGetMBarHeight();
	OffsetRect(&theScreen, -theScreen.left, -theScreen.top);
	
	dlogHandle = (DialogTHndl)GetResource('DLOG', sfGetDialogID);
	if (dlogHandle != nil)
	{
		wasState = HGetState((Handle)dlogHandle);
		HLock((Handle)dlogHandle);
		
		dlogBounds = (**dlogHandle).boundsRect;
		OffsetRect(&dlogBounds, -dlogBounds.left, -dlogBounds.top);
		
		theCorner->h = (theScreen.right - dlogBounds.right) / 2;
		theCorner->v = (theScreen.bottom - dlogBounds.bottom) / 3;
		
		HSetState((Handle)dlogHandle, wasState);
	}
	theCorner->v += LMGetMBarHeight();
}
*/
//--------------------------------------------------------------  CenterDialog
// Given a resource ID for a dialog, this function properly centers it.
/*
void CenterDialog (SInt16 dialogID)
{
	DialogTHndl	dlogHandle;
	Rect		theScreen, dlogBounds;
	SInt16		hPos, vPos;
	Byte		wasState;
	
	theScreen = qd.screenBits.bounds;
	theScreen.top += LMGetMBarHeight();
	
	dlogHandle = (DialogTHndl)GetResource('DLOG', dialogID);
	if (dlogHandle != nil)
	{
		wasState = HGetState((Handle)dlogHandle);
		HLock((Handle)dlogHandle);
		
		dlogBounds = (**dlogHandle).boundsRect;
		OffsetRect(&dlogBounds, -dlogBounds.left, -dlogBounds.top);
		
		hPos = ((theScreen.right - theScreen.left) - dlogBounds.right) / 2;
		vPos = ((theScreen.bottom - theScreen.top) - dlogBounds.bottom) / 3;
		
		OffsetRect(&dlogBounds, hPos, vPos + LMGetMBarHeight());
		
		(**dlogHandle).boundsRect = dlogBounds;
		HSetState((Handle)dlogHandle, wasState);
	}
}
*/
//--------------------------------------------------------------  GetDialogRect
// Determines the bounding rectangle for a given dialog.

void GetDialogRect (Rect *bounds, short dialogID)
{
	DialogTHndl	dlogHandle;
	Byte		wasState;
	
	dlogHandle = (DialogTHndl)GetResource('DLOG', dialogID);
	if (dlogHandle != nil)
	{
		wasState = HGetState((Handle)dlogHandle);
		HLock((Handle)dlogHandle);
		
		*bounds = (**dlogHandle).boundsRect;
		HSetState((Handle)dlogHandle, wasState);
	}
}

//--------------------------------------------------------------  TrueCenterDialog
// Places a dialog DEAD CENTER (as opposed to 1/3 of the way down as…
// is common for Mac dialog centering).
/*
void TrueCenterDialog (short dialogID)
{
	DialogTHndl	dlogHandle;
	Rect		theScreen, dlogBounds;
	short		hPos, vPos;
	Byte		wasState;
	
	theScreen = qd.screenBits.bounds;
	theScreen.top += LMGetMBarHeight();
	
	dlogHandle = (DialogTHndl)GetResource('DLOG', dialogID);
	if (dlogHandle != nil)
	{
		wasState = HGetState((Handle)dlogHandle);
		HLock((Handle)dlogHandle);
		
		dlogBounds = (**dlogHandle).boundsRect;
		OffsetRect(&dlogBounds, theScreen.left - dlogBounds.left, 
				theScreen.top - dlogBounds.top);
		
		hPos = ((theScreen.right - theScreen.left) - 
				(dlogBounds.right - dlogBounds.left)) / 2;
		vPos = ((theScreen.bottom - theScreen.top) - 
				(dlogBounds.bottom - dlogBounds.top)) / 2;
		
		OffsetRect(&dlogBounds, hPos, vPos + LMGetMBarHeight());
		
		(**dlogHandle).boundsRect = dlogBounds;
		HSetState((Handle)dlogHandle, wasState);
	}
}
*/
//--------------------------------------------------------------  CenterAlert
// Given an alert ID, this function properly centers it on the main monitor.
/*
void CenterAlert (short alertID)
{
	AlertTHndl	alertHandle;
	Rect		theScreen, alertRect;
	short		horiOff, vertOff;
	Byte		wasState;
	
	theScreen = qd.screenBits.bounds;
	theScreen.top += LMGetMBarHeight();
	
	alertHandle = (AlertTHndl)GetResource('ALRT', alertID);
	if (alertHandle != nil)
	{
		wasState = HGetState((Handle)alertHandle);
		HLock((Handle)alertHandle);
		
		alertRect = (**alertHandle).boundsRect;
		OffsetRect(&alertRect, -alertRect.left, -alertRect.top);
		
		horiOff = ((theScreen.right - theScreen.left) - alertRect.right) / 2;	
		vertOff = ((theScreen.bottom - theScreen.top) - alertRect.bottom) / 3;
		
		OffsetRect(&alertRect, horiOff, vertOff + LMGetMBarHeight());
		
		(**alertHandle).boundsRect = alertRect;
		HSetState((Handle)alertHandle, wasState);
	}
}
*/
//--------------------------------------------------------------  ZoomOutDialogRect

// Given a dialog, this function does the "zoom" animation to make the…
// the dialog appear to expand from nothingness or zoom in at you.
/*
void ZoomOutDialogRect (short dialogID)
{
	#define		kSteps		16
	#define		kZoomDelay	1
	DialogTHndl	dlogHandle;
	GrafPtr		wasPort, tempPort;
	Rect		dlogBounds, zoomRect;
	UInt32		dummyLong;
	Byte		wasState;
	short		wideStep, highStep, i;
	
	GetPort(&wasPort);
	
	tempPort = (GrafPtr)NewPtrClear(sizeof(GrafPort));
	OpenPort(tempPort);
	
	dlogHandle = (DialogTHndl)GetResource('DLOG', dialogID);
	if (dlogHandle != nil)
	{
		wasState = HGetState((Handle)dlogHandle);
		HLock((Handle)dlogHandle);
		dlogBounds = (**dlogHandle).boundsRect;
		HSetState((Handle)dlogHandle, wasState);
	}
	
	wideStep = ((dlogBounds.right - dlogBounds.left) / 2) / kSteps;
	highStep = ((dlogBounds.bottom - dlogBounds.top) / 2) / kSteps;
	
	SetRect(&zoomRect, dlogBounds.left + (wideStep * kSteps), 
			dlogBounds.top + (highStep * kSteps), 
			dlogBounds.right - (wideStep * kSteps), 
			dlogBounds.bottom - (highStep * kSteps));
	GlobalToLocalRect(&zoomRect);
	
	PenPat(GetQDGlobalsGray(&dummyPattern));
	PenMode(patXor);
	
	for (i = 0; i < kSteps; i++)
	{
		FrameRect(&zoomRect);
		Delay(kZoomDelay, &dummyLong);
		FrameRect(&zoomRect);
		InsetRect(&zoomRect, -wideStep, -highStep);
	}
	
	ClosePort(tempPort);
	
	SetPort((GrafPtr)wasPort);
}
*/
//--------------------------------------------------------------  ZoomOutAlertRect

// Like the above funciton but zooms out alerts instead of dialogs.
/*
void ZoomOutAlertRect (short alertID)
{
	#define		kSteps		16
	#define		kZoomDelay	1
	AlertTHndl	alertHandle;
	GrafPtr		wasPort, tempPort;
	Rect		alertBounds, zoomRect;
	UInt32		dummyLong;
	Byte		wasState;
	short		wideStep, highStep, i;
	
	GetPort(&wasPort);
	
	tempPort = (GrafPtr)NewPtrClear(sizeof(GrafPort));
	OpenPort(tempPort);
	
	alertHandle = (AlertTHndl)GetResource('ALRT', alertID);
	if (alertHandle != nil)
	{
		wasState = HGetState((Handle)alertHandle);
		HLock((Handle)alertHandle);
		alertBounds = (**alertHandle).boundsRect;
		HSetState((Handle)alertHandle, wasState);
	}
	
	wideStep = ((alertBounds.right - alertBounds.left) / 2) / kSteps;
	highStep = ((alertBounds.bottom - alertBounds.top) / 2) / kSteps;
	
	SetRect(&zoomRect, alertBounds.left + (wideStep * kSteps), 
			alertBounds.top + (highStep * kSteps), 
			alertBounds.right - (wideStep * kSteps), 
			alertBounds.bottom - (highStep * kSteps));
	GlobalToLocalRect(&zoomRect);
	
	PenPat(GetQDGlobalsGray(&dummyPattern));
	PenMode(patXor);
	
	for (i = 0; i < kSteps; i++)
	{
		FrameRect(&zoomRect);
		Delay(kZoomDelay, &dummyLong);
		FrameRect(&zoomRect);
		InsetRect(&zoomRect, -wideStep, -highStep);
	}
	
	ClosePort(tempPort);
	
	SetPort((GrafPtr)wasPort);
}
*/

//--------------------------------------------------------------  FlashDialogButton
// Flashes the default dialog button (item = 1) so as to make it appear…
// as though the user clicked on it.

void FlashDialogButton (DialogPtr theDialog, short itemNumber)
{
	Rect		itemRect;
	Handle		itemHandle;
	UInt32		dummyLong;
	short		itemType;
	
	GetDialogItem(theDialog, itemNumber, &itemType, &itemHandle, &itemRect);
	HiliteControl((ControlHandle)itemHandle, kControlButtonPart);
	Delay(8, &dummyLong);
	HiliteControl((ControlHandle)itemHandle, 0);
}

//--------------------------------------------------------------  DrawDefaultButton
// Draws a fat outline around the default item (item = 1).  This is the…
// item that is selected if the user hits the Return key.

void DrawDefaultButton (DialogPtr theDialog)
{
	Rect		itemRect;
	Handle		itemHandle;
	short		itemType;
	
	GetDialogItem(theDialog, 1, &itemType, &itemHandle, &itemRect);
	InsetRect(&itemRect, -4, -4);
	PenSize(3, 3);
	FrameRoundRect(&itemRect, 16, 16);
	PenNormal();
}

//--------------------------------------------------------------  GetDialogString
// Returns a string from a specific dialog item.

void GetDialogString (DialogPtr theDialog, short item, StringPtr theString)
{
	Rect		itemRect;
	Handle		itemHandle;
	short		itemType;
	
	GetDialogItem(theDialog, item, &itemType, &itemHandle, &itemRect);
	GetDialogItemText(itemHandle, theString);
}

//--------------------------------------------------------------  SetDialogString
// Sets a specific string to a specific dialog item.

void SetDialogString (DialogPtr theDialog, short item, StringPtr theString)
{
	Rect		itemRect;
	Handle		itemHandle;
	short		itemType;
	
	GetDialogItem(theDialog, item, &itemType, &itemHandle, &itemRect);
	SetDialogItemText(itemHandle, theString);
}

//--------------------------------------------------------------  GetDialogStringLen
// Returns the length of a dialog item string (text).

short GetDialogStringLen (DialogPtr theDialog, short item)
{
	Rect		itemRect;
	Str255		theString;
	Handle		itemHandle;
	short		itemType;
	
	GetDialogItem(theDialog, item, &itemType, &itemHandle, &itemRect);
	GetDialogItemText(itemHandle, theString);
	return (theString[0]);
}

//--------------------------------------------------------------  GetDialogItemValue
// Returns the value or "state" of a dialog item.  For checkboxes and…
// radio buttons, this may be a 1 or 0.

void GetDialogItemValue (DialogPtr theDialog, short item, short *theState)
{
	Rect		itemRect;
	Handle		itemHandle;
	short		itemType;
	
	GetDialogItem(theDialog, item, &itemType, &itemHandle, &itemRect);
	*theState = GetControlValue((ControlHandle)itemHandle);
}

//--------------------------------------------------------------  SetDialogItemValue
// Sets a specific dialogf items value or state (can set or clear…
// checkboxes, radio buttons, etc.).

void SetDialogItemValue (DialogPtr theDialog, short item, short theState)
{
	Rect		itemRect;
	Handle		itemHandle;
	short		itemType;
	
	GetDialogItem(theDialog, item, &itemType, &itemHandle, &itemRect);
	SetControlValue((ControlHandle)itemHandle, theState);
}

//--------------------------------------------------------------  ToggleDialogItemValue
// If item is a checkbox or radio button, its state is toggled.

void ToggleDialogItemValue (DialogPtr theDialog, short item)
{
	Rect		itemRect;
	Handle		itemHandle;
	short		itemType, theState;
	
	GetDialogItem(theDialog, item, &itemType, &itemHandle, &itemRect);
	theState = GetControlValue((ControlHandle)itemHandle);
	if (theState == 0)
		theState = 1;
	else
		theState = 0;
	SetControlValue((ControlHandle)itemHandle, theState);
}

//--------------------------------------------------------------  SetDialogNumToStr
// Function accepts an integer, converts it to a string and sets a…
// dialog items text to this string.

void SetDialogNumToStr (DialogPtr theDialog, short item, long theNumber)
{
	Str255		theString;
	Rect		itemRect;
	Handle		itemHandle;
	short		itemType;
	
	NumToString(theNumber, theString);
	GetDialogItem(theDialog, item, &itemType, &itemHandle, &itemRect);
	SetDialogItemText(itemHandle, theString);
}

//--------------------------------------------------------------  GetDialogNumFromStr
// Function extracts the text from a dialog item and converts it to an…
// integer for returning.

void GetDialogNumFromStr (DialogPtr theDialog, short item, long *theNumber)
{
	Str255		theString;
	Rect		itemRect;
	Handle		itemHandle;
	short		itemType;
	
	GetDialogItem(theDialog, item, &itemType, &itemHandle, &itemRect);
	GetDialogItemText(itemHandle, theString);
	StringToNum(theString, theNumber);
}

//--------------------------------------------------------------  GetDialogItemRect
// Returns the bounding rectangle of the specified dialog item.

void GetDialogItemRect (DialogPtr theDialog, short item, Rect *theRect)
{
	Handle		itemHandle;
	short		itemType;
	
	GetDialogItem(theDialog, item, &itemType, &itemHandle, theRect);
}

//--------------------------------------------------------------  SetDialogItemRect
// Sets the bounding rectangle of the specified dialog item.  Used to…
// resize or move a control.

void SetDialogItemRect (DialogPtr theDialog, short item, Rect *theRect)
{
	Rect		oldRect;
	Handle		itemHandle;
	short		itemType;
	
	GetDialogItem(theDialog, item, &itemType, &itemHandle, &oldRect);
	OffsetRect(&oldRect, theRect->left - oldRect.left, theRect->top - oldRect.top);
	SetDialogItem(theDialog, item, itemType, itemHandle, &oldRect);
}

//--------------------------------------------------------------  OffsetDialogItemRect
// Moves a dialog item by h and v.

void OffsetDialogItemRect (DialogPtr theDialog, short item, short h, short v)
{
	Rect		oldRect;
	Handle		itemHandle;
	short		itemType;
	
	GetDialogItem(theDialog, item, &itemType, &itemHandle, &oldRect);
	OffsetRect(&oldRect, h, v);
	SetDialogItem(theDialog, item, itemType, itemHandle, &oldRect);
}

//--------------------------------------------------------------  SelectFromRadioGroup
// Assuming a series of consecutively numbered radio buttons, this function…
// clears the whole range of them but sets the one specified (as though…
// the radio buttons are linked and only one can be set at a time).

void SelectFromRadioGroup (DialogPtr dial, short which, short first, short last)
{
	Rect		iRect;
	Handle		iHandle;
	short		iType, i;
	
	for (i = first; i <= last; i++)
	{
		GetDialogItem(dial, i, &iType, &iHandle, &iRect);
		SetControlValue((ControlHandle)iHandle, (short)false);
	}
	
	GetDialogItem(dial, which, &iType, &iHandle, &iRect);
	SetControlValue((ControlHandle)iHandle, (short)true);
}

//--------------------------------------------------------------  AddMenuToPopUp
// Assigns a menu handle to a pop-up dialog item - thus, giving that…
// pop-up item something to pop up.
/*
void AddMenuToPopUp (DialogPtr theDialog, short whichItem, MenuHandle theMenu)
{
	Rect		iRect;
	Handle		iHandle;
	short		iType;
	
	GetDialogItem(theDialog, whichItem, &iType, &iHandle, &iRect);
	(**(ControlHandle)iHandle).contrlRfCon = (long)theMenu;
}
*/
//--------------------------------------------------------------  GetPopUpMenuValu
// Returns which item is currently selected in a pop-up menu.

void GetPopUpMenuValue (DialogPtr theDialog, short whichItem, short *value)
{
	Rect		iRect;
	Handle		iHandle;
	short		iType;
	
	GetDialogItem(theDialog, whichItem, &iType, &iHandle, &iRect);
	*value = GetControlValue((ControlHandle)iHandle);
}

//--------------------------------------------------------------  SetPopUpMenuValue
// Forces a specific item to be set (as though selected) in a pop-up menu.

void SetPopUpMenuValue (DialogPtr theDialog, short whichItem, short value)
{
	Rect		iRect;
	Handle		iHandle;
	short		iType;
	
	GetDialogItem(theDialog, whichItem, &iType, &iHandle, &iRect);
	SetControlValue((ControlHandle)iHandle, value);
}

//--------------------------------------------------------------  MyEnableControl
// "Un-grays" or enables a dialog item (usually a button).

void MyEnableControl (DialogPtr theDialog, short whichItem)
{
	Rect		iRect;
	Handle		iHandle;
	short		iType;
	
	GetDialogItem(theDialog, whichItem, &iType, &iHandle, &iRect);
	HiliteControl((ControlHandle)iHandle, kActive);
}

//--------------------------------------------------------------  MyDisableControl
// "Grays out" or disables a dialog item (usually a button).

void MyDisableControl (DialogPtr theDialog, short whichItem)
{
	Rect		iRect;
	Handle		iHandle;
	short		iType;
	
	GetDialogItem(theDialog, whichItem, &iType, &iHandle, &iRect);
	HiliteControl((ControlHandle)iHandle, kInactive);
}

//--------------------------------------------------------------  DrawDialogUserText
// Given a string of text and an item, this function draws the string…
// within the bounding rect of the item.  Dialog item assumed to be…
// a "user item" (invisible item with only bounds).

void DrawDialogUserText (DialogPtr dial, short item, StringPtr text, Boolean invert)
{
	Rect		iRect;
	Handle		iHandle;
	Str255		newString, stringCopy;
	short		iType, textLong, i, inset;
	
	TextFont(applFont);
	TextSize(9);
	
	PasStringCopy(text, stringCopy);
	GetDialogItem(dial, item, &iType, &iHandle, &iRect);
	if ((StringWidth(stringCopy) + 2) > (iRect.right - iRect.left))
		CollapseStringToWidth(stringCopy, iRect.right - iRect.left - 2);
	textLong = stringCopy[0];
	for (i = 0; i < textLong; i++)
		newString[i] = stringCopy[i + 1];
	
	OffsetRect(&iRect, 0, 1);
	EraseRect(&iRect);
	OffsetRect(&iRect, 0, -1);
	
	inset = ((iRect.right - iRect.left) - (StringWidth(stringCopy) + 2)) / 2;
	iRect.left += inset;
	iRect.right -= inset;
	
	TETextBox(newString, textLong, &iRect, teCenter);
	if (invert)
	{
		OffsetRect(&iRect, 0, 1);
		InvertRect(&iRect);
	}
}

//--------------------------------------------------------------  DrawDialogUserText
// Similar to the above function but doesn't call TETextBox().  Instead,…
// it truncates the string (and appends "…") to the end in order that…
// the string fits within the dialog item's bounds.

void DrawDialogUserText2 (DialogPtr dial, short item, StringPtr text)
{
	Rect		iRect;
	Handle		iHandle;
	Str255		stringCopy;
	short		iType;
	
	TextFont(applFont);
	TextSize(9);
	
	PasStringCopy(text, stringCopy);
	GetDialogItem(dial, item, &iType, &iHandle, &iRect);
	if ((StringWidth(stringCopy) + 2) > (iRect.right - iRect.left))
		CollapseStringToWidth(stringCopy, iRect.right - iRect.left - 2);
	MoveTo(iRect.left, iRect.bottom);
	DrawString(stringCopy);
}

//--------------------------------------------------------------  LoadDialogPICT
// Draws a 'PICT' specified by ID within the bounds of the specified…
// dialog item.

void LoadDialogPICT (DialogPtr theDialog, short item, short theID)
{
	Rect		iRect;
	Handle		iHandle;
	PicHandle	thePict;
	short		iType;
	
	GetDialogItem(theDialog, item, &iType, &iHandle, &iRect);
	thePict = GetPicture(theID);
	if (thePict)
		DrawPicture(thePict, &iRect);
}

//--------------------------------------------------------------  FrameDialogItem
// Given a dialog item, this function draws a box around it.

void FrameDialogItem (DialogPtr theDialog, short item)
{
	Rect		itemRect;
	Handle		itemHandle;
	short		itemType;
	
	GetDialogItem(theDialog, item, &itemType, &itemHandle, &itemRect);
	FrameRect(&itemRect);
}

//--------------------------------------------------------------  FrameDialogItemC
// Given a dialog item, this function draws a color (specified) box around it.

void FrameDialogItemC (DialogPtr theDialog, short item, long color)
{
	RGBColor	theRGBColor, wasColor;
	Rect		itemRect;
	Handle		itemHandle;
	short		itemType;
	
	GetDialogItem(theDialog, item, &itemType, &itemHandle, &itemRect);
	GetForeColor(&wasColor);
	Index2Color(color, &theRGBColor);
	RGBForeColor(&theRGBColor);
	FrameRect(&itemRect);
	RGBForeColor(&wasColor);
}

//--------------------------------------------------------------  FrameOvalDialogItem
// Given a dialog item, this function draws an oval around it.

void FrameOvalDialogItem (DialogPtr theDialog, short item)
{
	Rect		itemRect;
	Handle		itemHandle;
	short		itemType;
	
	GetDialogItem(theDialog, item, &itemType, &itemHandle, &itemRect);
	FrameOval(&itemRect);
}

//--------------------------------------------------------------  BorderDialogItem
// Given a dialog item, this function draws any combination of 4 sides…
// of a box around it.  Which sides get drawn is encoded in "sides".

void BorderDialogItem (DialogPtr theDialog, short item, short sides)
{
	Rect		itemRect;
	Handle		itemHandle;
	short		itemType;
	
	// 1 = left
	// 2 = top
	// 4 = bottom
	// 8 = right ... so 6 = top & bottom, 15 = all 4 sides
	
	GetDialogItem(theDialog, item, &itemType, &itemHandle, &itemRect);
	
	if (sides >= 8)				// 8 = right
	{
		MoveTo(itemRect.right, itemRect.top);
		LineTo(itemRect.right, itemRect.bottom);
		sides -= 8;
	}
	if (sides >= 4)				// 4 = bottom
	{
		MoveTo(itemRect.left, itemRect.bottom);
		LineTo(itemRect.right, itemRect.bottom);
		sides -= 4;
	}
	if (sides >= 2)				// 2 = top
	{
		MoveTo(itemRect.left, itemRect.top - 1);
		LineTo(itemRect.right, itemRect.top - 1);
		sides -= 2;
	}
	if (sides >= 1)				// 1 = left
	{
		MoveTo(itemRect.left - 1, itemRect.top);
		LineTo(itemRect.left - 1, itemRect.bottom);
	}
}

//--------------------------------------------------------------  ShadowDialogItem
// Draws a drop shadow to the right and below a specified dialog item.

void ShadowDialogItem (DialogPtr theDialog, short item, short thickness)
{
	Rect		itemRect;
	Handle		itemHandle;
	short		itemType;
	
	GetDialogItem(theDialog, item, &itemType, &itemHandle, &itemRect);
	PenSize(thickness, thickness);
	MoveTo(itemRect.left + thickness, itemRect.bottom);
	Line(itemRect.right - itemRect.left - thickness, 0);
	MoveTo(itemRect.right, itemRect.top + thickness);
	Line(0, itemRect.bottom - itemRect.top - thickness);
	PenNormal();
}

//--------------------------------------------------------------  EraseDialogItem
// Erases (but doesn't physically remove) a dialog item.

void EraseDialogItem (DialogPtr theDialog, short item)
{
	Rect		itemRect;
	Handle		itemHandle;
	short		itemType;
	
	GetDialogItem(theDialog, item, &itemType, &itemHandle, &itemRect);
	EraseRect(&itemRect);
}

