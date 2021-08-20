//============================================================================
//----------------------------------------------------------------------------
//									Objects.c
//----------------------------------------------------------------------------
//============================================================================


#include "Externs.h"
#include "ObjectEdit.h"


#define kMaxTempManholes			8


short GetObjectLinked (objectType *);
void ListOneRoomsObjects (short);


Rect		blowerSrcRect;									// Blowers
GWorldPtr	blowerSrcMap;
GWorldPtr	blowerMaskMap;
Rect		flame[kNumCandleFlames], tikiFlame[kNumTikiFlames];
Rect		coals[kNumBBQCoals];
Rect		furnitureSrcRect;								// Furniture
GWorldPtr	furnitureSrcMap;
GWorldPtr	furnitureMaskMap;
Rect		tableSrc, shelfSrc, hingeSrc, handleSrc, knobSrc;
Rect		leftFootSrc, rightFootSrc, deckSrc;
Rect		bonusSrcRect;									// Bonuses
GWorldPtr	bonusSrcMap;
GWorldPtr	bonusMaskMap;
Rect		pointsSrcRect;
GWorldPtr	pointsSrcMap;
GWorldPtr	pointsMaskMap;
Rect		starSrc[6], sparkleSrc[kNumSparkleModes];
Rect		digits[11], pendulumSrc[3], greaseSrcRt[4], greaseSrcLf[4];
Rect		transSrcRect;									// Transport
GWorldPtr	transSrcMap;
GWorldPtr	transMaskMap;
Rect		switchSrcRect;									// Switches
GWorldPtr	switchSrcMap;
Rect		lightSwitchSrc[2], machineSwitchSrc[2], thermostatSrc[2];
Rect		powerSrc[2], knifeSwitchSrc[2];
Rect		lightSrcRect;									// Lights
GWorldPtr	lightSrcMap;
GWorldPtr	lightMaskMap;
Rect		flourescentSrc1, flourescentSrc2, trackLightSrc[kNumTrackLights];
Rect		applianceSrcRect, toastSrcRect, shredSrcRect;	// Appliances
GWorldPtr	applianceSrcMap, toastSrcMap, shredSrcMap;
GWorldPtr	applianceMaskMap, toastMaskMap, shredMaskMap;
Rect		plusScreen1, plusScreen2, tvScreen1, tvScreen2;
Rect		coffeeLight1, coffeeLight2, vcrTime1, vcrTime2;
Rect		stereoLight1, stereoLight2, microOn, microOff;
Rect		outletSrc[kNumOutletPicts];
Rect		balloonSrcRect, copterSrcRect, dartSrcRect;		// Enemies
Rect		ballSrcRect, dripSrcRect, enemySrcRect;
Rect		fishSrcRect;
GWorldPtr	balloonSrcMap, copterSrcMap, dartSrcMap;
GWorldPtr	ballSrcMap, dripSrcMap, enemySrcMap;
GWorldPtr	fishSrcMap;
GWorldPtr	balloonMaskMap, copterMaskMap, dartMaskMap;
GWorldPtr	ballMaskMap, dripMaskMap, enemyMaskMap;
GWorldPtr	fishMaskMap;
Rect		balloonSrc[kNumBalloonFrames], copterSrc[kNumCopterFrames];
Rect		dartSrc[kNumDartFrames], ballSrc[kNumBallFrames];
Rect		dripSrc[kNumDripFrames], fishSrc[kNumFishFrames];
GWorldPtr	clutterSrcMap;									// Clutter
GWorldPtr	clutterMaskMap;
Rect		clutterSrcRect;
Rect		flowerSrc[kNumFlowers];
Rect		*srcRects;
Rect		tempManholes[kMaxTempManholes];
savedType	savedMaps[kMaxSavedMaps];
objDataPtr	masterObjects;
hotPtr		hotSpots;
short		nLocalObj, nHotSpots, numMasterObjects, numLocalMasterObjects;
short		numTempManholes, tvWithMovieNumber;
Boolean		newState;

