
//============================================================================
//----------------------------------------------------------------------------
//								   HouseLegal.c
//----------------------------------------------------------------------------
//============================================================================


#include <NumberFormatting.h>
#include <StringCompare.h>
#include "Externs.h"
#include "ObjectEdit.h"
#include "RectUtils.h"


void WrapBannerAndTrailer (void);
void ValidateNumberOfRooms (void);
void CheckDuplicateFloorSuite (void);
void CompressHouse (void);
void LopOffExtraRooms (void);
void ValidateRoomNumbers (void);
void CountUntitledRooms (void);
void CheckRoomNameLength (void);
void MakeSureNumObjectsJives (void);
void KeepAllObjectsLegal (void);
void CheckForStaircasePairs (void);


short		houseErrors, wasRoom;
Boolean		isHouseChecks;

extern	short		numberRooms;


//==============================================================  Functions
//--------------------------------------------------------------  KeepObjectLegal

// Does a test of the current object active for any illegal boundsÉ
// or values.  It corrects the erros and returns true if any changesÉ
// were made.

Boolean KeepObjectLegal (void)
{
	objectType	*theObject;
	Rect		bounds, roomRect;
	short		direction, dist;
	char		wasState;
	Boolean		unchanged;
	
	unchanged = true;
#ifndef COMPILEDEMO
	
	theObject = &thisRoom->objects[objActive];
	
	if (objActive == kInitialGliderSelected)
	{
		wasState = HGetState((Handle)thisHouse);
		HLock((Handle)thisHouse);
		if ((*thisHouse)->initial.h < 0)
			(*thisHouse)->initial.h = 0;
		if ((*thisHouse)->initial.v < 0)
			(*thisHouse)->initial.v = 0;
		if ((*thisHouse)->initial.h > (kRoomWide - kGliderWide))
			(*thisHouse)->initial.h = kRoomWide - kGliderWide;
		if ((*thisHouse)->initial.v > (kTileHigh - kGliderHigh))
			(*thisHouse)->initial.v = kTileHigh - kGliderHigh;
		HSetState((Handle)thisHouse, wasState);
		return (true);
	}
	
	QSetRect(&roomRect, 0, 0, kRoomWide, kTileHigh);
	
	switch (theObject->what)
	{
		case kFloorVent:
		case kCeilingVent:
		case kFloorBlower:
		case kCeilingBlower:
		case kSewerGrate:
		case kLeftFan:
		case kRightFan:
		case kTaper:
		case kCandle:
		case kStubby:
		case kTiki:
		case kBBQ:
		case kInvisBlower:
		case kGrecoVent:
		case kSewerBlower:
		case kLiftArea:
		GetObjectRect(&thisRoom->objects[objActive], &bounds);
		if (ForceRectInRect(&bounds, &roomRect))
		{
			theObject->data.a.topLeft.h = bounds.left;
			theObject->data.a.topLeft.v = bounds.top;
			unchanged = false;
			if (theObject->what == kLiftArea)
			{
				theObject->data.a.distance = RectWide(&bounds);
				theObject->data.a.tall = RectTall(&bounds) / 2;
			}
		}
		if ((theObject->what == kStubby) && (theObject->data.a.topLeft.h % 2 == 0))
		{
			theObject->data.a.topLeft.h--;
			unchanged = false;
		}
		if (((theObject->what == kTaper) || (theObject->what == kCandle) || 
				(theObject->what == kTiki) || (theObject->what == kBBQ)) && 
				(theObject->data.a.topLeft.h % 2 != 0))
		{
			theObject->data.a.topLeft.h--;
			unchanged = false;
		}
		if ((theObject->what == kFloorVent) && (theObject->data.a.topLeft.v != kFloorVentTop))
		{
			theObject->data.a.topLeft.v = kFloorVentTop;
			theObject->data.a.distance += 2;
		}
		if ((theObject->what == kFloorBlower) && 
				(theObject->data.a.topLeft.v != kFloorBlowerTop))
		{
			theObject->data.a.topLeft.v = kFloorBlowerTop;
			theObject->data.a.distance += 2;
		}
		if ((theObject->what == kSewerGrate) && 
				(theObject->data.a.topLeft.v != kSewerGrateTop))
		{
			theObject->data.a.topLeft.v = kSewerGrateTop;
			theObject->data.a.distance += 2;
		}
		if ((theObject->what == kFloorTrans) && 
				(theObject->data.a.topLeft.v != kFloorTransTop))
		{
			theObject->data.a.topLeft.v = kFloorTransTop;
			theObject->data.a.distance += 2;
		}
		if (ObjectHasHandle(&direction, &dist))
		{
			switch (direction)
			{
				case kAbove:
				dist = bounds.top - dist;
				if ((theObject->what == kFloorVent) || 
						(theObject->what == kFloorBlower) ||
						(theObject->what == kTaper) ||
						(theObject->what == kCandle) ||
						(theObject->what == kStubby))
				{
					if (dist < 36)
					{
						theObject->data.a.distance += dist - 36;
						unchanged = false;
					}
				}
				else
				{
					if (dist < 0)
					{
						theObject->data.a.distance += dist;
						unchanged = false;
					}
				}
				break;
				
				case kToRight:
				dist = bounds.right + dist;
				if (dist > kRoomWide)
				{
					theObject->data.a.distance += (kRoomWide - dist);
					unchanged = false;
				}
				break;
				
				case kBelow:
				dist = bounds.bottom + dist;
				if (dist > kTileHigh)
				{
					theObject->data.a.distance += (kTileHigh - dist);
					unchanged = false;
				}
				break;
				
				case kToLeft:
				dist = bounds.left - dist;
				if (dist < 0)
				{
					theObject->data.a.distance += dist;
					unchanged = false;
				}
				break;
			}
		}
		break;
		
		case kTable:
		case kShelf:
		case kCabinet:
		case kFilingCabinet:
		case kWasteBasket:
		case kMilkCrate:
		case kCounter:
		case kDresser:
		case kDeckTable:
		case kStool:
		case kTrunk:
		case kInvisObstacle:
		case kManhole:
		case kBooks:
		case kInvisBounce:
		GetObjectRect(&thisRoom->objects[objActive], &bounds);
		if (ForceRectInRect(&bounds, &roomRect))
		{
			theObject->data.b.bounds = bounds;
			unchanged = false;
		}
		if ((theObject->what == kManhole) && 
				(((bounds.left - 3) % 64) != 0))
		{
			theObject->data.b.bounds.left = 
					(((bounds.left + 29) / 64) * 64) + 3;
			theObject->data.b.bounds.right = 
					theObject->data.b.bounds.left + 
					RectWide(&srcRects[kManhole]);
			unchanged = false;
		}
		break;
		
		case kRedClock:
		case kBlueClock:
		case kYellowClock:
		case kCuckoo:
		case kPaper:
		case kBattery:
		case kBands:
		case kGreaseRt:
		case kGreaseLf:
		case kFoil:
		case kInvisBonus:
		case kStar:
		case kSparkle:
		case kHelium:
		case kSlider:
		GetObjectRect(&thisRoom->objects[objActive], &bounds);
		if (ForceRectInRect(&bounds, &roomRect))
		{
			theObject->data.c.topLeft.h = bounds.left;
			theObject->data.c.topLeft.v = bounds.top;
			unchanged = false;
		}
		if ((theObject->what == kGreaseRt) && 
				(bounds.right + theObject->data.c.length > kRoomWide))
		{
			theObject->data.c.length = kRoomWide - bounds.right;
			unchanged = false;
		}
		else if ((theObject->what == kGreaseLf) && 
				(bounds.left - theObject->data.c.length < 0))
		{
			theObject->data.c.length = bounds.left;
			unchanged = false;
		}
		else if ((theObject->what == kSlider) && 
				(bounds.left + theObject->data.c.length > kRoomWide))
		{
			theObject->data.c.length = kRoomWide - bounds.left;
			unchanged = false;
		}
		if (theObject->data.c.topLeft.h % 2 != 0)
		{
			theObject->data.c.topLeft.h--;
			unchanged = false;
		}
		if ((theObject->what != kStar) && 
				(theObject->data.c.length % 2 != 0))
		{
			theObject->data.c.length--;
			unchanged = false;
		}
		break;
		
		case kUpStairs:
		case kDownStairs:
		case kMailboxLf:
		case kMailboxRt:
		case kFloorTrans:
		case kCeilingTrans:
		case kDoorInLf:
		case kDoorInRt:
		case kDoorExRt:
		case kDoorExLf:
		case kWindowInLf:
		case kWindowInRt:
		case kWindowExRt:
		case kWindowExLf:
		case kInvisTrans:
		case kDeluxeTrans:
		GetObjectRect(&thisRoom->objects[objActive], &bounds);
		if (ForceRectInRect(&bounds, &roomRect))
		{
			theObject->data.d.topLeft.h = bounds.left;
			theObject->data.d.topLeft.v = bounds.top;
			unchanged = false;
			if (theObject->what == kDeluxeTrans)
			{
				theObject->data.d.tall = ((RectWide(&bounds) / 4) << 8) + 
						(RectTall(&bounds) / 4);
			}
		}
		if ((theObject->what == kDoorInLf) || 
				(theObject->what == kDoorInRt))
		{
			if (theObject->data.d.topLeft.h + 
					HalfRectWide(&srcRects[kDoorInLf]) > (kRoomWide / 2))
			{
				theObject->data.d.topLeft.h = kDoorInRtLeft;
				theObject->what = kDoorInRt;
			}
			else
			{
				theObject->data.d.topLeft.h = kDoorInLfLeft;
				theObject->what = kDoorInLf;
			}
		}
		if ((theObject->what == kDoorExRt) || 
				(theObject->what == kDoorExLf))
		{
			if (theObject->data.d.topLeft.h + 
					HalfRectWide(&srcRects[kDoorExRt]) > (kRoomWide / 2))
			{
				theObject->data.d.topLeft.h = kDoorExRtLeft;
				theObject->what = kDoorExRt;
			}
			else
			{
				theObject->data.d.topLeft.h = kDoorExLfLeft;
				theObject->what = kDoorExLf;
			}
		}
		if ((theObject->what == kWindowInLf) || 
				(theObject->what == kWindowInRt))
		{
			if (theObject->data.d.topLeft.h + 
					HalfRectWide(&srcRects[kWindowInLf]) > (kRoomWide / 2))
			{
				theObject->data.d.topLeft.h = kWindowInRtLeft;
				theObject->what = kWindowInRt;
			}
			else
			{
				theObject->data.d.topLeft.h = kWindowInLfLeft;
				theObject->what = kWindowInLf;
			}
		}
		if ((theObject->what == kWindowExRt) || 
				(theObject->what == kWindowExLf))
		{
			if (theObject->data.d.topLeft.h + 
					HalfRectWide(&srcRects[kWindowExRt]) > (kRoomWide / 2))
			{
				theObject->data.d.topLeft.h = kWindowExRtLeft;
				theObject->what = kWindowExRt;
			}
			else
			{
				theObject->data.d.topLeft.h = kWindowExLfLeft;
				theObject->what = kWindowExLf;
			}
		}
		
		if ((theObject->what == kInvisTrans) && 
				((theObject->data.d.topLeft.v + 
				theObject->data.d.tall) > kTileHigh))
		{
			theObject->data.d.tall = kTileHigh - 
				theObject->data.d.topLeft.v;
			unchanged = false;
		}
		if ((theObject->what == kInvisTrans) && 
				(theObject->data.d.wide < 0))
		{
			theObject->data.d.wide = 0;
			unchanged = false;
		}
		break;
		
		case kLightSwitch:
		case kMachineSwitch:
		case kThermostat:
		case kPowerSwitch:
		case kKnifeSwitch:
		case kInvisSwitch:
		case kTrigger:
		case kLgTrigger:
		case kSoundTrigger:
		GetObjectRect(&thisRoom->objects[objActive], &bounds);
		if (ForceRectInRect(&bounds, &roomRect))
		{
			theObject->data.e.topLeft.h = bounds.left;
			theObject->data.e.topLeft.v = bounds.top;
			unchanged = false;
		}
		if (theObject->data.e.topLeft.h % 2 != 0)
		{
			theObject->data.e.topLeft.h--;
			unchanged = false;
		}
		break;
		
		case kCeilingLight:
		case kLightBulb:
		case kTableLamp:
		case kHipLamp:
		case kDecoLamp:
		case kFlourescent:
		case kTrackLight:
		case kInvisLight:
		GetObjectRect(&thisRoom->objects[objActive], &bounds);
		if (ForceRectInRect(&bounds, &roomRect))
		{
			if ((theObject->what == kFlourescent) || (theObject->what == kTrackLight))
			{
				if (theObject->data.f.topLeft.h < bounds.left)
					theObject->data.f.topLeft.h = bounds.left;
				
				if (theObject->data.f.topLeft.v < bounds.top)
					theObject->data.f.topLeft.v = bounds.top;
				
				if ((theObject->data.f.topLeft.h + theObject->data.f.length) > bounds.right)
					theObject->data.f.length = bounds.right - theObject->data.f.topLeft.h;
			}
			else
			{
				theObject->data.f.topLeft.h = bounds.left;
				theObject->data.f.topLeft.v = bounds.top;
			}
			unchanged = false;
		}
		if (((theObject->what == kFlourescent) || 
				(theObject->what == kTrackLight)) && 
				((bounds.right > kRoomWide) || (bounds.left < 0)))
		{
			if (theObject->data.f.topLeft.h < 0)
			{
				theObject->data.f.topLeft.h = 0;
				unchanged = false;
			}
			if (bounds.left < 0)
			{
				bounds.left = 0;
				unchanged = false;
			}
			if (theObject->data.f.topLeft.h > kRoomWide)
			{
				theObject->data.f.topLeft.h = kRoomWide;
				unchanged = false;
			}
			if (bounds.right > kRoomWide)
			{
				bounds.right = kRoomWide;
				unchanged = false;
			}
			theObject->data.f.length = kRoomWide - bounds.left;
		}
		break;
		
		case kShredder:
		case kToaster:
		case kMacPlus:
		case kGuitar:
		case kTV:
		case kCoffee:
		case kOutlet:
		case kVCR:
		case kStereo:
		case kMicrowave:
		case kCinderBlock:
		case kFlowerBox:
		case kCDs:
		case kCustomPict:
		GetObjectRect(&thisRoom->objects[objActive], &bounds);
		if (ForceRectInRect(&bounds, &roomRect))
		{
			theObject->data.g.topLeft.h = bounds.left;
			theObject->data.g.topLeft.v = bounds.top;
			unchanged = false;
		}
		if ((theObject->what == kToaster) && 
				(bounds.top - theObject->data.g.height < 0))
		{
			theObject->data.g.height = bounds.top;
			unchanged = false;
		}
		if ((theObject->what == kTV) && 
				(theObject->data.g.topLeft.h % 2 == 0))
		{
			theObject->data.g.topLeft.h--;
			unchanged = false;
		}
		if (((theObject->what == kToaster) || 
				(theObject->what == kMacPlus) || 
				(theObject->what == kCoffee) || 
				(theObject->what == kOutlet) || 
				(theObject->what == kVCR) || 
				(theObject->what == kStereo) || 
				(theObject->what == kMicrowave)) && 
				(theObject->data.g.topLeft.h % 2 != 0))
		{
			theObject->data.g.topLeft.h--;
			unchanged = false;
		}
		break;
		
		case kBalloon:
		case kCopterLf:
		case kCopterRt:
		case kDartLf:
		case kDartRt:
		case kBall:
		case kDrip:
		case kFish:
		case kCobweb:
		GetObjectRect(&thisRoom->objects[objActive], &bounds);
		if (ForceRectInRect(&bounds, &roomRect))
		{
			theObject->data.h.topLeft.h = bounds.left;
			theObject->data.h.topLeft.v = bounds.top;
			unchanged = false;
		}
		if (((theObject->what == kBall) || 
				(theObject->what == kFish)) &&
				(bounds.top - theObject->data.h.length < 0))
		{
			theObject->data.h.length = bounds.top;
			unchanged = false;
		}
		if ((theObject->what == kDrip) && 
				(bounds.bottom + theObject->data.h.length > kTileHigh))
		{
			theObject->data.h.length = kTileHigh - bounds.bottom;
			unchanged = false;
		}
		if (((theObject->what == kBalloon) || 
				(theObject->what == kCopterLf) || 
				(theObject->what == kCopterRt) || 
				(theObject->what == kBall) || 
				(theObject->what == kDrip) || 
				(theObject->what == kFish)) && 
				(theObject->data.h.topLeft.h % 2 != 0))
		{
			theObject->data.h.topLeft.h--;
			unchanged = false;
		}
		break;
		
		case kOzma:
		case kMirror:
		case kMousehole:
		case kFireplace:
		case kFlower:
		case kWallWindow:
		case kBear:
		case kCalendar:
		case kVase1:
		case kVase2:
		case kBulletin:
		case kCloud:
		case kFaucet:
		case kRug:
		case kChimes:
		GetObjectRect(&thisRoom->objects[objActive], &bounds);
		if (ForceRectInRect(&bounds, &roomRect))
		{
			theObject->data.i.bounds = bounds;
			unchanged = false;
		}
		if (theObject->what == kMirror)
		{
			if (theObject->data.i.bounds.left % 2 != 0)
			{
				theObject->data.i.bounds.left--;
				unchanged = false;
			}
			if (theObject->data.i.bounds.right % 2 != 0)
			{
				theObject->data.i.bounds.right--;
				unchanged = false;
			}
		}
		break;
		
	}
	
#endif
	
	return (unchanged);
}

