//============================================================================
//----------------------------------------------------------------------------
//									Room.c
//----------------------------------------------------------------------------
//============================================================================


#include <Resources.h>
#include <ToolUtils.h>
#include "Externs.h"
#include "House.h"
#include "MainWindow.h"
#include "RectUtils.h"


#define kDeleteRoomAlert		1005
#define kYesDoDeleteRoom		1


Boolean QueryDeleteRoom (void);
void SetToNearestNeighborRoom (short, short);


roomPtr		thisRoom;
Rect		backSrcRect;
GWorldPtr	backSrcMap;
short		numberRooms, thisRoomNumber, previousRoom;
short		leftThresh, rightThresh, lastBackground;
Boolean		autoRoomEdit, newRoomNow, noRoomAtAll;
Boolean		leftOpen, rightOpen, topOpen, bottomOpen;
Boolean		doBitchDialogs;

extern	short		tempTiles[];


//==============================================================  Functions
//--------------------------------------------------------------  SetInitialTiles

#ifndef COMPILEDEMO
void SetInitialTiles (short background, Boolean doRoom)
{
	short		i;
	
	if (background >= kUserBackground)
	{
		for (i = 0; i < kNumTiles; i++)
		{
			if (doRoom)
				thisRoom->tiles[i] = i;
			else
				tempTiles[i] = i;
		}
	}
	else
	{
		switch (background)
		{
			case kSimpleRoom:
			case kPaneledRoom:
			case kBasement:
			case kChildsRoom:
			case kAsianRoom:
			case kUnfinishedRoom:
			case kSwingersRoom:
			case kBathroom:
			case kLibrary:
			for (i = 0; i < kNumTiles; i++)
			{
				if (doRoom)
					thisRoom->tiles[i] = 1;
				else
					tempTiles[i] = 1;
			}
			if (doRoom)
			{
				thisRoom->tiles[0] = 0;
				thisRoom->tiles[kNumTiles - 1] = kNumTiles - 1;
			}
			else
			{
				tempTiles[0] = 0;
				tempTiles[kNumTiles - 1] = kNumTiles - 1;
			}
			break;
			
			case kSkywalk:
			for (i = 0; i < kNumTiles; i++)
			{
				if (doRoom)
					thisRoom->tiles[i] = i;
				else
					tempTiles[i] = i;
			}
			break;
			
			case kField:
			case kGarden:
			case kDirt:
			for (i = 0; i < kNumTiles; i++)
			{
				if (doRoom)
					thisRoom->tiles[i] = 0;
				else
					tempTiles[i] = 0;
			}
			break;
			
			case kMeadow:
			for (i = 0; i < kNumTiles; i++)
			{
				if (doRoom)
					thisRoom->tiles[i] = 1;
				else
					tempTiles[i] = 1;
			}
			break;
			
			case kRoof:
			for (i = 0; i < kNumTiles; i++)
			{
				if (doRoom)
					thisRoom->tiles[i] = 3;
				else
					tempTiles[i] = 3;
			}
			break;
			
			case kSky:
			for (i = 0; i < kNumTiles; i++)
			{
				if (doRoom)
					thisRoom->tiles[i] = 2;
				else
					tempTiles[i] = 2;
			}
			break;
			
			case kStratosphere:
			case kStars:
			for (i = 0; i < kNumTiles; i++)
			{
				if (doRoom)
					thisRoom->tiles[i] = i;
				else
					tempTiles[i] = i;
			}
			break;
			
			default:
			break;
		}
	}
}
#endif

//--------------------------------------------------------------  CreateNewRoom