extern	linksPtr	linksList;
extern	short		srcLocations[], destLocations[];
extern	short		localNumbers[];
extern	short		numNeighbors;


//==============================================================  Functions
//--------------------------------------------------------------  IsThisValid

Boolean IsThisValid (short where, short who)
{
	char		wasState;
	Boolean		itsGood;
	
	itsGood = true;
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	switch ((*thisHouse)->rooms[where].objects[who].what)
	{
		case kObjectIsEmpty:
		itsGood = false;
		break;
		
		case kRedClock:
		case kBlueClock:
		case kYellowClock:
		case kCuckoo:
		case kPaper:
		case kBattery:
		case kBands:
		case kFoil:
		case kInvisBonus:
		case kStar:
		case kSparkle:
		case kHelium:
		itsGood = (*thisHouse)->rooms[where].objects[who].data.c.state;
		break;
	}
	HSetState((Handle)thisHouse, wasState);
	
	return (itsGood);
}

//--------------------------------------------------------------  GetRoomLinked

short GetRoomLinked (objectType *who)
{
	short		compoundRoomNumber, whereLinked;
	short		floor, suite;
	
	switch (who->what)
	{
		case kMailboxLf:
		case kMailboxRt:
		case kFloorTrans:
		case kCeilingTrans:
		case kInvisTrans:
		case kDeluxeTrans:
		compoundRoomNumber = who->data.d.where;
		if (compoundRoomNumber != -1)	// is object linked
		{
			ExtractFloorSuite(compoundRoomNumber, &floor, &suite);
			whereLinked = GetRoomNumber(floor, suite);
		}
		else
			whereLinked = -1;			// not linked
		break;
		
		case kLightSwitch:
		case kMachineSwitch:
		case kThermostat:
		case kPowerSwitch:
		case kKnifeSwitch:
		case kInvisSwitch:
		case kTrigger:
		case kLgTrigger:
		compoundRoomNumber = who->data.e.where;
		if (compoundRoomNumber != -1)	// is object linked
		{
			ExtractFloorSuite(compoundRoomNumber, &floor, &suite);
			whereLinked = GetRoomNumber(floor, suite);
		}
		else
			whereLinked = -1;			// not linked
		break;
		
		default:
		whereLinked = -1;
		break;
	}
	
	return (whereLinked);
}

//--------------------------------------------------------------  GetObjectLinked

short GetObjectLinked (objectType *who)
{
	short		whoLinked;
	
	switch (who->what)
	{
		case kMailboxLf:
		case kMailboxRt:
		case kFloorTrans:
		case kCeilingTrans:
		case kInvisTrans:
		case kDeluxeTrans:
		if (who->data.d.who != 255)		// is it linked?
			whoLinked = (short)who->data.d.who;
		else
			whoLinked = -1;				// object not linked
		break;
		
		case kLightSwitch:
		case kMachineSwitch:
		case kThermostat:
		case kPowerSwitch:
		case kKnifeSwitch:
		case kInvisSwitch:
		case kTrigger:
		case kLgTrigger:
		if (who->data.e.who != 255)		// is it linked?
			whoLinked = (short)who->data.e.who;
		else
			whoLinked = -1;				// object not linked
		break;
		
		default:
		whoLinked = -1;
		break;
	}
	
	return (whoLinked);
}

//--------------------------------------------------------------  ObjectIsLinkTransport

Boolean ObjectIsLinkTransport (objectType *who)
{
	Boolean		itIs;
	
	itIs = false;
	if ((who->what == kMailboxLf) || (who->what == kMailboxRt) || 
			(who->what == kFloorTrans) || (who->what == kCeilingTrans) || 
			(who->what == kInvisTrans) || (who->what == kDeluxeTrans))
	{
		itIs = true;
	}
	
	return (itIs);
}