//--------------------------------------------------------------  WrapBannerAndTrailer

// Tries to wrap around the text of the banner and trailer messages.

#ifndef COMPILEDEMO
void WrapBannerAndTrailer (void)
{
	char		wasState;
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	
	WrapText((*thisHouse)->banner, 40);
	WrapText((*thisHouse)->trailer, 64);
	
	HSetState((Handle)thisHouse, wasState);
}

//--------------------------------------------------------------  ValidateNumberOfRooms

// Makes sure the number of room count and actual number of rooms match.

void ValidateNumberOfRooms (void)
{
	long		countedRooms, reportsRooms;
	char		wasState;
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	
	reportsRooms = (long)(*thisHouse)->nRooms;
	countedRooms = (GetHandleSize((Handle)thisHouse) - 
			sizeof(houseType)) / sizeof(roomType);
	if (reportsRooms != countedRooms)
	{
		(*thisHouse)->nRooms = (short)countedRooms;
		numberRooms = (*thisHouse)->nRooms;
		houseErrors++;
	}
	
	HSetState((Handle)thisHouse, wasState);
}

//--------------------------------------------------------------  CheckDuplicateFloorSuite

// Error check, looks for rooms with the same floor suite (stacked).

void CheckDuplicateFloorSuite (void)
{
	#define		kRoomsTimesSuites	8192
	short		i, numRooms, bitPlace;
	char		*pidgeonHoles;
	char		wasState;
	
	pidgeonHoles = (char *)NewPtrClear(sizeof(char) * kRoomsTimesSuites);
	if (pidgeonHoles == nil)
		return;
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	
	numRooms = (*thisHouse)->nRooms;
	for (i = 0; i < numRooms; i++)
	{
		if ((*thisHouse)->rooms[i].suite != kRoomIsEmpty)
		{
			bitPlace = (((*thisHouse)->rooms[i].floor + 7) * 128) + 
					(*thisHouse)->rooms[i].suite;
			if ((bitPlace < 0) || (bitPlace >= 8192))
				DebugStr("\pBlew array");
			if (pidgeonHoles[bitPlace] != 0)
			{
				houseErrors++;
				(*thisHouse)->rooms[i].suite = kRoomIsEmpty;
			}
			else
				pidgeonHoles[bitPlace]++;
		}
	}
	
	HSetState((Handle)thisHouse, wasState);
	
	DisposePtr((Ptr)pidgeonHoles);
}

