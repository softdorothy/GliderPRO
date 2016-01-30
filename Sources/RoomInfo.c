
//============================================================================
//----------------------------------------------------------------------------
//								   RoomInfo.c
//----------------------------------------------------------------------------
//============================================================================


#include <NumberFormatting.h>
#include <Resources.h>
#include <Sound.h>
#include "DialogUtils.h"
#include "Externs.h"
#include "RectUtils.h"
#include "Utilities.h"


#define kRoomInfoDialogID			1003
#define kOriginalArtDialogID		1016
#define kNoPICTFoundAlert			1036
#define kRoomNameItem				3
#define kRoomLocationBox			6
#define kRoomTilesBox				10
#define kRoomPopupItem				11
#define kRoomDividerLine			12
#define kRoomTilesBox2				15
#define kRoomFirstCheck				17
#define kLitUnlitText				18
#define kMiniTileWide				16
#define kBoundsButton				19
#define kOriginalArtworkItem		19
#define kPICTIDItem					5
#define kFloorSupportCheck			12


void UpdateRoomInfoDialog (DialogPtr);
void DragMiniTile (Point, short *);
void HiliteTileOver (Point);
pascal Boolean RoomFilter (DialogPtr, EventRecord *, short *);
short ChooseOriginalArt (short);
void UpdateOriginalArt (DialogPtr);
pascal Boolean OriginalArtFilter (DialogPtr, EventRecord *, short *);
Boolean PictIDExists (short);
short GetFirstPICT (void);
void BitchAboutPICTNotFound (void);


Rect		tileSrc, tileDest, tileSrcRect, editTETextBox;
Rect		leftBound, topBound, rightBound, bottomBound;
CGrafPtr	tileSrcMap;
short		tempTiles[kNumTiles];
short		tileOver, tempBack, cursorIs;
Boolean		originalLeftOpen, originalTopOpen, originalRightOpen, originalBottomOpen;
Boolean		originalFloor;

extern	Cursor		handCursor, beamCursor;
extern	short		houseResFork, lastBackground;


//==============================================================  Functions
//--------------------------------------------------------------  UpdateRoomInfoDialog

#ifndef COMPILEDEMO
void UpdateRoomInfoDialog (DialogPtr theDialog)
{
	Rect		src, dest;
	short		i;
	
	DrawDialog(theDialog);
	if (tempBack >= kUserBackground)
		SetPopUpMenuValue(theDialog, kRoomPopupItem, kOriginalArtworkItem);
	else
		SetPopUpMenuValue(theDialog, kRoomPopupItem, 
				(tempBack - kBaseBackgroundID) + 1);
	
	
	
	CopyBits(GetPortBitMapForCopyBits(tileSrcMap), 
			GetPortBitMapForCopyBits(GetDialogPort(theDialog)), 
			&tileSrcRect, &tileSrc, srcCopy, nil);
	/*
	CopyBits(&((GrafPtr)tileSrcMap)->portBits, 
			&(((GrafPtr)theDialog)->portBits), 
			&tileSrcRect, &tileSrc, srcCopy, nil);
	*/
	dest = tileDest;
	dest.right = dest.left + kMiniTileWide;
	for (i = 0; i < kNumTiles; i++)
	{
		QSetRect(&src, 0, 0, kMiniTileWide, 80);
		QOffsetRect(&src, tempTiles[i] * kMiniTileWide, 0);
		
		CopyBits(GetPortBitMapForCopyBits(tileSrcMap), 
				GetPortBitMapForCopyBits(GetDialogPort(theDialog)), 
				&src, &dest, srcCopy, nil);
		/*
		CopyBits(&((GrafPtr)tileSrcMap)->portBits, 
				&(((GrafPtr)theDialog)->portBits), 
				&src, &dest, srcCopy, nil);
		*/
		QOffsetRect(&dest, kMiniTileWide, 0);
	}
	
	if (GetNumberOfLights(thisRoomNumber) == 0)
		SetDialogString(theDialog, kLitUnlitText, "\p(Room Is Dark)");
	else
		SetDialogString(theDialog, kLitUnlitText, "\p(Room Is Lit)");
	
	FrameDialogItemC(theDialog, kRoomLocationBox, kRedOrangeColor8);
	FrameDialogItem(theDialog, kRoomTilesBox);
	FrameDialogItemC(theDialog, kRoomDividerLine, kRedOrangeColor8);
	FrameDialogItem(theDialog, kRoomTilesBox2);
}
#endif