//--------------------------------------------------------------  ObjectIsLinkSwitch

Boolean ObjectIsLinkSwitch (objectType *who)
{
	Boolean		itIs;
	
	itIs = false;
	if ((who->what == kLightSwitch) || (who->what == kMachineSwitch) || 
			(who->what == kThermostat) || (who->what == kPowerSwitch) || 
			(who->what == kKnifeSwitch) || (who->what == kInvisSwitch) || 
			(who->what == kTrigger) || (who->what == kLgTrigger))
	{
		itIs = true;
	}
	
	return (itIs);
}

//--------------------------------------------------------------  ListOneRoomsObjects

void ListOneRoomsObjects (short where)
{
	objectType	thisObject;
	short		roomNum, n;
	char		wasState;
	
	roomNum = localNumbers[where];
	if (roomNum == kRoomIsEmpty)
		return;
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	for (n = 0; n < kMaxRoomObs; n++)
	{
		if (numMasterObjects < kMaxMasterObjects)
		{
			thisObject = (*thisHouse)->rooms[roomNum].objects[n];
			
			masterObjects[numMasterObjects].roomNum = roomNum;
			masterObjects[numMasterObjects].objectNum = n;
			masterObjects[numMasterObjects].roomLink = 
					GetRoomLinked(&thisObject);
			masterObjects[numMasterObjects].objectLink = 
					GetObjectLinked(&thisObject);
			masterObjects[numMasterObjects].localLink = -1;
			
			masterObjects[numMasterObjects].theObject = 
					(*thisHouse)->rooms[roomNum].objects[n];
			
			if ((where == kCentralRoom) && (IsThisValid(roomNum, n)))
				masterObjects[numMasterObjects].hotNum = CreateActiveRects(n);
			else
				masterObjects[numMasterObjects].hotNum = -1;
			masterObjects[numMasterObjects].dynaNum = -1;
			
			numMasterObjects++;
			
			if (where == kCentralRoom)
				numLocalMasterObjects++;
		}
	}
	HSetState((Handle)thisHouse, wasState);
}

//--------------------------------------------------------------  ListAllLocalObjects

void ListAllLocalObjects (void)
{
	short		i, n;
	char		wasState;
	
	numMasterObjects = 0;
	numLocalMasterObjects = 0;
	nHotSpots = 0;
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	
	ListOneRoomsObjects(kCentralRoom);
	
	if (numNeighbors > 1)
	{
		ListOneRoomsObjects(kEastRoom);
		ListOneRoomsObjects(kWestRoom);
	}
	
	if (numNeighbors > 3)
	{
		ListOneRoomsObjects(kNorthRoom);
		ListOneRoomsObjects(kNorthEastRoom);
		ListOneRoomsObjects(kSouthEastRoom);
		ListOneRoomsObjects(kSouthRoom);
		ListOneRoomsObjects(kSouthWestRoom);
		ListOneRoomsObjects(kNorthWestRoom);
	}
	
	HSetState((Handle)thisHouse, wasState);
	
	for (i = 0; i < numMasterObjects; i++)				// correlate links with…
	{													// index into this list
		if ((masterObjects[i].roomLink != -1) && 		// if object has a link
				(masterObjects[i].objectLink != -1))
		{
			for (n = 0; n < numMasterObjects; n++)		// search for the object…
			{											// linked to in this list
				if ((masterObjects[i].roomLink == masterObjects[n].roomNum) && 
						(masterObjects[i].objectLink == masterObjects[n].objectNum))
				{
					masterObjects[i].localLink = n;		// log the index
				}
			}
		}
	}
}

//--------------------------------------------------------------  AddTempManholeRect

void AddTempManholeRect (Rect *manHole)
{
	Rect		tempRect;
	
	if (numTempManholes < kMaxTempManholes)
	{
		tempRect = *manHole;
		tempRect.bottom = tempRect.top + kFloorSupportTall;
		tempManholes[numTempManholes] = tempRect;
		numTempManholes++;
	}
}