//--------------------------------------------------------------  CompressHouse

// Removes place-holder (deleted) rooms from the middle of the file.

void CompressHouse (void)
{
	short		wasFirstRoom, roomNumber, probe;
	char		wasState;
	Boolean		compressing, probing;
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	
	wasFirstRoom = (*thisHouse)->firstRoom;
	compressing = true;
	roomNumber = (*thisHouse)->nRooms - 1;	// start with last room
	do
	{
		if ((*thisHouse)->rooms[roomNumber].suite != kRoomIsEmpty)
		{									// if not an empty roomÉ
			probe = 0;						// start looking for empty slot
			probing = true;
			do
			{								// test room at probe to see if empty
				if ((*thisHouse)->rooms[probe].suite == kRoomIsEmpty)
				{							// if it is, copy room there
					(*thisHouse)->rooms[probe] = (*thisHouse)->rooms[roomNumber];
					(*thisHouse)->rooms[roomNumber].suite = kRoomIsEmpty;
					if (roomNumber == wasFirstRoom)
						(*thisHouse)->firstRoom = probe;
					if (roomNumber == wasRoom)
						wasRoom = probe;
					probing = false;
				}
				probe++;					// bump probe up to next room
				if ((probing) && (probe >= roomNumber))
				{							// we reached the current room
					probing = false;		// we can look no further
					compressing = false;	// so we can compress no more
				}
			}
			while (probing);
		}
		roomNumber--;						// go on to room preceding
		if (roomNumber <= 0)				// stop if we reach the first room
			compressing = false;
	}
	while (compressing);
	
	HSetState((Handle)thisHouse, wasState);
}