#ifndef COMPILEDEMO
Boolean CreateNewRoom (short h, short v)
{
	KeyMap		theKeys;
	long		howMuch;
	OSErr		theErr;
	short		i, availableRoom;
	char		wasState;
	
	CopyThisRoomToRoom();					// save off current room
	
	PasStringCopy("\pUntitled Room", thisRoom->name);
	thisRoom->leftStart = 32;				// fill out fields of new room
	thisRoom->rightStart = 32;
	thisRoom->bounds = 0;
	thisRoom->unusedByte = 0;
	thisRoom->visited = false;
	thisRoom->background = lastBackground;
	SetInitialTiles(thisRoom->background, true);
	thisRoom->floor = v;
	thisRoom->suite = h;
	thisRoom->openings = 0;
	thisRoom->numObjects = 0;
	for (i = 0; i < kMaxRoomObs; i++)		// zero out all objects
		thisRoom->objects[i].what = kObjectIsEmpty;
	
	wasState = HGetState((Handle)thisHouse);
	MoveHHi((Handle)thisHouse);
	HLock((Handle)thisHouse);
	availableRoom = -1;						// assume no available rooms
	if ((*thisHouse)->nRooms > 0)			// look for an empty room
		for (i = 0; i < (*thisHouse)->nRooms; i++)
			if ((*thisHouse)->rooms[i].suite == kRoomIsEmpty)
			{
				availableRoom = i;
				break;
			}
	
	if (availableRoom == -1)				// found no available rooms
	{
		HUnlock((Handle)thisHouse);
		howMuch = sizeof(roomType);			// add new room to end of house
		theErr = PtrAndHand((Ptr)thisRoom, (Handle)thisHouse, howMuch);
		if (theErr != noErr)
		{
			YellowAlert(kYellowUnaccounted, theErr);
			MoveHHi((Handle)thisHouse);
			HLock((Handle)thisHouse);
			return (false);
		}
		MoveHHi((Handle)thisHouse);
		HLock((Handle)thisHouse);
		(*thisHouse)->nRooms++;				// increment nRooms
		numberRooms = (*thisHouse)->nRooms;
		previousRoom = thisRoomNumber;
		thisRoomNumber = numberRooms - 1;
	}
	else
	{
		previousRoom = thisRoomNumber;
		thisRoomNumber = availableRoom;
	}
	
	if (noRoomAtAll)
		(*thisHouse)->firstRoom = thisRoomNumber;
	
	HSetState((Handle)thisHouse, wasState);
	
	CopyThisRoomToRoom();
	UpdateEditWindowTitle();
	noRoomAtAll = false;
	fileDirty = true;
	UpdateMenus(false);
	
	GetKeys(theKeys);
	if (BitTst(&theKeys, kShiftKeyMap))
		newRoomNow = false;
	else
		newRoomNow = autoRoomEdit;			// Flag to bring up RoomInfo
	
	return (true);
}
#endif

//--------------------------------------------------------------  ReadyBackground

void ReadyBackground (short theID, short *theTiles)
{
	Rect		src, dest;
	PicHandle	thePicture;
	short		i;
	
	SetPort((GrafPtr)workSrcMap);
	
	if ((noRoomAtAll) || (!houseUnlocked))
	{
		LtGrayForeColor();
		PaintRect(&workSrcRect);
		ForeColor(blackColor);
		MoveTo(10, 20);
		if (houseUnlocked)
			DrawString("\pNo rooms");
		else
			DrawString("\pNothing to show");
		
		CopyBits((BitMap *)*GetGWorldPixMap(workSrcMap), 
				(BitMap *)*GetGWorldPixMap(backSrcMap), 
				&workSrcRect, &workSrcRect, srcCopy, nil);
		return;
	}
	
	thePicture = GetPicture(theID);
	if (thePicture == nil)
	{
		thePicture = (PicHandle)GetResource('Date', theID);
		if (thePicture == nil)
		{
			YellowAlert(kYellowNoBackground, 0);
			return;
		}
	}
	
	HLock((Handle)thePicture);
	dest = (*thePicture)->picFrame;
	HUnlock((Handle)thePicture);
	QOffsetRect(&dest, -dest.left, -dest.top);
	DrawPicture(thePicture, &dest);
	ReleaseResource((Handle)thePicture);
	
	QSetRect(&src, 0, 0, kTileWide, kTileHigh);
	QSetRect(&dest, 0, 0, kTileWide, kTileHigh);
	for (i = 0; i < kNumTiles; i++)
	{
		src.left = theTiles[i] * kTileWide;
		src.right = src.left + kTileWide;
		CopyBits((BitMap *)*GetGWorldPixMap(workSrcMap), 
				(BitMap *)*GetGWorldPixMap(backSrcMap), 
				&src, &dest, srcCopy, nil);
		QOffsetRect(&dest, kTileWide, 0);
	}
	
	QSetRect(&src, 0, 0, kRoomWide, kTileHigh);
	QSetRect(&dest, 0, 0, kRoomWide, kTileHigh);
	CopyBits((BitMap *)*GetGWorldPixMap(backSrcMap), 
			(BitMap *)*GetGWorldPixMap(workSrcMap), 
			&src, &dest, srcCopy, nil);
}