//--------------------------------------------------------------  SetObjectState

Boolean SetObjectState (short room, short object, short action, short local)
{
	char		wasState;
	Boolean		changed;
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	switch ((*thisHouse)->rooms[room].objects[object].what)
	{
		case kFloorVent:
		case kCeilingVent:
		case kFloorBlower:
		case kCeilingBlower:
		case kLeftFan:
		case kRightFan:
		case kSewerGrate:
		case kInvisBlower:
		case kGrecoVent:
		case kSewerBlower:
		case kLiftArea:
		switch (action)
		{
			case kToggle:
			newState = !(*thisHouse)->rooms[room].objects[object].data.a.state;
			(*thisHouse)->rooms[room].objects[object].data.a.state = newState;
			changed = true;
			break;
			
			case kForceOn:
			changed = ((*thisHouse)->rooms[room].objects[object].data.a.state == false);
			newState = true;
			(*thisHouse)->rooms[room].objects[object].data.a.state = newState;
			break;
			
			case kForceOff:
			changed = ((*thisHouse)->rooms[room].objects[object].data.a.state == true);
			newState = false;
			(*thisHouse)->rooms[room].objects[object].data.a.state = newState;
			break;
		}
		if ((changed) && (local != -1))
		{
			masterObjects[local].theObject.data.a.state = newState;
			if (room == thisRoomNumber)
				thisRoom->objects[object].data.a.state = newState;
			if (newState)
				PlayPrioritySound(kBlowerOn, kBlowerOnPriority);
			else
				PlayPrioritySound(kBlowerOff, kBlowerOffPriority);
			if (masterObjects[local].hotNum != -1)
				hotSpots[masterObjects[local].hotNum].isOn = newState;
		}
		break;
		
		case kTaper:
		case kCandle:
		case kStubby:
		case kTiki:
		case kBBQ:
		changed = false;	// Cannot switch on/off these
		break;
		
		case kTable:
		case kShelf:
		case kCabinet:
		case kFilingCabinet:
		case kWasteBasket:
		case kMilkCrate:
		case kCounter:
		case kDresser:
		case kStool:
		case kTrunk:
		case kDeckTable:
		case kInvisObstacle:
		case kManhole:
		case kBooks:
		case kInvisBounce:
		changed = false;	// Cannot switch on/off these
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
		changed = ((*thisHouse)->rooms[room].objects[object].data.c.state == true);
		newState = false;
		(*thisHouse)->rooms[room].objects[object].data.c.state = newState;
		if ((changed) && (local != -1))
		{
			masterObjects[local].theObject.data.a.state = false;
			if (room == thisRoomNumber)
			{
				thisRoom->objects[object].data.c.state = false;
				if (masterObjects[local].hotNum != -1)
					hotSpots[masterObjects[local].hotNum].isOn = false;
			}
		}
		break;
		
		case kSlider:
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
		changed = false;
		break;
		
		case kDeluxeTrans:
		switch (action)
		{
			case kToggle:
			newState = (*thisHouse)->rooms[room].objects[object].data.d.wide & 0x0F;
			newState = !newState;
			(*thisHouse)->rooms[room].objects[object].data.d.wide &= 0xF0;
			(*thisHouse)->rooms[room].objects[object].data.d.wide += newState;
			changed = true;
			break;
			
			case kForceOn:
			changed = (((*thisHouse)->rooms[room].objects[object].data.d.wide & 0x0F) == 0x00);
			newState = true;
			(*thisHouse)->rooms[room].objects[object].data.d.wide &= 0xF0;
			(*thisHouse)->rooms[room].objects[object].data.d.wide += newState;
			break;
			
			case kForceOff:
			changed = (((*thisHouse)->rooms[room].objects[object].data.d.wide & 0x0F) != 0x00);
			newState = false;
			(*thisHouse)->rooms[room].objects[object].data.d.wide &= 0xF0;
			(*thisHouse)->rooms[room].objects[object].data.d.wide += newState;
			break;
		}
		if ((changed) && (local != -1))
		{
			masterObjects[local].theObject.data.d.wide = 
					(*thisHouse)->rooms[room].objects[object].data.d.wide;
			if (room == thisRoomNumber)
				thisRoom->objects[object].data.d.wide = 
						(*thisHouse)->rooms[room].objects[object].data.d.wide;
			if (masterObjects[local].hotNum != -1)
				hotSpots[masterObjects[local].hotNum].isOn = newState;
		}
		break;
		
		case kLightSwitch:
		case kMachineSwitch:
		case kThermostat:
		case kPowerSwitch:
		case kInvisSwitch:
		case kTrigger:
		case kLgTrigger:
		case kSoundTrigger:
		changed = false;
		break;
		
		case kCeilingLight:
		case kLightBulb:
		case kTableLamp:
		case kHipLamp:
		case kDecoLamp:
		case kFlourescent:
		case kTrackLight:
		case kInvisLight:
		switch (action)
		{
			case kToggle:
			newState = !(*thisHouse)->rooms[room].objects[object].data.f.state;
			(*thisHouse)->rooms[room].objects[object].data.f.state = newState;
			changed = true;
			break;
			
			case kForceOn:
			changed = ((*thisHouse)->rooms[room].objects[object].data.f.state == false);
			newState = true;
			(*thisHouse)->rooms[room].objects[object].data.f.state = newState;
			break;
			
			case kForceOff:
			changed = ((*thisHouse)->rooms[room].objects[object].data.f.state == true);
			newState = false;
			(*thisHouse)->rooms[room].objects[object].data.f.state = newState;
			break;
		}
		if ((changed) && (local != -1))
		{
			masterObjects[local].theObject.data.f.state = newState;
			if (room == thisRoomNumber)
				thisRoom->objects[object].data.f.state = newState;
		}
		break;
		
		case kGuitar:		// really no point to change this state
		changed = false;
		break;
		
		case kStereo:
		newState = !isPlayMusicGame;
		isPlayMusicGame = newState;
		changed = true;
		break;
		
		case kShredder:
		case kToaster:
		case kMacPlus:
		case kTV:
		case kCoffee:
		case kOutlet:
		case kVCR:
		case kMicrowave:
		switch (action)
		{
			case kToggle:
			newState = !(*thisHouse)->rooms[room].objects[object].data.g.state;
			(*thisHouse)->rooms[room].objects[object].data.g.state = newState;
			changed = true;
			break;
			
			case kForceOn:
			changed = ((*thisHouse)->rooms[room].objects[object].data.g.state == false);
			newState = true;
			(*thisHouse)->rooms[room].objects[object].data.g.state = newState;
			break;
			
			case kForceOff:
			changed = ((*thisHouse)->rooms[room].objects[object].data.g.state == true);
			newState = false;
			(*thisHouse)->rooms[room].objects[object].data.g.state = newState;
			break;
		}
		if ((changed) && (local != -1))
		{
			masterObjects[local].theObject.data.g.state = newState;
			if (room == thisRoomNumber)
			{
				thisRoom->objects[object].data.g.state = newState;
				if ((*thisHouse)->rooms[room].objects[object].what == kShredder)
					hotSpots[masterObjects[local].hotNum].isOn = newState;
			}
		}
		break;
		
		case kCinderBlock:
		case kFlowerBox:
		case kCDs:
		case kCustomPict:
		changed = false;
		break;
				
		case kBalloon:
		case kCopterLf:
		case kCopterRt:
		case kDartLf:
		case kDartRt:
		case kBall:
		case kDrip:
		case kFish:
		switch (action)
		{
			case kToggle:
			newState = !(*thisHouse)->rooms[room].objects[object].data.h.state;
			(*thisHouse)->rooms[room].objects[object].data.h.state = newState;
			changed = true;
			break;
			
			case kForceOn:
			changed = ((*thisHouse)->rooms[room].objects[object].data.h.state == false);
			newState = true;
			(*thisHouse)->rooms[room].objects[object].data.h.state = newState;
			break;
			
			case kForceOff:
			changed = ((*thisHouse)->rooms[room].objects[object].data.h.state == true);
			newState = false;
			(*thisHouse)->rooms[room].objects[object].data.h.state = newState;
			break;
		}
		if ((changed) && (local != -1))
		{
			masterObjects[local].theObject.data.h.state = newState;
			if (room == thisRoomNumber)
				thisRoom->objects[object].data.h.state = newState;
		}
		break;
		
		case kCobweb:
		changed = false;
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
		changed = false;
		break;
		
	}
	HSetState((Handle)thisHouse, wasState);
	
	return (changed);
}