//--------------------------------------------------------------  LopOffExtraRooms

// Deletes all empty rooms hanging off the end of the house file.

void LopOffExtraRooms (void)
{
	long		newSize;
	short		r, count;
	char		wasState;
	Str255		message;
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	
	count = 0;
	r = (*thisHouse)->nRooms;		// begin at last room
	do
	{
		r--;						// look for trailing empties
		if ((*thisHouse)->rooms[r].suite == kRoomIsEmpty)
			count++;
		else
			r = 0;
	}
	while (r > 0);
	
	if (count > 0)					// if there were trailing emptiesÉ
	{
		r = (*thisHouse)->nRooms - count;
		newSize = sizeof(houseType) + (sizeof(roomType) * (long)r);
		HUnlock((Handle)thisHouse);	// resize house handle (shrink)
		SetHandleSize((Handle)thisHouse, newSize);
		if (MemError() != noErr)	// problem?
		{
			ForeColor(redColor);
			GetLocalizedString(16, message);
			SetMessageWindowMessage(message);
		}
		HLock((Handle)thisHouse);	// reflect new room count
		(*thisHouse)->nRooms -= count;
		numberRooms = (*thisHouse)->nRooms;
	}
	HSetState((Handle)thisHouse, wasState);
}

//--------------------------------------------------------------  ValidateRoomNumbers