//--------------------------------------------------------------  ReflectCurrentRoom

void ReflectCurrentRoom (Boolean forceMapRedraw)
{
#ifndef COMPILEDEMO
	if (theMode != kEditMode)
		return;
	
	if ((noRoomAtAll) || (!houseUnlocked))
	{
		CenterMapOnRoom(64, 1);
		UpdateMapWindow();
	}
	else
	{
		if ((!ThisRoomVisibleOnMap()) || (forceMapRedraw))
		{
			CenterMapOnRoom(thisRoom->suite, thisRoom->floor);
			UpdateMapWindow();			// whole map window redrawm
		}
		else
		{
			FindNewActiveRoomRect();	// find newly selected room rect
			FlagMapRoomsForUpdate();	// redraw only the portions required
		}
	}
	GenerateRetroLinks();
	UpdateEditWindowTitle();
	ReadyBackground(thisRoom->background, thisRoom->tiles);
	GetThisRoomsObjRects();
	DrawThisRoomsObjects();
	InvalWindowRect(mainWindow, &mainWindowRect);
#endif
}

//--------------------------------------------------------------  CopyRoomToThisRoom

void CopyRoomToThisRoom (short roomNumber)
{
	if (roomNumber == -1)
		return;
	
	CopyThisRoomToRoom();			// copy back to house
	ForceThisRoom(roomNumber);		// load new room from house
}

//--------------------------------------------------------------  CopyThisRoomToRoom

void CopyThisRoomToRoom (void)
{
	char		tagByte;
	
	if ((noRoomAtAll) || (thisRoomNumber == -1))
		return;
	
	tagByte = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);		// copy back to house
	(*thisHouse)->rooms[thisRoomNumber] = *thisRoom;
	HSetState((Handle)thisHouse, tagByte);
}

//--------------------------------------------------------------  ForceThisRoom

void ForceThisRoom (short roomNumber)
{
	char		tagByte;
	
	if (roomNumber == -1)
		return;
	
	tagByte = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	if (roomNumber < (*thisHouse)->nRooms)
		*thisRoom = (*thisHouse)->rooms[roomNumber];
	else
		YellowAlert(kYellowIllegalRoomNum, 0);
	HSetState((Handle)thisHouse, tagByte);
	
	previousRoom = thisRoomNumber;
	thisRoomNumber = roomNumber;
}

//--------------------------------------------------------------  RoomExists

Boolean RoomExists (short suite, short floor, short *roomNum)
{
	// pass in a suite and floor; returns true is it is a legitimate room
	houseType	*thisHousePtr;
	short		i;
	char		wasState;
	Boolean		foundIt;
	
	foundIt = false;
	
	if (suite < 0)
		return (foundIt);
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	thisHousePtr = *thisHouse;
	
	for (i = 0; i < numberRooms; i++)
	{
		if ((thisHousePtr->rooms[i].floor == floor) && 
				(thisHousePtr->rooms[i].suite == suite))
		{
			foundIt = true;
			*roomNum = i;
			break;
		}
	}
	
	HSetState((Handle)thisHouse, wasState);
	
	return (foundIt);
}