//--------------------------------------------------------------  GetObjectState

Boolean GetObjectState (short room, short object)
{
	char		wasState;
	Boolean		theState;
	
	theState = true;
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	switch ((*thisHouse)->rooms[room].objects[object].what)
	{
		case kFloorVent:
		case kCeilingVent:
		case kFloorBlower:
		case kCeilingBlower:
		case kLeftFan:
		case kRightFan:
		case kSewerGrate:
		case kInvisBlower:
		case kGrecoVent:
		case kSewerBlower:
		case kLiftArea:
		theState = (*thisHouse)->rooms[room].objects[object].data.a.state;
		break;
		
		case kTaper:
		case kCandle:
		case kStubby:
		case kTiki:
		case kBBQ:
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
		theState = (*thisHouse)->rooms[room].objects[object].data.c.state;
		break;
		
		case kSlider:
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
		break;
		
		case kDeluxeTrans:
		theState = (*thisHouse)->rooms[room].objects[object].data.d.wide & 0x0F;
		break;
		
		case kLightSwitch:
		case kMachineSwitch:
		case kThermostat:
		case kPowerSwitch:
		case kInvisSwitch:
		case kTrigger:
		case kLgTrigger:
		case kSoundTrigger:
		break;
		
		case kCeilingLight:
		case kLightBulb:
		case kTableLamp:
		case kHipLamp:
		case kDecoLamp:
		case kFlourescent:
		case kTrackLight:
		case kInvisLight:
		theState = (*thisHouse)->rooms[room].objects[object].data.f.state;
		break;
		
		case kStereo:
		theState = isPlayMusicGame;
		break;
		
		case kShredder:
		case kToaster:
		case kMacPlus:
		case kGuitar:
		case kTV:
		case kCoffee:
		case kOutlet:
		case kVCR:
		case kMicrowave:
		theState = (*thisHouse)->rooms[room].objects[object].data.g.state;
		break;
		
		case kCinderBlock:
		case kFlowerBox:
		case kCDs:
		case kCustomPict:
		break;
		
		case kBalloon:
		case kCopterLf:
		case kCopterRt:
		case kDartLf:
		case kDartRt:
		case kBall:
		case kDrip:
		case kFish:
		theState = (*thisHouse)->rooms[room].objects[object].data.h.state;
		break;
		
		case kCobweb:
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
		break;
	}
		
	HSetState((Handle)thisHouse, wasState);
	
	return (theState);
}