//--------------------------------------------------------------  DragMiniTile

#ifndef COMPILEDEMO
void DragMiniTile (Point mouseIs, short *newTileOver)
{
	Rect		dragRect;
	Point		mouseWas;
	short		wasTileOver;
	Pattern		dummyPattern;
	
	tileOver = (mouseIs.h - tileSrc.left) / kMiniTileWide;
	wasTileOver = -1;
	QSetRect(&dragRect, 0, 0, kMiniTileWide, 80);
	QOffsetRect(&dragRect, 
			tileSrc.left + (tileOver * kMiniTileWide), 
			tileSrc.top);
	PenMode(patXor);
	PenPat(GetQDGlobalsGray(&dummyPattern));
	FrameRect(&dragRect);
	mouseWas = mouseIs;
	while (WaitMouseUp())							// loop until mouse button let up
	{
		GetMouse(&mouseIs);							// get mouse coords
		if (DeltaPoint(mouseWas, mouseIs) != 0L)	// the mouse has moved
		{
			FrameRect(&dragRect);
			QOffsetRect(&dragRect, mouseIs.h - mouseWas.h, 0);
			FrameRect(&dragRect);
			
			if (PtInRect(mouseIs, &tileDest))		// is cursor in the drop rect
			{
				*newTileOver = (mouseIs.h - tileDest.left) / kMiniTileWide;
				if (*newTileOver != wasTileOver)
				{
					PenNormal();
					PenSize(1, 2);
					ForeColor(blueColor);
					MoveTo(tileDest.left + (*newTileOver * kMiniTileWide), 
							tileDest.top - 3);
					Line(kMiniTileWide, 0);
					MoveTo(tileDest.left + (*newTileOver * kMiniTileWide), 
							tileDest.bottom + 1);
					Line(kMiniTileWide, 0);
					
					if (wasTileOver != -1)
					{
						ForeColor(whiteColor);
						MoveTo(tileDest.left + (wasTileOver * kMiniTileWide), 
								tileDest.top - 3);
						Line(kMiniTileWide, 0);
						MoveTo(tileDest.left + (wasTileOver * kMiniTileWide), 
								tileDest.bottom + 1);
						Line(kMiniTileWide, 0);
					}
					ForeColor(blackColor);
					PenNormal();
					PenMode(patXor);
					PenPat(GetQDGlobalsGray(&dummyPattern));
					wasTileOver = *newTileOver;
				}
			}
			else
			{
				*newTileOver = -1;					// we're not in the drop zone
				if (wasTileOver != -1)
				{
					PenNormal();
					PenSize(1, 2);
					ForeColor(whiteColor);
					MoveTo(tileDest.left + (wasTileOver * kMiniTileWide), 
							tileDest.top - 3);
					Line(kMiniTileWide, 0);
					MoveTo(tileDest.left + (wasTileOver * kMiniTileWide), 
							tileDest.bottom + 1);
					Line(kMiniTileWide, 0);
					ForeColor(blackColor);
					PenNormal();
					PenMode(patXor);
					PenPat(GetQDGlobalsGray(&dummyPattern));
					wasTileOver = -1;
				}
			}
			
			mouseWas = mouseIs;
		}
	}
	if (wasTileOver != -1)
	{
		PenNormal();
		PenSize(1, 2);
		ForeColor(whiteColor);
		MoveTo(tileDest.left + (wasTileOver * kMiniTileWide), tileDest.top - 3);
		Line(kMiniTileWide, 0);
		MoveTo(tileDest.left + (wasTileOver * kMiniTileWide), tileDest.bottom + 1);
		Line(kMiniTileWide, 0);
		ForeColor(blackColor);
		PenNormal();
		PenMode(patXor);
		PenPat(GetQDGlobalsGray(&dummyPattern));
		wasTileOver = -1;
	}
	FrameRect(&dragRect);
	PenNormal();
}
#endif

//--------------------------------------------------------------  HiliteTileOver