//--------------------------------------------------------------  RoomNumExists

Boolean RoomNumExists (short roomNum)
{
	short		floor, suite, whoCares;
	Boolean		exists;
	
	exists = false;
	if (GetRoomFloorSuite(roomNum, &floor, &suite))
		exists = RoomExists(suite, floor, &whoCares);
	
	return (exists);
}

//--------------------------------------------------------------  DeleteRoom

void DeleteRoom (Boolean doWarn)
{
#ifndef COMPILEDEMO
	short		wasFloor, wasSuite;
	char		wasState;
	Boolean		firstDeleted;
	
	if ((theMode != kEditMode) || (noRoomAtAll))
		return;
	
	if (doWarn)
	{
		if (!QueryDeleteRoom())
			return;
	}
	
	DeselectObject();
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	wasFloor = (*thisHouse)->rooms[thisRoomNumber].floor;
	wasSuite = (*thisHouse)->rooms[thisRoomNumber].suite;
	firstDeleted = ((*thisHouse)->firstRoom == thisRoomNumber);	// is room "first"
	thisRoom->suite = kRoomIsEmpty;
	(*thisHouse)->rooms[thisRoomNumber].suite = kRoomIsEmpty;
	HSetState((Handle)thisHouse, wasState);
	
	noRoomAtAll = (RealRoomNumberCount() == 0);					// see if now no rooms
	if (noRoomAtAll)
		thisRoomNumber = kRoomIsEmpty;
	else
		SetToNearestNeighborRoom(wasFloor, wasSuite);
	
	if (firstDeleted)
	{
		wasState = HGetState((Handle)thisHouse);
		HLock((Handle)thisHouse);
		(*thisHouse)->firstRoom = thisRoomNumber;
		HSetState((Handle)thisHouse, wasState);
	}
	
	newRoomNow = false;
	fileDirty = true;
	UpdateMenus(false);
	ReflectCurrentRoom(false);
#endif
}

//--------------------------------------------------------------  QueryDeleteRoom

#ifndef COMPILEDEMO
Boolean QueryDeleteRoom (void)
{
	short		hitWhat;
	
//	CenterAlert(kDeleteRoomAlert);
	hitWhat = Alert(kDeleteRoomAlert, nil);
	if (hitWhat == kYesDoDeleteRoom)
		return (true);
	else
		return (false);
}
#endif

//--------------------------------------------------------------  DoesNeighborRoomExist

short DoesNeighborRoomExist (short whichNeighbor)
{
#ifndef COMPILEDEMO
	short		newH, newV, newRoomNumber;
	
	if (theMode != kEditMode)
		return(-1);
	
	newH = thisRoom->suite;
	newV = thisRoom->floor;
	
	switch (whichNeighbor)
	{
		case kRoomAbove:
		newV++;
		break;
		
		case kRoomBelow:
		newV--;
		break;
		
		case kRoomToRight:
		newH++;
		break;
		
		case kRoomToLeft:
		newH--;
		break;
	}
	
	if (RoomExists(newH, newV, &newRoomNumber))
		return (newRoomNumber);
	else
		return (-1);
#endif
}

//--------------------------------------------------------------  SelectNeighborRoom

void SelectNeighborRoom (short whichNeighbor)
{
#ifndef COMPILEDEMO
	short		newRoomNumber;
	
	newRoomNumber = DoesNeighborRoomExist(whichNeighbor);
	
	if (newRoomNumber != -1)
	{
		DeselectObject();
		CopyRoomToThisRoom(newRoomNumber);
		ReflectCurrentRoom(false);
	}
#endif
}

//--------------------------------------------------------------  GetNeighborRoomNumber