// Error check - ensures that the floor and suite numbers are within legal ranges.

void ValidateRoomNumbers (void)
{
	short		i, numRooms;
	char		wasState;
	Str255		message;
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	
	numRooms = (*thisHouse)->nRooms;
	if (numRooms < 0)
	{
		(*thisHouse)->nRooms = 0;
		numRooms = 0;
	}
	for (i = 0; i < numRooms; i++)
	{
		if ((*thisHouse)->rooms[i].suite != kRoomIsEmpty)
		{
			if (((*thisHouse)->rooms[i].floor > 56) || 
					((*thisHouse)->rooms[i].floor < -7))
			{
				(*thisHouse)->rooms[i].suite = kRoomIsEmpty;
				ForeColor(redColor);
				GetLocalizedString(17, message);
				SetMessageWindowMessage(message);
				houseErrors++;
				ForeColor(blackColor);
			}
			if (((*thisHouse)->rooms[i].suite >= 128) || 
					((*thisHouse)->rooms[i].suite < 0))
			{
				(*thisHouse)->rooms[i].suite = kRoomIsEmpty;
				ForeColor(redColor);
				GetLocalizedString(18, message);
				SetMessageWindowMessage(message);
				houseErrors++;
				ForeColor(blackColor);
			}
		}
	}
	
	HSetState((Handle)thisHouse, wasState);
}