#ifndef COMPILEDEMO
void HiliteTileOver (Point mouseIs)
{
	short		newTileOver;
	
	if (PtInRect(mouseIs, &tileSrc))
	{
		if (cursorIs != kHandCursor)
		{
			SetCursor(&handCursor);
			cursorIs = kHandCursor;
		}
		
		newTileOver = (mouseIs.h - tileSrc.left) / kMiniTileWide;
		if (newTileOver != tileOver)
		{
			PenSize(1, 2);
			ForeColor(redColor);
			MoveTo(tileSrc.left + (newTileOver * kMiniTileWide), tileSrc.top - 3);
			Line(kMiniTileWide, 0);
			MoveTo(tileSrc.left + (newTileOver * kMiniTileWide), tileSrc.bottom + 1);
			Line(kMiniTileWide, 0);
			
			if (tileOver != -1)
			{
				ForeColor(whiteColor);
				MoveTo(tileSrc.left + (tileOver * kMiniTileWide), tileSrc.top - 3);
				Line(kMiniTileWide, 0);
				MoveTo(tileSrc.left + (tileOver * kMiniTileWide), tileSrc.bottom + 1);
				Line(kMiniTileWide, 0);
			}
			ForeColor(blackColor);
			PenNormal();
			
			tileOver = newTileOver;
		}
	}
	else
	{
		if (tileOver != -1)
		{
			PenSize(1, 2);
			ForeColor(whiteColor);
			MoveTo(tileSrc.left + (tileOver * kMiniTileWide), tileSrc.top - 3);
			Line(kMiniTileWide, 0);
			MoveTo(tileSrc.left + (tileOver * kMiniTileWide), tileSrc.bottom + 1);
			Line(kMiniTileWide, 0);
			ForeColor(blackColor);
			PenNormal();
			tileOver = -1;
		}
		
		if (PtInRect(mouseIs, &editTETextBox))
		{
			if (cursorIs != kBeamCursor)
			{
				SetCursor(&beamCursor);
				cursorIs = kBeamCursor;
			}
		}
		else
		{
			if (cursorIs != kArrowCursor)
			{
				InitCursor();
				cursorIs = kArrowCursor;
			}
		}
	}
}
#endif

//--------------------------------------------------------------  RoomFilter
#ifndef COMPILEDEMO

pascal Boolean RoomFilter (DialogPtr dial, EventRecord *event, short *item)
{
	Point		mouseIs;
	short		newTileOver;
	
	switch (event->what)
	{
		case keyDown:
		switch ((event->message) & charCodeMask)
		{
			case kReturnKeyASCII:
			case kEnterKeyASCII:
			FlashDialogButton(dial, kOkayButton);
			*item = kOkayButton;
			return(true);
			break;
			
			case kEscapeKeyASCII:
			FlashDialogButton(dial, kCancelButton);
			*item = kCancelButton;
			return(true);
			break;
			
			case kTabKeyASCII:
			SelectDialogItemText(dial, kRoomNameItem, 0, 1024);
			return(true);
			break;
			
			default:
			return(false);
		}
		break;
		
		case mouseDown:
		mouseIs = event->where;
		GlobalToLocal(&mouseIs);		
		if (PtInRect(mouseIs, &tileSrc))
		{
			if (StillDown())
			{
				DragMiniTile(mouseIs, &newTileOver);
				if ((newTileOver >= 0) && (newTileOver < kNumTiles))
				{
					tempTiles[newTileOver] = tileOver;
					UpdateRoomInfoDialog(dial);
				}
			}
			return(true);
		}
		else
			return(false);
		break;
		
		case mouseUp:
		return(false);
		break;
		
		case updateEvt:
		SetPort((GrafPtr)dial);
		BeginUpdate(GetDialogWindow(dial));
		UpdateRoomInfoDialog(dial);
		EndUpdate(GetDialogWindow(dial));
		event->what = nullEvent;
		return(false);
		break;
		
		default:
		GetMouse(&mouseIs);
		HiliteTileOver(mouseIs);
		return(false);
		break;
	}
}
#endif

//--------------------------------------------------------------  DoRoomInfo