short GetNeighborRoomNumber (short which)
{
	short		hDelta, vDelta, i;
	short		roomH, roomV;
	short		roomNum;
	char		wasState;
	
	switch (which)
	{
		case kCentralRoom:
		hDelta = 0;
		vDelta = 0;
		break;
		
		case kNorthRoom:
		hDelta = 0;
		vDelta = 1;
		break;
		
		case kNorthEastRoom:
		hDelta = 1;
		vDelta = 1;
		break;
		
		case kEastRoom:
		hDelta = 1;
		vDelta = 0;
		break;
		
		case kSouthEastRoom:
		hDelta = 1;
		vDelta = -1;
		break;
		
		case kSouthRoom:
		hDelta = 0;
		vDelta = -1;
		break;
		
		case kSouthWestRoom:
		hDelta = -1;
		vDelta = -1;
		break;
		
		case kWestRoom:
		hDelta = -1;
		vDelta = 0;
		break;
		
		case kNorthWestRoom:
		hDelta = -1;
		vDelta = 1;
		break;
	}
	
	roomNum = kRoomIsEmpty;
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	roomH = (*thisHouse)->rooms[thisRoomNumber].suite + hDelta;
	roomV = (*thisHouse)->rooms[thisRoomNumber].floor + vDelta;
	
	for (i = 0; i < numberRooms; i++)
	{
		if (((*thisHouse)->rooms[i].suite == roomH) && 
				((*thisHouse)->rooms[i].floor == roomV))
		{
			roomNum = i;
			break;
		}
	}
	HSetState((Handle)thisHouse, wasState);
	
	return (roomNum);
}

//--------------------------------------------------------------  SetToNearestNeighborRoom

void SetToNearestNeighborRoom (short wasFloor, short wasSuite)
{
	// searches in a clockwise spiral pattern (from thisRoom) for aÉ
	// legitimate neighboring room - then sets thisRoom to it
	short		distance, h, v;
	short		hStep, vStep;
	short		testRoomNum, testH, testV;
	char		wasState;
	Boolean		finished;
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	
	finished = false;
	distance = 1;	// we begin our walk a distance of one from source room
	h = -1;			// we begin with the neighbor to the leftÉ
	v = 0;			// and on the same floor
	hStep = 0;		// we don't 'walk' left or rightÉ
	vStep = -1;		// instead, we 'walk' up
	
	do
	{
		testH = wasSuite + h;
		testV = wasFloor + v;
		
		if (RoomExists(testH, testV, &testRoomNum))		// if a legitimate room
		{
			CopyRoomToThisRoom(testRoomNum);
			finished = true;
		}
		else
		{
			h += hStep;
			v += vStep;
			if ((h > distance) || (h < -distance) || (v > distance) || (v < -distance))
			{			// we have walked beyond the bounds of our spiral
				if ((hStep == -1) && (vStep == 0))	// we expand our spiral out
				{
					distance++;
					hStep = 0;						// begin travelling up again
					vStep = -1;
				}
				else
				{
					h -= hStep;						// first, back up a step
					v -= vStep;
					
					if (hStep == 0)					// we were travelling up or down
					{
						if (vStep == -1)			// we were travelling upÉ
							hStep = 1;				// so begin travelling right
						else						// we were travelling downÉ
							hStep = -1;				// so begin travelling left
						vStep = 0;
					}
					else
					{
						hStep = 0;					// begin travelling down
						vStep = 1;
					}
					h += hStep;						// proceed a step now
					v += vStep;
				}
			}
		}
	} while (!finished);
	
	HSetState((Handle)thisHouse, wasState);
}

//--------------------------------------------------------------  GetRoomFloorSuite

Boolean GetRoomFloorSuite (short room, short *floor, short *suite)
{
	char		wasState;
	Boolean		isRoom;
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	if ((*thisHouse)->rooms[room].suite == kRoomIsEmpty)
	{
		*floor = 0;
		*suite = kRoomIsEmpty;
		isRoom = false;
	}
	else
	{
		*suite = (*thisHouse)->rooms[room].suite;
		*floor = (*thisHouse)->rooms[room].floor;
		isRoom = true;
	}
	HSetState((Handle)thisHouse, wasState);
	
	return (isRoom);
}

//--------------------------------------------------------------  GetRoomNumber

