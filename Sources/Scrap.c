
//============================================================================
//----------------------------------------------------------------------------
//								    Scrap.c
//----------------------------------------------------------------------------
//============================================================================

/*
#include "Externs.h"
#include "Environ.h"
#include <Drag.h>


Boolean DropLocationIsTrash (AEDesc *);


Boolean		hasScrap, scrapIsARoom;

extern	WindowPtr	mapWindow;
extern	Rect		roomObjectRects[];
extern	short		objActive;


//==============================================================  Functions
//--------------------------------------------------------------  PutRoomScrap

#ifndef COMPILEDEMO
void PutRoomScrap (void)
{
	// this function copies the current room into the clipboard
	Rect		largeBounds, smallBounds;
	PicHandle	smallPict;
	long		theErr;
	
	theErr = ZeroScrap();
	if (theErr == noErr)
	{
		SetRect(&largeBounds, 0, 0, kRoomWide, kTileHigh);
		SetRect(&smallBounds, 0, 0, kRoomWide / 4, kTileHigh / 4);
		smallPict = OpenPicture(&smallBounds);
		CopyBits(&(((GrafPtr)mainWindow)->portBits), &(((GrafPtr)mainWindow)->portBits), 
				&largeBounds, &smallBounds, srcCopy, nil);
		ClosePicture();
		
		HLock((Handle)smallPict);
		theErr = PutScrap(GetHandleSize((Handle)smallPict), 'PICT', (Ptr)(*smallPict));
		theErr = PutScrap(sizeof(roomType), 'Room', (Ptr)thisRoom);
		if (theErr == noErr)
		{
			if (!hasScrap)
			{
				hasScrap = true;
				UpdateMenus(false);
			}
			scrapIsARoom = true;
		}
		else
			YellowAlert(kYellowScrapError, theErr);
		
		KillPicture(smallPict);
	}
	else
		YellowAlert(kYellowScrapError, theErr);
}
#endif

//--------------------------------------------------------------  PutObjectScrap

#ifndef COMPILEDEMO
void PutObjectScrap (void)
{
	// this function copies the currently selected object into the clipboard
	Str255		kindStr;
	objectPtr	scrapObjPtr;
	long		theErr;
	
	theErr = ZeroScrap();
	if (theErr == noErr)
	{
		GetIndString(kindStr, kObjectNameStrings, thisRoom->objects[objActive].what);
		theErr = PutScrap(kindStr[0], 'TEXT', (Ptr)(kindStr + 1));
		scrapObjPtr = &(thisRoom->objects[objActive]);
		theErr = PutScrap(sizeof(objectType), 'Obj.', (Ptr)scrapObjPtr);
		if (theErr == noErr)
		{
			if (!hasScrap)
			{
				hasScrap = true;
				UpdateMenus(false);
			}
			scrapIsARoom = false;
		}
		else
			YellowAlert(kYellowScrapError, theErr);
	}
	else
		YellowAlert(kYellowScrapError, theErr);
}
#endif

//--------------------------------------------------------------  GetRoomScrap

#ifndef COMPILEDEMO
void GetRoomScrap (void)
{
	// this function pastes a room from the clipboard
	Handle		tempRoom;
	long		theErr, scrapOffset;
	short		wasFloor, wasSuite, srcRoomNumber, destRoomNumber, i;
	short		linkRoomNumber;
	
	tempRoom = NewHandle(0L);
	if (tempRoom == nil)
	{
		YellowAlert(kYellowNoMemory, 0);
		return;
	}
	
	theErr = GetScrap(tempRoom, 'Room', &scrapOffset);
	if (theErr < 0)
		YellowAlert(kYellowScrapError, theErr);
	else
	{
		DeselectObject();
		
		wasFloor = thisRoom->floor;
		wasSuite = thisRoom->suite;
		destRoomNumber = GetRoomNumber(thisRoom->floor, thisRoom->suite);
		HLock(tempRoom);
		BlockMove(*tempRoom, (Ptr)thisRoom, sizeof(roomType));
		HUnlock(tempRoom);
		DisposeHandle(tempRoom);
		srcRoomNumber = GetRoomNumber(thisRoom->floor, thisRoom->suite);
		thisRoom->floor = wasFloor;
		thisRoom->suite = wasSuite;
		
		for (i = 0; i < kMaxRoomObs; i++)		// fix links
		{										// first see if a linkable object
			if ((ObjectIsLinkTransport(&thisRoom->objects[i])) || 
					(ObjectIsLinkSwitch(&thisRoom->objects[i])))
			{
				linkRoomNumber = GetRoomLinked (&thisRoom->objects[i]);
				if (linkRoomNumber == srcRoomNumber)
				{								// if linked to an object in same roomÉ
					if (ObjectIsLinkSwitch(&thisRoom->objects[i]))
					{							// point to new room location
						thisRoom->objects[i].data.d.where = 
								(wasSuite * 100) + wasFloor + kNumUndergroundFloors;
					}
					else
					{							// point to new room location
						thisRoom->objects[i].data.e.where = 
								(wasSuite * 100) + wasFloor + kNumUndergroundFloors;
					}
				}
			}
		}
		
		CopyThisRoomToRoom();
		ReflectCurrentRoom(false);
		fileDirty = true;
		UpdateMenus(false);
	}
}
#endif

//--------------------------------------------------------------  GetObjectScrap

#ifndef COMPILEDEMO
void GetObjectScrap (void)
{
	// this function pastes an object from the clipboard
	objectType	tempObject;
	Handle		tempObjectHand;
	Point		noPoint;
	long		theErr, scrapOffset;
	short		direction, dist;
	
	tempObjectHand = NewHandle(0L);
	if (tempObjectHand == nil)
	{
		YellowAlert(kYellowNoMemory, 0);
		return;
	}
	
	theErr = GetScrap(tempObjectHand, 'Obj.', &scrapOffset);
	if (theErr < 0)
		YellowAlert(kYellowScrapError, theErr);
	else
	{
		DeselectObject();
		
		HLock(tempObjectHand);
		noPoint.h = 100;
		noPoint.v = 100;
		BlockMove(*tempObjectHand, (Ptr)(&tempObject), sizeof(objectType));
		if (AddNewObject(noPoint, tempObject.what, false))
		{
			thisRoom->objects[objActive] = tempObject;
			ReadyBackground(thisRoom->background, thisRoom->tiles);
			GetThisRoomsObjRects();
			DrawThisRoomsObjects();
			SetPort((GrafPtr)mainWindow);
			InvalRect(&mainWindowRect);
			if (ObjectHasHandle(&direction, &dist))
			{
				StartMarqueeHandled(&roomObjectRects[objActive], direction, dist);
				HandleBlowerGlider();
			}
			else
				StartMarquee(&roomObjectRects[objActive]);
		}
		HUnlock(tempObjectHand);
		DisposeHandle(tempObjectHand);
	}
}
#endif

//--------------------------------------------------------------  SeeIfValidScrapAvailable

#ifndef COMPILEDEMO
void SeeIfValidScrapAvailable (Boolean updateMenus)
{
	Handle		tempRoom, tempObject;
	long		theErr, scrapOffset;
	
	hasScrap = false;
	
	tempRoom = NewHandle(0L);
	if (tempRoom != nil)
	{
		theErr = GetScrap(tempRoom, 'Room', &scrapOffset);
		if (theErr >= 0)
		{
			hasScrap = true;
			scrapIsARoom = true;
		}
		DisposeHandle(tempRoom);
	}
	
	tempObject = NewHandle(0L);
	if (tempObject != nil)
	{
		theErr = GetScrap(tempObject, 'Obj.', &scrapOffset);
		if (theErr >= 0)
		{
			hasScrap = true;
			scrapIsARoom = false;
		}
		DisposeHandle(tempObject);
	}
	
	if (updateMenus)
		UpdateClipboardMenus();
}
#endif

//--------------------------------------------------------------  DropLocationIsTrash

Boolean DropLocationIsTrash (AEDesc *dropLocation)
{
	AEDesc		dropSpec;
	FSSpec		*theSpec;
	CInfoPBRec	thePB;
	long		trashDirID;
	OSErr		theErr;
	short		trashVRefNum;
	
	if ((dropLocation->descriptorType != typeNull) &&
			(AECoerceDesc(dropLocation, typeFSS, &dropSpec) == noErr))
	{
		HLock(dropSpec.dataHandle);
		theSpec = (FSSpec *) *dropSpec.dataHandle;
		
		thePB.dirInfo.ioCompletion = 0L;
		thePB.dirInfo.ioNamePtr = (StringPtr) &theSpec->name;
		thePB.dirInfo.ioVRefNum = theSpec->vRefNum;
		thePB.dirInfo.ioFDirIndex = 0;
		thePB.dirInfo.ioDrDirID = theSpec->parID;
		
		theErr = PBGetCatInfo(&thePB, false);
		
		HUnlock(dropSpec.dataHandle);
		AEDisposeDesc(&dropSpec);
		
		if (theErr != noErr)
			return(false);
		
		if (!(thePB.dirInfo.ioFlAttrib & (1 << 4)))
			return(false);
		
		FindFolder(theSpec->vRefNum, kTrashFolderType, kCreateFolder, 
				&trashVRefNum, &trashDirID);
		
		if (thePB.dirInfo.ioDrDirID == trashDirID)
			return(true);
	}
	
	return(false);
}


//--------------------------------------------------------------  DragTrackingFunc

#if 0
pascal OSErr DragTrackingFunc (DragTrackingMessage theMessage, WindowPtr theWindow, 
		void *theRefCon, DragReference theDrag)
{
	DragAttributes	attributes;
	OSErr			theErr;
	
	theErr = noErr;
	
	GetDragAttributes(theDrag, &attributes);
	
	switch (theMessage)
	{
		case dragTrackingEnterWindow:
		xxx;
		break;
		
		case dragTrackingInWindow:
		xxx;
		break;
		
		case dragTrackingLeaveWindow:
		xxx;
		break;
	}
	
	return (theErr);
}
#endif

//--------------------------------------------------------------  DragRoom

Boolean DragRoom (EventRecord *theEvent, Rect *roomSrc, short roomNumber)
{
	DragReference	theDrag;
	DragAttributes	attributes;
	AEDesc			dropLocation;
	Rect			largeBounds, smallBounds;
	PicHandle		smallPict;
	roomType		*theRoom;
	RgnHandle		boundsRgn, tempRgn;
//	Point			dragPoint;
	OSErr			theErr;
	short			mouseDnMods, mouseUpMods, copyRoom;
	char			wasState;
	
	if (thisMac.hasDrag)
	{
		if (!WaitMouseMoved(theEvent->where))
			return(false);
		
		SetPort((GrafPtr)mainWindow);
		BeginUpdate((GrafPtr)mainWindow);
		UpdateMainWindow();
		EndUpdate((GrafPtr)mainWindow);
		
		theErr = NewDrag(&theDrag);
		if (theErr != noErr)
			return (false);
		
		wasState = HGetState((Handle)thisHouse);
		HLock((Handle)thisHouse);
		theRoom = &((*thisHouse)->rooms[roomNumber]);
		
		theErr = AddDragItemFlavor(theDrag, (ItemReference)roomNumber, 
				(FlavorType)'Room', (Ptr)theRoom, 
				sizeof(roomType), (FlavorFlags)0);
		if (theErr != noErr)
		{
			HSetState((Handle)thisHouse, wasState);
			DisposeDrag(theDrag);
			return (false);
		}
		
		SetRect(&largeBounds, 0, 0, kRoomWide, kTileHigh);
		SetRect(&smallBounds, 0, 0, kRoomWide / 4, kTileHigh / 4);
		smallPict = OpenPicture(&smallBounds);
		CopyBits(&(((GrafPtr)mainWindow)->portBits), &(((GrafPtr)mainWindow)->portBits), 
				&largeBounds, &smallBounds, srcCopy, nil);
		ClosePicture();
		HLock((Handle)smallPict);
		theErr = AddDragItemFlavor(theDrag, (ItemReference)roomNumber, 
				(FlavorType)'PICT', (Ptr)(*smallPict), 
				GetHandleSize((Handle)smallPict), (FlavorFlags)0);
		HUnlock((Handle)smallPict);
		KillPicture(smallPict);
		
		HSetState((Handle)thisHouse, wasState);
		if (theErr != noErr)
		{
			DisposeDrag(theDrag);
			return (false);
		}
		
		theErr = SetDragItemBounds(theDrag, (ItemReference)roomNumber, roomSrc);
		if (theErr != noErr)
		{
			DisposeDrag(theDrag);
			return (false);
		}
		
		boundsRgn = NewRgn();
		RectRgn(boundsRgn, roomSrc);
		
		tempRgn = NewRgn();
		CopyRgn(boundsRgn, tempRgn);
		InsetRgn(tempRgn, 1, 1);
		DiffRgn(boundsRgn, tempRgn, boundsRgn);
		DisposeRgn(tempRgn);
		
		theErr = TrackDrag(theDrag, theEvent, boundsRgn);
		
		if ((theErr != noErr) && (theErr != userCanceledErr))
		{
			DisposeRgn(boundsRgn);
			DisposeDrag(theDrag);
			return(true);
		}
		
		theErr = GetDragAttributes(theDrag, &attributes);
		if (theErr != noErr)
		{
			DisposeRgn(boundsRgn);
			DisposeDrag(theDrag);
			return(true);
		}
		
		theErr = GetDropLocation(theDrag, &dropLocation);
		if (theErr != noErr)
		{
			DisposeRgn(boundsRgn);
			DisposeDrag(theDrag);
			return(true);
		}
		
		theErr = GetDragModifiers(theDrag, 0L, &mouseDnMods, &mouseUpMods);
		if (theErr != noErr)
		{
			DisposeRgn(boundsRgn);
			DisposeDrag(theDrag);
			return(true);
		}
		
		copyRoom = (mouseDnMods | mouseUpMods) & optionKey;
		
		if (!(attributes & kDragInsideSenderApplication))
		{
			if ((!copyRoom) && (DropLocationIsTrash(&dropLocation)))
			{
				DeselectObject();
				DeleteRoom(true);
			}
		}
		else if (attributes & kDragInsideSenderWindow)
		{
	//		SetPort(mapWindow);
	//		GetDragMouse(theDrag, &dragPoint, 0L);
	//		GlobalToLocal(&dragPoint);
	//		MoveRoom(dragPoint);
		}
		
		DisposeRgn(boundsRgn);
		DisposeDrag(theDrag);
	}
	
	return (true);
}

//--------------------------------------------------------------  InitDragInfo

#if 0
OSErr InitDragInfo (DragInfoHandle dragInfo)
{
	OSErr					theErr;
	DragTrackingHandlerUPP	trackingProc;
	DragReceiveHandlerUPP	receiveProc;
	
	if (!HasDragManager())
		return (noErr);
	
	trackingProc = NewDragTrackingHandlerProc(DragTrackingFunc);
	(**dragInfo).dragTrackingProc = trackingProc;
	theErr = InstallTrackingHandler(trackingProc, mapWindow, dragInfo);
	if (theErr != noErr)
		return (theErr);
	
	receiveProc = NewDragReceiveHandlerProc(DragReceiveFunc);
	(**dragInfo).dragReceiveProc = receiveProc;
	theErr = InstallReceiveHandler(receiveProc, (**dragInfo).window, dragInfo);
	
	return err;
}
#endif

//--------------------------------------------------------------  KillDragInfo

#if 0
void KillDragInfo (DragInfoHandle dragInfo)
{
	OSErr		theErr;
	
	if (!HasDragManager())
		return (noErr);
	
	theErr = RemoveTrackingHandler((**dragInfo).dragTrackingProc,
			(**dragInfo).window);
	theErr = RemoveReceiveHandler((**dragInfo).dragReceiveProc,
			(**dragInfo).window);
}
#endif

*/