void DoRoomInfo (void)
{
#ifndef COMPILEDEMO
	#define			kBackgroundsMenuID		140
	DialogPtr		roomInfoDialog;
	MenuHandle		backgroundsMenu;
	Str255			floorStr, suiteStr, objectsStr, tempStr;
	short			item, i, newBack;
	char			wasState;
	Boolean			leaving, wasFirstRoom, forceDraw;
	ModalFilterUPP	roomFilterUPP;
	CGrafPtr	wasCPort;
	GDHandle	wasWorld;
	OSErr		theErr;
	
	GetGWorld(&wasCPort, &wasWorld);
	roomFilterUPP = NewModalFilterUPP(RoomFilter);
	
	tileOver = -1;
	cursorIs = kArrowCursor;
	tempBack = thisRoom->background;
	backgroundsMenu = GetMenu(kBackgroundsMenuID);
//	SetMenuItemTextStyle(backgroundsMenu, kOriginalArtworkItem, italic);
	if (HouseHasOriginalPicts())
		EnableMenuItem(backgroundsMenu, kOriginalArtworkItem);
	
	NumToString(thisRoom->floor, floorStr);
	NumToString(thisRoom->suite, suiteStr);
	NumToString(thisRoom->numObjects, objectsStr);
	ParamText(floorStr, suiteStr, objectsStr, "\p");
	
	theErr = CreateOffScreenGWorld(&tileSrcMap, &tileSrcRect, kPreferredDepth);
	SetGWorld(tileSrcMap, nil);
//	CreateOffScreenPixMap(&tileSrcRect, &tileSrcMap);
//	SetPort((GrafPtr)tileSrcMap);
	if ((tempBack > kStars) && (!PictIDExists(tempBack)))
	{
		BitchAboutPICTNotFound();
		tempBack = kSimpleRoom;
	}
	if ((tempBack == 2002) || (tempBack == 2011) || 
			(tempBack == 2016) || (tempBack == 2017))
		LoadScaledGraphic(tempBack - 800, &tileSrcRect);
	else
		LoadScaledGraphic(tempBack, &tileSrcRect);
	
	SetGWorld(wasCPort, wasWorld);
	
	for (i = 0; i < kNumTiles; i++)
		tempTiles[i] = thisRoom->tiles[i];
	
//	CenterDialog(kRoomInfoDialogID);
	roomInfoDialog = GetNewDialog(kRoomInfoDialogID, nil, kPutInFront);
	if (roomInfoDialog == nil)
		RedAlert(kErrDialogDidntLoad);
	SetPort((GrafPtr)roomInfoDialog);
	
	// Fix this later.  TEMP
//	AddMenuToPopUp(roomInfoDialog, kRoomPopupItem, backgroundsMenu);
	if (tempBack >= kUserBackground)
		SetPopUpMenuValue(roomInfoDialog, kRoomPopupItem, kOriginalArtworkItem);
	else
		SetPopUpMenuValue(roomInfoDialog, kRoomPopupItem, 
				(tempBack - kBaseBackgroundID) + 1);
	SetDialogString(roomInfoDialog, kRoomNameItem, thisRoom->name);
	GetDialogItemRect(roomInfoDialog, kRoomTilesBox, &tileSrc);
	GetDialogItemRect(roomInfoDialog, kRoomTilesBox2, &tileDest);
	GetDialogItemRect(roomInfoDialog, kRoomNameItem, &editTETextBox);
	SelectDialogItemText(roomInfoDialog, kRoomNameItem, 0, 1024);
	
	ShowWindow(GetDialogWindow(roomInfoDialog));
	DrawDefaultButton(roomInfoDialog);
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	wasFirstRoom = ((*thisHouse)->firstRoom == thisRoomNumber);
	HSetState((Handle)thisHouse, wasState);
	SetDialogItemValue(roomInfoDialog, kRoomFirstCheck, (short)wasFirstRoom);
	
	if (tempBack >= kUserBackground)
		MyEnableControl(roomInfoDialog, kBoundsButton);
	else
		MyDisableControl(roomInfoDialog, kBoundsButton);
	
	leaving = false;
	
	while (!leaving)
	{
		ModalDialog(roomFilterUPP, &item);
		
		if (item == kOkayButton)
		{
			for (i = 0; i < kNumTiles; i++)
				thisRoom->tiles[i] = tempTiles[i];
			
			GetDialogString(roomInfoDialog, kRoomNameItem, tempStr);
			PasStringCopyNum(tempStr, thisRoom->name, 27);
			if (wasFirstRoom)
			{
				HLock((Handle)thisHouse);
				(*thisHouse)->firstRoom = thisRoomNumber;
				HUnlock((Handle)thisHouse);
			}
			thisRoom->background = tempBack;
			if (tempBack < kUserBackground)
				lastBackground = tempBack;
			CopyThisRoomToRoom();
			ReflectCurrentRoom(false);
			fileDirty = true;
			UpdateMenus(false);
			leaving = true;
		}
		else if (item == kCancelButton)
		{
			leaving = true;
		}
		else if (item == kRoomFirstCheck)
		{
			wasFirstRoom = !wasFirstRoom;
			SetDialogItemValue(roomInfoDialog, kRoomFirstCheck, (short)wasFirstRoom);
		}
		else if (item == kRoomPopupItem)
		{
			GetPopUpMenuValue(roomInfoDialog, kRoomPopupItem, &newBack);
			if (newBack == kOriginalArtworkItem)				// original art item selected?
			{
				if (tempBack < kUserBackground)					// was previous bg built-in?
				{
					tempBack = GetFirstPICT();					// then assign 1st PICT
					forceDraw = true;
				}
				else
					forceDraw = false;
				newBack = ChooseOriginalArt(tempBack);			// bring up dialog
				if ((tempBack != newBack) || (forceDraw))
				{
					tempBack = newBack;
					SetPort((GrafPtr)tileSrcMap);
					LoadScaledGraphic(tempBack, &tileSrcRect);
					InvalWindowRect(GetDialogWindow(roomInfoDialog), &tileSrc);
					InvalWindowRect(GetDialogWindow(roomInfoDialog), &tileDest);
				}
			}
			else
			{
				newBack += (kBaseBackgroundID - 1);		// adjust to get real PICT ID
				if (newBack != tempBack)				// if background has changed
					SetInitialTiles(newBack, false);
			}
			
			if (newBack >= kUserBackground)
			{
				MyEnableControl(roomInfoDialog, kBoundsButton);
				if (newBack != tempBack)				// if background has changed
					SetInitialTiles(newBack, false);
			}
			else
				MyDisableControl(roomInfoDialog, kBoundsButton);
			
			if (newBack != tempBack)
			{
				tempBack = newBack;
				SetPort((GrafPtr)tileSrcMap);
				if ((tempBack == 2002) || (tempBack == 2011) || 
						(tempBack == 2016) || (tempBack == 2017))
					LoadScaledGraphic(tempBack - 800, &tileSrcRect);
				else
					LoadScaledGraphic(tempBack, &tileSrcRect);
				InvalWindowRect(GetDialogWindow(roomInfoDialog), &tileSrc);
				InvalWindowRect(GetDialogWindow(roomInfoDialog), &tileDest);
			}
		}
		else if (item == kBoundsButton)
		{
			newBack = ChooseOriginalArt(tempBack);
			if (tempBack != newBack)
			{
				tempBack = newBack;
				SetPort((GrafPtr)tileSrcMap);
				LoadScaledGraphic(tempBack, &tileSrcRect);
				InvalWindowRect(GetDialogWindow(roomInfoDialog), &tileSrc);
				InvalWindowRect(GetDialogWindow(roomInfoDialog), &tileDest);
			}
		}
	}
	
	InitCursor();
	DisposeDialog(roomInfoDialog);
	DisposeModalFilterUPP(roomFilterUPP);
	
//	KillOffScreenPixMap(tileSrcMap);
	DisposeGWorld(tileSrcMap);
	tileSrcMap = nil;
#endif
}