short GetRoomNumber (short floor, short suite)
{
	// pass in a floor and suite; returns the room index into the house file
	short		roomNum, i;
	char		wasState;
	
	roomNum = kRoomIsEmpty;
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	for (i = 0; i < numberRooms; i++)
	{
		if (((*thisHouse)->rooms[i].suite == suite) && 
				((*thisHouse)->rooms[i].floor == floor))
		{
			roomNum = i;
			break;
		}
	}
	HSetState((Handle)thisHouse, wasState);
	
	return (roomNum);
}

//--------------------------------------------------------------  IsRoomAStructure

Boolean	IsRoomAStructure (short roomNum)
{
	char		wasState;
	Boolean		isStructure;
	
	if (roomNum == kRoomIsEmpty)
		return (false);
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	if ((*thisHouse)->rooms[roomNum].background >= kUserBackground)
	{
		if ((*thisHouse)->rooms[roomNum].bounds != 0)
		{
			isStructure = (((*thisHouse)->rooms[roomNum].bounds & 32) == 32);
		}
		else
		{
			if ((*thisHouse)->rooms[roomNum].background < kUserStructureRange)
				isStructure = true;
			else
				isStructure = false;
		}
	}
	else
	{
		switch ((*thisHouse)->rooms[roomNum].background)
		{
			case kPaneledRoom:
			case kSimpleRoom:
			case kChildsRoom:
			case kAsianRoom:
			case kUnfinishedRoom:
			case kSwingersRoom:
			case kBathroom:
			case kLibrary:
			case kSkywalk:
			case kRoof:
			isStructure = true;
			break;
			
			default:
			isStructure = false;
			break;
		}
	}
	HSetState((Handle)thisHouse, wasState);
	
	return (isStructure);
}

//--------------------------------------------------------------  DetermineRoomOpenings

void DetermineRoomOpenings (void)
{
	short		whichBack, leftTile, rightTile;
	short		boundsCode;
	
	whichBack = thisRoom->background;
	leftTile = thisRoom->tiles[0];
	rightTile = thisRoom->tiles[kNumTiles - 1];
	
	if (whichBack >= kUserBackground)
	{
		if (thisRoom->bounds != 0)
			boundsCode = thisRoom->bounds >> 1;
		else
			boundsCode = GetOriginalBounding(whichBack);
		leftOpen = ((boundsCode & 0x0001) == 0x0001);
		rightOpen = ((boundsCode & 0x0004) == 0x0004);
		
		if (leftOpen)
			leftThresh = kNoLeftWallLimit;
		else
			leftThresh = kLeftWallLimit;
		
		if (rightOpen)
			rightThresh = kNoRightWallLimit;
		else
			rightThresh = kRightWallLimit;
	}
	else
	{
		switch (whichBack)
		{
			case kSimpleRoom:
			case kPaneledRoom:
			case kBasement:
			case kChildsRoom:
			case kAsianRoom:
			case kUnfinishedRoom:
			case kSwingersRoom:
			case kBathroom:
			case kLibrary:
			case kSky:
			if (leftTile == 0)
				leftThresh = kLeftWallLimit;
			else
				leftThresh = kNoLeftWallLimit;
			if (rightTile == (kNumTiles - 1))
				rightThresh = kRightWallLimit;
			else
				rightThresh = kNoRightWallLimit;
			leftOpen = (leftTile != 0);
			rightOpen = (rightTile != (kNumTiles - 1));
			break;
			
			case kDirt:
			if (leftTile == 1)
				leftThresh = kLeftWallLimit;
			else
				leftThresh = kNoLeftWallLimit;
			if (rightTile == (kNumTiles - 1))
				rightThresh = kRightWallLimit;
			else
				rightThresh = kNoRightWallLimit;
			leftOpen = (leftTile != 0);
			rightOpen = (rightTile != (kNumTiles - 1));
			break;
			
			case kMeadow:
			if (leftTile == 6)
				leftThresh = kLeftWallLimit;
			else
				leftThresh = kNoLeftWallLimit;
			if (rightTile == 7)
				rightThresh = kRightWallLimit;
			else
				rightThresh = kNoRightWallLimit;
			leftOpen = (leftTile != 6);
			rightOpen = (rightTile != 7);
			break;
			
			case kGarden:
			case kSkywalk:
			case kField:
			case kStratosphere:
			case kStars:
			leftThresh = kNoLeftWallLimit;
			rightThresh = kNoRightWallLimit;
			leftOpen = true;
			rightOpen = true;
			break;
			
			default:
			if (leftTile == 0)
				leftThresh = kLeftWallLimit;
			else
				leftThresh = kNoLeftWallLimit;
			
			if (rightTile == (kNumTiles - 1))
				rightThresh = kRightWallLimit;
			else
				rightThresh = kNoRightWallLimit;
			
			leftOpen = (leftTile != 0);
			rightOpen = (rightTile != (kNumTiles - 1));
			break;
		}
	}
	
	if (DoesRoomHaveFloor())
		bottomOpen = false;
	else
		bottomOpen = true;
	
	if (DoesRoomHaveCeiling())
		topOpen = false;
	else
		topOpen = true;
}