//--------------------------------------------------------------  CountUntitledRooms

// Returns the number of rooms left "Untitled".

void CountUntitledRooms (void)
{
	short		i, numRooms;
	char		wasState;
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	
	numRooms = (*thisHouse)->nRooms;
	for (i = 0; i < numRooms; i++)
	{
		if (((*thisHouse)->rooms[i].suite != kRoomIsEmpty) && 
				(EqualString((*thisHouse)->rooms[i].name, "\pUntitled Room", false, true)))
			houseErrors++;
	}
	
	HSetState((Handle)thisHouse, wasState);
}

//--------------------------------------------------------------  CheckRoomNameLength

// Error check - ensures the length of the room name is legal.

void CheckRoomNameLength (void)
{
	short		i, numRooms;
	char		wasState;
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	
	numRooms = (*thisHouse)->nRooms;
	for (i = 0; i < numRooms; i++)
	{
		(*thisHouse)->rooms[i].unusedByte = 0;
		
		if (((*thisHouse)->rooms[i].suite != kRoomIsEmpty) && 
				((*thisHouse)->rooms[i].name[0] > 27))
		{
			(*thisHouse)->rooms[i].name[0] = 27;
			houseErrors++;
		}
	}
	
	HSetState((Handle)thisHouse, wasState);
}

//--------------------------------------------------------------  MakeSureNumObjectsJives