//--------------------------------------------------------------  UpdateOriginalArt

#ifndef COMPILEDEMO
void UpdateOriginalArt (DialogPtr theDialog)
{
	Pattern		dummyPattern;
	
	DrawDialog(theDialog);
	DrawDefaultButton(theDialog);
	
	PenSize(2, 1);
	if (!originalLeftOpen)
		BorderDialogItem(theDialog, 7, 8);
	else
	{
		PenPat(GetQDGlobalsGray(&dummyPattern));
		BorderDialogItem(theDialog, 7, 8);
		PenPat(GetQDGlobalsBlack(&dummyPattern));
	}
	
	PenSize(1, 2);
	if (!originalTopOpen)
		BorderDialogItem(theDialog, 8, 4);
	else
	{
		PenPat(GetQDGlobalsGray(&dummyPattern));
		BorderDialogItem(theDialog, 8, 4);
		PenPat(GetQDGlobalsBlack(&dummyPattern));
	}
	
	PenSize(2, 1);
	if (!originalRightOpen)
		BorderDialogItem(theDialog, 9, 1);
	else
	{
		PenPat(GetQDGlobalsGray(&dummyPattern));
		BorderDialogItem(theDialog, 9, 1);
		PenPat(GetQDGlobalsBlack(&dummyPattern));
	}
	
	PenSize(1, 2);
	if (!originalBottomOpen)
		BorderDialogItem(theDialog, 10, 2);
	else
	{
		PenPat(GetQDGlobalsGray(&dummyPattern));
		BorderDialogItem(theDialog, 10, 2);
		PenPat(GetQDGlobalsBlack(&dummyPattern));
	}
	
	PenSize(1, 1);
}
#endif