//--------------------------------------------------------------  GetOriginalBounding

short GetOriginalBounding (short theID)
{
	boundsHand	boundsRes;
	short		boundCode;
	
	boundsRes = (boundsHand)GetResource('bnds', theID);
	if (boundsRes == nil)
	{
		if (PictIDExists(theID))
			YellowAlert(kYellowNoBoundsRes, 0);
		boundCode = 0;
	}
	else
	{
		boundCode = 0;
		HLock((Handle)boundsRes);
		if ((*boundsRes)->left)
			boundCode += 1;
		if ((*boundsRes)->top)
			boundCode += 2;
		if ((*boundsRes)->right)
			boundCode += 4;
		if ((*boundsRes)->bottom)
			boundCode += 8;
		HUnlock((Handle)boundsRes);
		ReleaseResource((Handle)boundsRes);
	}
	
	return (boundCode);
}

//--------------------------------------------------------------  GetNumberOfLights

short GetNumberOfLights (short where)
{
	houseType	*thisHousePtr;
	short		i, count;
	char		wasState;
	
	if (theMode == kEditMode)
	{
		switch (thisRoom->background)
		{
			case kGarden:
			case kSkywalk:
			case kMeadow:
			case kField:
			case kRoof:
			case kSky:
			case kStratosphere:
			case kStars:
			count = 1;
			break;
			
			case kDirt:
			count = 0;
			if ((thisRoom->tiles[0] == 0) && (thisRoom->tiles[1] == 0) && 
					(thisRoom->tiles[2] == 0) && (thisRoom->tiles[3] == 0) && 
					(thisRoom->tiles[4] == 0) && (thisRoom->tiles[5] == 0) &&
					(thisRoom->tiles[6] == 0) && (thisRoom->tiles[7] == 0))
				count = 1;
			break;
			
			default:
			count = 0;
			break;
		}
		if (count == 0)
		{
			for (i = 0; i < kMaxRoomObs; i++)
			{
				switch (thisRoom->objects[i].what)
				{
					case kDoorInLf:
					case kDoorInRt:
					case kWindowInLf:
					case kWindowInRt:
					case kWallWindow:
					count++;
					break;
					
					case kCeilingLight:
					case kLightBulb:
					case kTableLamp:
					case kHipLamp:
					case kDecoLamp:
					case kFlourescent:
					case kTrackLight:
					case kInvisLight:
					if (thisRoom->objects[i].data.f.initial)
						count++;
					break;
				}
			}
		}
	}
	else
	{
		wasState = HGetState((Handle)thisHouse);
		HLock((Handle)thisHouse);
		thisHousePtr = *thisHouse;
		switch (thisHousePtr->rooms[where].background)
		{
			case kGarden:
			case kSkywalk:
			case kMeadow:
			case kField:
			case kRoof:
			case kSky:
			case kStratosphere:
			case kStars:
			count = 1;
			break;
			
			case kDirt:
			count = 0;
			if ((thisHousePtr->rooms[where].tiles[0] == 0) && 
					(thisHousePtr->rooms[where].tiles[1] == 0) && 
					(thisHousePtr->rooms[where].tiles[2] == 0) && 
					(thisHousePtr->rooms[where].tiles[3] == 0) && 
					(thisHousePtr->rooms[where].tiles[4] == 0) && 
					(thisHousePtr->rooms[where].tiles[5] == 0) &&
					(thisHousePtr->rooms[where].tiles[6] == 0) && 
					(thisHousePtr->rooms[where].tiles[7] == 0))
				count = 1;
			break;
			
			default:
			count = 0;
			break;
		}
		if (count == 0)
		{
			for (i = 0; i < kMaxRoomObs; i++)
			{
				switch (thisHousePtr->rooms[where].objects[i].what)
				{
					case kDoorInLf:
					case kDoorInRt:
					case kWindowInLf:
					case kWindowInRt:
					case kWallWindow:
					count++;
					break;
					
					case kCeilingLight:
					case kLightBulb:
					case kTableLamp:
					case kHipLamp:
					case kDecoLamp:
					case kFlourescent:
					case kTrackLight:
					case kInvisLight:
					if (thisHousePtr->rooms[where].objects[i].data.f.state)
						count++;
					break;
				}
			}
		}
		HSetState((Handle)thisHouse, wasState);
	}
	return (count);
}