// Error check - ensures the actual count of objects equals number of objects.

void MakeSureNumObjectsJives (void)
{
	short		i, h, numRooms, count;
	char		wasState;
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	
	numRooms = (*thisHouse)->nRooms;
	for (i = 0; i < numRooms; i++)
	{
		if ((*thisHouse)->rooms[i].suite != kRoomIsEmpty)
		{
			count = 0;
			for (h = 0; h < kMaxRoomObs; h++)
			{
				if ((*thisHouse)->rooms[i].objects[h].what != kObjectIsEmpty)
					count++;
			}
			if (count != (*thisHouse)->rooms[i].numObjects)
			{
				houseErrors++;
				(*thisHouse)->rooms[i].numObjects = count;
			}
		}
	}
	
	HSetState((Handle)thisHouse, wasState);
}

//--------------------------------------------------------------  KeepAllObjectsLegal

// Repeatedly calls KeepObjectLegal() on ALL objects in a house.  Wow!

void KeepAllObjectsLegal (void)
{
	short		i, h, numRooms;
	char		wasState;
	Str255		message;
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	
	numRooms = (*thisHouse)->nRooms;
	for (i = 0; i < numRooms; i++)
	{
		if ((*thisHouse)->rooms[i].suite != kRoomIsEmpty)
		{
			ForceThisRoom(i);
			for (h = 0; h < kMaxRoomObs; h++)
			{
				objActive = h;
				if (thisRoom->objects[objActive].what != kObjectIsEmpty)
				{
					if (!KeepObjectLegal())
					{
						ForeColor(redColor);
						GetLocalizedString(19, message);
						SetMessageWindowMessage(message);
						houseErrors++;
						ForeColor(blackColor);
						DelayTicks(60);
					}
				}
			}
			CopyThisRoomToRoom();
		}
	}
	
	HSetState((Handle)thisHouse, wasState);
}

//--------------------------------------------------------------  CheckForStaircasePairs

// Ensures that for every up-stair there is a down-stair.

void CheckForStaircasePairs (void)
{
	short		i, h, g, numRooms, neighbor;
	char		wasState;
	Boolean		hasStairs;
	Str255		message;
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	
	numRooms = (*thisHouse)->nRooms;
	for (i = 0; i < numRooms; i++)
	{
		if ((*thisHouse)->rooms[i].suite != kRoomIsEmpty)
		{
			for (h = 0; h < kMaxRoomObs; h++)
			{
				if ((*thisHouse)->rooms[i].objects[h].what == kUpStairs)
				{
					thisRoomNumber = i;
					neighbor = GetNeighborRoomNumber(kNorthRoom);
					if (neighbor == kRoomIsEmpty)
					{
						ForeColor(redColor);
						GetLocalizedString(20, message);
						SetMessageWindowMessage(message);
						ForeColor(blackColor);
						DelayTicks(60);
					}
					else
					{
						hasStairs = false;
						for (g = 0; g < kMaxRoomObs; g++)
						{
							if ((*thisHouse)->rooms[neighbor].objects[g].what == 
									kDownStairs)
								hasStairs = true;
						}
						if (!hasStairs)
						{
							ForeColor(redColor);
							GetLocalizedString(21, message);
							SetMessageWindowMessage(message);
							ForeColor(blackColor);
							DelayTicks(60);
						}
					}
				}
				else if ((*thisHouse)->rooms[i].objects[h].what == kDownStairs)
				{
					thisRoomNumber = i;
					neighbor = GetNeighborRoomNumber(kSouthRoom);
					if (neighbor == kRoomIsEmpty)
					{
						ForeColor(redColor);
						GetLocalizedString(22, message);
						SetMessageWindowMessage(message);
						ForeColor(blackColor);
						DelayTicks(60);
					}
					else
					{
						hasStairs = false;
						for (g = 0; g < kMaxRoomObs; g++)
						{
							if ((*thisHouse)->rooms[neighbor].objects[g].what == 
									kUpStairs)
								hasStairs = true;
						}
						if (!hasStairs)
						{
							ForeColor(redColor);
							GetLocalizedString(23, message);
							SetMessageWindowMessage(message);
							ForeColor(blackColor);
							DelayTicks(60);
						}
					}
				}
			}
		}
	}
	
	HSetState((Handle)thisHouse, wasState);
}
#endif

//--------------------------------------------------------------  CheckHouseForProblems