//--------------------------------------------------------------  OriginalArtFilter
#ifndef COMPILEDEMO

pascal Boolean OriginalArtFilter (DialogPtr dial, EventRecord *event, short *item)
{
	Point		mouseIs;
	
	switch (event->what)
	{
		case keyDown:
		switch ((event->message) & charCodeMask)
		{
			case kReturnKeyASCII:
			case kEnterKeyASCII:
			FlashDialogButton(dial, kOkayButton);
			*item = kOkayButton;
			return(true);
			break;
			
			case kEscapeKeyASCII:
			FlashDialogButton(dial, kCancelButton);
			*item = kCancelButton;
			return(true);
			break;
			
			case kTabKeyASCII:
			SelectDialogItemText(dial, kPICTIDItem, 0, 1024);
			return(true);
			break;
			
			default:
			return(false);
		}
		break;
		
		case mouseDown:
		mouseIs = event->where;
		GlobalToLocal(&mouseIs);		
		if (PtInRect(mouseIs, &leftBound))
		{
			*item = 7;
			return(true);
		}
		else if (PtInRect(mouseIs, &topBound))
		{
			*item = 8;
			return(true);
		}
		else if (PtInRect(mouseIs, &rightBound))
		{
			*item = 9;
			return(true);
		}
		else if (PtInRect(mouseIs, &bottomBound))
		{
			*item = 10;
			return(true);
		}
		else
			return(false);
		break;
		
		case mouseUp:
		return(false);
		break;
		
		case updateEvt:
		SetPort((GrafPtr)dial);
		BeginUpdate(GetDialogWindow(dial));
		UpdateOriginalArt(dial);
		EndUpdate(GetDialogWindow(dial));
		event->what = nullEvent;
		return(false);
		break;
		
		default:
		return(false);
		break;
	}
}
#endif

//--------------------------------------------------------------  ChooseOriginalArt