//--------------------------------------------------------------  IsShadowVisible

Boolean IsShadowVisible (void)
{
	short		boundsCode;
	Boolean		hasFloor;
	
	if (thisRoom->background >= kUserBackground)
	{
		if (thisRoom->bounds != 0)			// is this a version 2.0 house?
			boundsCode = (thisRoom->bounds >> 1);
		else
			boundsCode = GetOriginalBounding(thisRoom->background);
		hasFloor = ((boundsCode & 0x0008) != 0x0008);
	}
	else
	{
		switch (thisRoom->background)
		{
			case kRoof:
			case kSky:
			case kStratosphere:
			case kStars:
			hasFloor = false;
			break;
			
			default:
			hasFloor = true;
			break;
		}
	}
	
	return (hasFloor);
}

//--------------------------------------------------------------  DoesRoomHaveFloor

Boolean DoesRoomHaveFloor (void)
{
	short		boundsCode;
	Boolean		hasFloor;
	
	if (thisRoom->background >= kUserBackground)
	{
		if (thisRoom->bounds != 0)			// is this a version 2.0 house?
			boundsCode = (thisRoom->bounds >> 1);
		else
			boundsCode = GetOriginalBounding(thisRoom->background);
		hasFloor = ((boundsCode & 0x0008) != 0x0008);
	}
	else
	{
		switch (thisRoom->background)
		{
			case kSky:
			case kStratosphere:
			case kStars:
			hasFloor = false;
			break;
			
			default:
			hasFloor = true;
			break;
		}
	}
	
	return (hasFloor);
}

//--------------------------------------------------------------  DoesRoomHaveCeiling

Boolean DoesRoomHaveCeiling (void)
{
	short		boundsCode;
	Boolean		hasCeiling;
	
	if (thisRoom->background >= kUserBackground)
	{
		if (thisRoom->bounds != 0)			// is this a version 2.0 house?
			boundsCode = (thisRoom->bounds >> 1);
		else
			boundsCode = GetOriginalBounding(thisRoom->background);
		hasCeiling = ((boundsCode & 0x0002) != 0x0002);
	}
	else
	{
		switch (thisRoom->background)
		{
			case kGarden:
			case kMeadow:
			case kField:
			case kRoof:
			case kSky:
			case kStratosphere:
			case kStars:
			hasCeiling = false;
			break;
			
			default:
			hasCeiling = true;
			break;
		}
	}
	return (hasCeiling);
}