// Calls all the above functions and reports (and corrects) errors.

void CheckHouseForProblems (void)
{
#ifndef COMPILEDEMO
	Str255		message, message2;
	short		wasActive;
	
	houseErrors = 0;
	CopyThisRoomToRoom();
	wasRoom = thisRoomNumber;
	wasActive = objActive;
	GetLocalizedString(24, message);
	OpenMessageWindow(message);
	
	SpinCursor(3);
	GetLocalizedString(25, message);
	SetMessageWindowMessage(message);
	WrapBannerAndTrailer();
	
	if (isHouseChecks)
	{
		SpinCursor(3);
		GetLocalizedString(26, message);
		SetMessageWindowMessage(message);
		ValidateNumberOfRooms();
		if (houseErrors != 0)
		{
			GetLocalizedString(27, message);
			SetMessageWindowMessage(message);
			DelayTicks(60);
			houseErrors = 0;
		}
	}
	
	if (isHouseChecks)
	{
		SpinCursor(3);
		houseErrors = 0;
		CheckDuplicateFloorSuite();
		if (houseErrors != 0)
		{
			NumToString((long)houseErrors, message);
			GetLocalizedString(28, message2);
			PasStringConcat(message, message2);
			ForeColor(redColor);
			SetMessageWindowMessage(message);
			ForeColor(blackColor);
			DelayTicks(45);
		}
	}
	
	SpinCursor(3);
	CompressHouse();
	SpinCursor(3);
	LopOffExtraRooms();
	
	if (isHouseChecks)
	{
		SpinCursor(3);
		ValidateRoomNumbers();
		if (houseErrors != 0)
		{
			NumToString((long)houseErrors, message);
			GetLocalizedString(29, message2);
			PasStringConcat(message, message2);
			ForeColor(redColor);
			SetMessageWindowMessage(message);
			ForeColor(blackColor);
			DelayTicks(60);
		}
	}
	
	if (isHouseChecks)
	{
		SpinCursor(3);
		houseErrors = 0;
		CountUntitledRooms();
		if (houseErrors != 0)
		{
			NumToString((long)houseErrors, message);
			GetLocalizedString(30, message2);
			PasStringConcat(message, message2);
			ForeColor(blueColor);
			SetMessageWindowMessage(message);
			ForeColor(blackColor);
			DelayTicks(45);
		}
	}
	
	if (isHouseChecks)
	{
		SpinCursor(3);
		houseErrors = 0;
		CheckRoomNameLength();
		if (houseErrors != 0)
		{
			NumToString((long)houseErrors, message);
			GetLocalizedString(31, message2);
			PasStringConcat(message, message2);
			ForeColor(blueColor);
			SetMessageWindowMessage(message);
			ForeColor(blackColor);
			DelayTicks(45);
		}
	}
	
	if (isHouseChecks)
	{
		SpinCursor(3);
		houseErrors = 0;
		MakeSureNumObjectsJives();
		if (houseErrors != 0)
		{
			NumToString((long)houseErrors, message);
			GetLocalizedString(32, message2);
			PasStringConcat(message, message2);
			ForeColor(redColor);
			SetMessageWindowMessage(message);
			ForeColor(blackColor);
			DelayTicks(60);
		}
	}
	
	if (isHouseChecks)
	{
		SpinCursor(3);
		houseErrors = 0;
		GetLocalizedString(33, message);
		SetMessageWindowMessage(message);
		KeepAllObjectsLegal();
		if (houseErrors != 0)
		{
			NumToString((long)houseErrors, message);
			GetLocalizedString(34, message2);
			PasStringConcat(message, message2);
			ForeColor(redColor);
			SetMessageWindowMessage(message);
			ForeColor(blackColor);
			DelayTicks(60);
		}
	}
	
	if (isHouseChecks)
	{
		SpinCursor(3);
		houseErrors = 0;
		CheckForStaircasePairs();
	}
	
	if (isHouseChecks)
	{
		SpinCursor(3);
		if (CountStarsInHouse() < 1)
		{
			ForeColor(redColor);
			GetLocalizedString(35, message);
			SetMessageWindowMessage(message);
			ForeColor(blackColor);
			DelayTicks(60);
		}
	}
	
	InitCursor();
	CloseMessageWindow();
	ForceThisRoom(wasRoom);
	objActive = wasActive;
#endif
}