#ifndef COMPILEDEMO
short ChooseOriginalArt (short was)
{
	DialogPtr		theDialog;
	long			longID;
	short			item, newPictID, tempShort, wasPictID;
	Boolean			leaving;
	ModalFilterUPP	originalArtFilterUPP;
	
	originalArtFilterUPP = NewModalFilterUPP(OriginalArtFilter);
	
	if (was < kUserBackground)
		was = kUserBackground;
	
	InitCursor();
	BringUpDialog(&theDialog, kOriginalArtDialogID);
	if (was >= kOriginalArtworkItem)
	{
		newPictID = was;
		wasPictID = was;
	}
	else
	{
		newPictID = kUserBackground;
		wasPictID = 0;
	}
	SetDialogNumToStr(theDialog, kPICTIDItem, (long)newPictID);
	SelectDialogItemText(theDialog, kPICTIDItem, 0, 16);
	
	GetDialogItemRect(theDialog, 7, &leftBound);
	GetDialogItemRect(theDialog, 8, &topBound);
	GetDialogItemRect(theDialog, 9, &rightBound);
	GetDialogItemRect(theDialog, 10, &bottomBound);
	
	tempShort = thisRoom->bounds >> 1;			// version 2.0 house
	originalLeftOpen = ((tempShort & 1) == 1);
	originalTopOpen = ((tempShort & 2) == 2);
	originalRightOpen = ((tempShort & 4) == 4);
	originalBottomOpen = ((tempShort & 8) == 8);
	originalFloor = ((tempShort & 16) == 16);
	
	SetDialogItemValue(theDialog, kFloorSupportCheck, (short)originalFloor);
	
	leaving = false;
	
	while (!leaving)
	{
		ModalDialog(originalArtFilterUPP, &item);
		
		if (item == kOkayButton)
		{
			GetDialogNumFromStr(theDialog, kPICTIDItem, &longID);
			if ((longID >= 3000) && (longID < 3800) && (PictIDExists((short)longID)))
			{
				newPictID = (short)longID;
				if (newPictID != wasPictID)
					SetInitialTiles(tempBack, false);
				tempShort = 0;
				if (originalLeftOpen)
					tempShort += 1;
				if (originalTopOpen)
					tempShort += 2;
				if (originalRightOpen)
					tempShort += 4;
				if (originalBottomOpen)
					tempShort += 8;
				if (originalFloor)
					tempShort += 16;
				tempShort = tempShort << 1;		// shift left 1 bit
				tempShort += 1;					// flag that says orginal bounds used
				thisRoom->bounds = tempShort;
				leaving = true;
			}
			else
			{
				SysBeep(1);
				SetDialogNumToStr(theDialog, kPICTIDItem, (long)newPictID);
			}
		}
		else if (item == kCancelButton)
		{
			newPictID = was;
			leaving = true;
		}
		else if (item == 7)
		{
			originalLeftOpen = !originalLeftOpen;
			UpdateOriginalArt(theDialog);
		}
		else if (item == 8)
		{
			originalTopOpen = !originalTopOpen;
			UpdateOriginalArt(theDialog);
		}
		else if (item == 9)
		{
			originalRightOpen = !originalRightOpen;
			UpdateOriginalArt(theDialog);
		}
		else if (item == 10)
		{
			originalBottomOpen = !originalBottomOpen;
			UpdateOriginalArt(theDialog);
		}
		else if (item == kFloorSupportCheck)
		{
			originalFloor = !originalFloor;
			ToggleDialogItemValue(theDialog, kFloorSupportCheck);
		}
	}
	
	DisposeDialog(theDialog);
	DisposeModalFilterUPP(originalArtFilterUPP);
	
	return (newPictID);
}
#endif

//--------------------------------------------------------------  PictIDExists

Boolean PictIDExists (short theID)
{
	PicHandle	thePicture;
//	Handle		resHandle;
//	Str255		resName;
//	ResType		resType;
//	short		numPicts, i;
//	short		resID;
	Boolean		foundIt;
	
	foundIt = true;
	
	thePicture = GetPicture(theID);
	if (thePicture == nil)
	{
		thePicture = (PicHandle)GetResource('Date', theID);
		if (thePicture == nil)
		{
			foundIt = false;
		}
		else
			ReleaseResource((Handle)thePicture);
	}
	else
		ReleaseResource((Handle)thePicture);
	
//	foundIt = false;
//	numPicts = Count1Resources('PICT');
//	for (i = 1; i <= numPicts; i++)
//	{
//		resHandle = Get1IndResource('PICT', i);
//		if (resHandle != nil)
//		{
//			GetResInfo(resHandle, &resID, &resType, resName);
//			ReleaseResource(resHandle);
//			if (resID == theID)
//			{
//				foundIt = true;
//				break;
//			}
//		}
//	}
	
	return (foundIt);
}

//--------------------------------------------------------------  GetFirstPICT

short GetFirstPICT (void)
{
	Handle		resHandle;
	Str255		resName;
	ResType		resType;
	short		resID;
	
	resHandle = Get1IndResource('PICT', 1);
	if (resHandle != nil)
	{
		GetResInfo(resHandle, &resID, &resType, resName);
		ReleaseResource(resHandle);
		return (resID);
	}
	else
		return (-1);
}

//--------------------------------------------------------------  BitchAboutPICTNotFound

#ifndef COMPILEDEMO
void BitchAboutPICTNotFound (void)
{
	short		hitWhat;
	
//	CenterAlert(kNoPICTFoundAlert);
	hitWhat = Alert(kNoPICTFoundAlert, nil);
}
#endif