//--------------------------------------------------------------  SendObjectToBack

#ifndef COMPILEDEMO
void BringSendFrontBack (Boolean bringFront)
{
	houseType	*thisHousePtr;
	objectType	savedObject;
	short		numLinks, i;
	short		srcRoom, srcObj;
	short		sorting[kMaxRoomObs];
	short		sorted[kMaxRoomObs];
	char		wasState;
	
	if (bringFront)							// No need to bring to front…
	{										// or send to back if the object…
		if (objActive == (kMaxRoomObs - 1))	// in question is already front-
			return;							// most or backmost.
	}
	else
	{
		if (objActive == 0)
			return;
	}
	
	CopyThisRoomToRoom();					// Any changes to room written…
											// back to the house handle.
	numLinks = CountHouseLinks();			// Determine space needed for all links.
	if (numLinks != 0)						// Create links list of ALL house links.
	{
		linksList = nil;
		linksList = (linksPtr)NewPtr(sizeof(linksType) * numLinks);
		if (linksList == nil)
		{
			YellowAlert(kYellowCantOrderLinks, MemError());
			return;
		}
		GenerateLinksList();				// Fill in links list with src/dest…
	}										// data on objects and room numbers.
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);				// Lock down house.
	thisHousePtr = *thisHouse;				// Get a pointer to house structure.
	
	for (i = 0; i < kMaxRoomObs; i++)		// Set up an ordered array.
		sorting[i] = i;
	
	savedObject = (*thisHouse)->rooms[thisRoomNumber].objects[objActive];
	
	if (bringFront)
	{
		for (i = objActive; i < kMaxRoomObs - 1; i++)
		{									// Pull all objects down to fill hole.
			(*thisHouse)->rooms[thisRoomNumber].objects[i] = 
					(*thisHouse)->rooms[thisRoomNumber].objects[i + 1];
			sorting[i] = sorting[i + 1];
			SpinCursor(2);
		}
											// Insert object at end of array.
		(*thisHouse)->rooms[thisRoomNumber].objects[kMaxRoomObs - 1] = savedObject;
		sorting[kMaxRoomObs - 1] = objActive;
	}
	else
	{
		for (i = objActive; i > 0; i--)
		{									// Move all objects up to fill hole.
			(*thisHouse)->rooms[thisRoomNumber].objects[i] = 
					(*thisHouse)->rooms[thisRoomNumber].objects[i - 1];
			sorting[i] = sorting[i - 1];
			SpinCursor(2);
		}
											// Insert object at beginning of array.
		(*thisHouse)->rooms[thisRoomNumber].objects[0] = savedObject;
		sorting[0] = objActive;
	}
	
	for (i = 0; i < kMaxRoomObs; i++)		// Set up retro-ordered array.
		sorted[sorting[i]] = i;
	
	for (i = 0; i < numLinks; i++)			// Walk links list in order to assign…
	{										// corrected links to objects moved.
		if (linksList[i].destRoom == thisRoomNumber)
		{									// Does link point to room we re-ordered?
			srcRoom = linksList[i].srcRoom;	// Room where-which an object is linked from.
			if (srcRoom == thisRoomNumber)	// Handle special case for local links.
				srcObj = sorted[linksList[i].srcObj];
			else
				srcObj = linksList[i].srcObj;
			
			switch ((*thisHouse)->rooms[srcRoom].objects[srcObj].what)
			{
				case kLightSwitch:
				case kMachineSwitch:
				case kThermostat:
				case kPowerSwitch:
				case kKnifeSwitch:
				case kInvisSwitch:
				case kTrigger:
				case kLgTrigger:
				(*thisHouse)->rooms[srcRoom].objects[srcObj].data.d.who = 
						sorted[linksList[i].destObj];
				break;
				
				default:
				(*thisHouse)->rooms[srcRoom].objects[srcObj].data.e.who = 
						sorted[linksList[i].destObj];
				break;
			}
		}
	}
	
	HSetState((Handle)thisHouse, wasState);
	if (linksList != nil)
		DisposePtr((Ptr)linksList);
	
	ForceThisRoom(thisRoomNumber);
	
	fileDirty = true;
	UpdateMenus(false);
	InvalWindowRect(mainWindow, &mainWindowRect);
	DeselectObject();
	GetThisRoomsObjRects();
	ReadyBackground(thisRoom->background, thisRoom->tiles);
	DrawThisRoomsObjects();
	GenerateRetroLinks();
	
	InitCursor();
}
#endif

