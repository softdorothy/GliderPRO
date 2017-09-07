
//============================================================================
//----------------------------------------------------------------------------
//								 ObjectRects.c
//----------------------------------------------------------------------------
//============================================================================


#include "Externs.h"
#include "RectUtils.h"


#define kFloorColumnWide		4
#define kCeilingColumnWide		24
#define kFanColumnThick			16
#define kFanColumnDown			20
#define kDeadlyFlameHeight		24
#define kStoolThick				25
#define kShredderActiveHigh		40


short AddActiveRect (Rect *, short, short, Boolean, Boolean);


extern	hotPtr		hotSpots;
extern	short		nHotSpots, numChimes;


//==============================================================  Functions
//--------------------------------------------------------------  GetObjectRect

void GetObjectRect (objectPtr who, Rect *itsRect)
{
	PicHandle	thePict;
	short		wide, tall;
	
	switch (who->what)
	{
		case kObjectIsEmpty:
		QSetRect(itsRect, 0, 0, 0, 0);
		break;
		
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
		*itsRect = srcRects[who->what];
		ZeroRectCorner(itsRect);
		QOffsetRect(itsRect, who->data.a.topLeft.h, who->data.a.topLeft.v);
		break;
		
		case kLiftArea:
		QSetRect(itsRect, 0, 0, who->data.a.distance, who->data.a.tall * 2);
		QOffsetRect(itsRect, who->data.a.topLeft.h, who->data.a.topLeft.v);
		break;
		
		*itsRect = srcRects[who->what];
		ZeroRectCorner(itsRect);
		QOffsetRect(itsRect, who->data.a.topLeft.h, who->data.a.topLeft.v);
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
		*itsRect = who->data.b.bounds;
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
		*itsRect = srcRects[who->what];
		ZeroRectCorner(itsRect);
		QOffsetRect(itsRect, 
				who->data.c.topLeft.h, 
				who->data.c.topLeft.v);
		break;
		
		case kSlider:
		*itsRect = srcRects[who->what];
		ZeroRectCorner(itsRect);
		QOffsetRect(itsRect, 
				who->data.c.topLeft.h, 
				who->data.c.topLeft.v);
		itsRect->right = itsRect->left + who->data.c.length;
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
		*itsRect = srcRects[who->what];
		ZeroRectCorner(itsRect);
		QOffsetRect(itsRect, 
				who->data.d.topLeft.h, 
				who->data.d.topLeft.v);
		break;
		
		case kInvisTrans:
		*itsRect = srcRects[who->what];
		ZeroRectCorner(itsRect);
		QOffsetRect(itsRect, 
				who->data.d.topLeft.h, 
				who->data.d.topLeft.v);
		itsRect->bottom = itsRect->top + who->data.d.tall;
		itsRect->right += (short)who->data.d.wide;
		break;
		
		case kDeluxeTrans:
		wide = (who->data.d.tall & 0xFF00) >> 8;		// Get high byte
		tall = who->data.d.tall & 0x00FF;				// Get low byte
		QSetRect(itsRect, 0, 0, wide * 4, tall * 4);	// Scale by 4
		QOffsetRect(itsRect, 
				who->data.d.topLeft.h, 
				who->data.d.topLeft.v);
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
		*itsRect = srcRects[who->what];
		ZeroRectCorner(itsRect);
		QOffsetRect(itsRect, 
				who->data.e.topLeft.h, 
				who->data.e.topLeft.v);
		break;
		
		case kCeilingLight:
		case kLightBulb:
		case kTableLamp:
		case kHipLamp:
		case kDecoLamp:
		case kInvisLight:
		*itsRect = srcRects[who->what];
		ZeroRectCorner(itsRect);
		QOffsetRect(itsRect, 
				who->data.f.topLeft.h, 
				who->data.f.topLeft.v);
		break;
		
		case kFlourescent:
		case kTrackLight:
		*itsRect = srcRects[who->what];
		ZeroRectCorner(itsRect);
		itsRect->right = who->data.f.length;
		QOffsetRect(itsRect, 
				who->data.f.topLeft.h, 
				who->data.f.topLeft.v);
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
		*itsRect = srcRects[who->what];
		ZeroRectCorner(itsRect);
		QOffsetRect(itsRect, 
				who->data.g.topLeft.h, 
				who->data.g.topLeft.v);
		break;
		
		case kCustomPict:
		thePict = GetPicture(who->data.g.height);
		if (thePict == nil)
		{
			who->data.g.height = 10000;
			*itsRect = srcRects[who->what];
		}
		else
		{
			HLock((Handle)thePict);
			*itsRect = (*thePict)->picFrame;
			HUnlock((Handle)thePict);
		}
		ZeroRectCorner(itsRect);
		QOffsetRect(itsRect, 
				who->data.g.topLeft.h, 
				who->data.g.topLeft.v);
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
		*itsRect = srcRects[who->what];
		ZeroRectCorner(itsRect);
		QOffsetRect(itsRect, 
				who->data.h.topLeft.h, 
				who->data.h.topLeft.v);
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
		*itsRect = who->data.i.bounds;
		break;
	}
}

//--------------------------------------------------------------  AddActiveRect

short AddActiveRect (Rect *bounds, short action, short who, Boolean isOn, 
		Boolean doScrutinize)
{
	if (nHotSpots >= kMaxHotSpots)
		return (-1);
	
	hotSpots[nHotSpots].bounds = *bounds;		// the active rect
	hotSpots[nHotSpots].action = action;		// what it does
	hotSpots[nHotSpots].who = who;				// local obj. linked to
	hotSpots[nHotSpots].isOn = isOn;			// is it active?
	hotSpots[nHotSpots].stillOver = false;
	hotSpots[nHotSpots].doScrutinize = doScrutinize;
	nHotSpots++;
	
	return (nHotSpots - 1);
}

//--------------------------------------------------------------  CreateActiveRects

short CreateActiveRects (short who)
{
	objectType	theObject;
	Rect		bounds;
	short		hotSpotNumber, wide, tall;
	Boolean		isOn;
	
	hotSpotNumber = -1;
	theObject = masterObjects[who].theObject;
	
	switch (theObject.what)
	{
		case kObjectIsEmpty:
		break;
		
		case kFloorVent:
		QSetRect(&bounds, 0, -theObject.data.a.distance, kFloorColumnWide, 0);
		QOffsetRect(&bounds, 
				HalfRectWide(&srcRects[kFloorVent]) - kFloorColumnWide / 2, 0);
		QOffsetRect(&bounds, theObject.data.a.topLeft.h, 
				theObject.data.a.topLeft.v);
		hotSpotNumber = AddActiveRect(&bounds, kLiftIt, who, theObject.data.a.state, 
				false);
		break;
		
		case kCeilingVent:
		QSetRect(&bounds, 0, 0, kCeilingColumnWide, theObject.data.a.distance);
		QOffsetRect(&bounds, 
				HalfRectWide(&srcRects[kCeilingVent]) - kCeilingColumnWide / 2, 
				0);
		QOffsetRect(&bounds, 
				theObject.data.a.topLeft.h, 
				theObject.data.a.topLeft.v);
		hotSpotNumber = AddActiveRect(&bounds, kDropIt, who, theObject.data.a.state, 
				false);
		break;
		
		case kFloorBlower:
		QSetRect(&bounds, 0, -theObject.data.a.distance, kFloorColumnWide, 0);
		QOffsetRect(&bounds, 
				HalfRectWide(&srcRects[kFloorBlower]) - kFloorColumnWide / 2, 
				0);
		QOffsetRect(&bounds, 
				theObject.data.a.topLeft.h, 
				theObject.data.a.topLeft.v);
		hotSpotNumber = AddActiveRect(&bounds, kLiftIt, who, theObject.data.a.state, 
				false);
		break;
		
		case kCeilingBlower:
		QSetRect(&bounds, 0, 0, kCeilingColumnWide, theObject.data.a.distance);
		QOffsetRect(&bounds, 
				HalfRectWide(&srcRects[kCeilingBlower]) - kCeilingColumnWide / 2, 
				0);
		QOffsetRect(&bounds, 
				theObject.data.a.topLeft.h, 
				theObject.data.a.topLeft.v);
		hotSpotNumber = AddActiveRect(&bounds, kDropIt, who, theObject.data.a.state, 
				false);
		break;
		
		case kSewerGrate:
		QSetRect(&bounds, 0, -theObject.data.a.distance, kFloorColumnWide, 0);
		QOffsetRect(&bounds, 
				HalfRectWide(&srcRects[kSewerGrate]) - kFloorColumnWide / 2, 
				0);
		QOffsetRect(&bounds, 
				theObject.data.a.topLeft.h, 
				theObject.data.a.topLeft.v);
		hotSpotNumber = AddActiveRect(&bounds, kLiftIt, who, theObject.data.a.state, 
				false);
		break;
		
		case kLeftFan:
		QSetRect(&bounds, 0, 0, 13, 43);
		QOffsetRect(&bounds, 
				theObject.data.a.topLeft.h + 16, 
				theObject.data.a.topLeft.v + 12);
		hotSpotNumber = AddActiveRect(&bounds, kDissolveIt, who, true, true);
		QSetRect(&bounds, 0, 0, theObject.data.a.distance, kFanColumnThick);
		QOffsetRect(&bounds, -(theObject.data.a.distance), kFanColumnDown);
		QOffsetRect(&bounds, 
				theObject.data.a.topLeft.h, 
				theObject.data.a.topLeft.v);
		hotSpotNumber = AddActiveRect(&bounds, kPushItLeft, who, 
				theObject.data.a.state, false);
		break;
		
		case kRightFan:
		QSetRect(&bounds, 0, 0, 13, 43);
		QOffsetRect(&bounds, 
				theObject.data.a.topLeft.h + 6, 
				theObject.data.a.topLeft.v + 12);
		hotSpotNumber = AddActiveRect(&bounds, kDissolveIt, who, true, true);
		QSetRect(&bounds, 0, 0, theObject.data.a.distance, kFanColumnThick);
		QOffsetRect(&bounds, RectWide(&srcRects[kRightFan]), kFanColumnDown);
		QOffsetRect(&bounds, 
				theObject.data.a.topLeft.h, 
				theObject.data.a.topLeft.v);
		hotSpotNumber = AddActiveRect(&bounds, kPushItRight, who, 
				theObject.data.a.state, false);
		break;
		
		case kTaper:
		QSetRect(&bounds, 0, -theObject.data.a.distance, kFloorColumnWide, 0);
		QOffsetRect(&bounds, 
				HalfRectWide(&srcRects[kTaper]) - kFloorColumnWide / 2, 
				0);
		QOffsetRect(&bounds, 
				theObject.data.a.topLeft.h, 
				theObject.data.a.topLeft.v);
		if ((bounds.bottom - bounds.top) > kDeadlyFlameHeight)
		{
			bounds.bottom -= kDeadlyFlameHeight;
			hotSpotNumber = AddActiveRect(&bounds, kLiftIt, who, true, false);
			bounds.bottom += kDeadlyFlameHeight;
			bounds.top = bounds.bottom - kDeadlyFlameHeight + 2;
			hotSpotNumber = AddActiveRect(&bounds, kBurnIt, who, true, false);
		}
		else
			hotSpotNumber = AddActiveRect(&bounds, kBurnIt, who, true, false);
		QSetRect(&bounds, 0, 0, 7, 48);
		QOffsetRect(&bounds, 
				theObject.data.a.topLeft.h + 6, 
				theObject.data.a.topLeft.v + 11);
		hotSpotNumber = AddActiveRect(&bounds, kDissolveIt, who, true, true);
		break;
		
		case kCandle:
		QSetRect(&bounds, 0, -theObject.data.a.distance, kFloorColumnWide, 0);
		QOffsetRect(&bounds, 
				HalfRectWide(&srcRects[kCandle]) - kFloorColumnWide / 2, 
				0);
		QOffsetRect(&bounds, 
				theObject.data.a.topLeft.h - 2, 
				theObject.data.a.topLeft.v);
		if ((bounds.bottom - bounds.top) > kDeadlyFlameHeight)
		{
			bounds.bottom -= kDeadlyFlameHeight;
			hotSpotNumber = AddActiveRect(&bounds, kLiftIt, who, true, false);
			bounds.bottom += kDeadlyFlameHeight;
			bounds.top = bounds.bottom - kDeadlyFlameHeight + 2;
			hotSpotNumber = AddActiveRect(&bounds, kBurnIt, who, true, false);
		}
		else
			hotSpotNumber = AddActiveRect(&bounds, kBurnIt, who, true, false);
		QSetRect(&bounds, 0, 0, 8, 20);
		QOffsetRect(&bounds, 
				theObject.data.a.topLeft.h + 9, 
				theObject.data.a.topLeft.v + 11);
		hotSpotNumber = AddActiveRect(&bounds, kDissolveIt, who, true, true);
		break;
		
		case kStubby:
		QSetRect(&bounds, 0, -theObject.data.a.distance, kFloorColumnWide, 0);
		QOffsetRect(&bounds, 
				(HalfRectWide(&srcRects[kStubby]) - kFloorColumnWide / 2) - 1, 
				0);
		QOffsetRect(&bounds, 
				theObject.data.a.topLeft.h, 
				theObject.data.a.topLeft.v);
		if ((bounds.bottom - bounds.top) > kDeadlyFlameHeight)
		{
			bounds.bottom -= kDeadlyFlameHeight;
			hotSpotNumber = AddActiveRect(&bounds, kLiftIt, who, true, false);
			bounds.bottom += kDeadlyFlameHeight;
			bounds.top = bounds.bottom - kDeadlyFlameHeight + 2;
			hotSpotNumber = AddActiveRect(&bounds, kBurnIt, who, true, false);
		}
		else
			hotSpotNumber = AddActiveRect(&bounds, kBurnIt, who, true, false);
		QSetRect(&bounds, 0, 0, 15, 26);
		QOffsetRect(&bounds, 
				theObject.data.a.topLeft.h + 1, 
				theObject.data.a.topLeft.v + 11);
		hotSpotNumber = AddActiveRect(&bounds, kDissolveIt, who, true, true);
		break;
		
		case kTiki:
		QSetRect(&bounds, 0, -theObject.data.a.distance, kFloorColumnWide, 0);
		QOffsetRect(&bounds, 
				HalfRectWide(&srcRects[kTiki]) - kFloorColumnWide / 2, 
				0);
		QOffsetRect(&bounds, 
				theObject.data.a.topLeft.h, 
				theObject.data.a.topLeft.v);
		if ((bounds.bottom - bounds.top) > kDeadlyFlameHeight)
		{
			bounds.bottom -= kDeadlyFlameHeight;
			hotSpotNumber = AddActiveRect(&bounds, kLiftIt, who, true, false);
			bounds.bottom += kDeadlyFlameHeight;
			bounds.top = bounds.bottom - kDeadlyFlameHeight + 2;
			hotSpotNumber = AddActiveRect(&bounds, kBurnIt, who, true, false);
		}
		else
			hotSpotNumber = AddActiveRect(&bounds, kBurnIt, who, true, false);
		QSetRect(&bounds, 0, 0, 15, 14);
		QOffsetRect(&bounds, 
				theObject.data.a.topLeft.h + 6, 
				theObject.data.a.topLeft.v + 6);
		hotSpotNumber = AddActiveRect(&bounds, kDissolveIt, who, true, true);
		break;
		
		case kBBQ:
		QSetRect(&bounds, 0, -theObject.data.a.distance, kFloorColumnWide, 8);
		QOffsetRect(&bounds, 
				HalfRectWide(&srcRects[kBBQ]) - kFloorColumnWide / 2, 0);
		QOffsetRect(&bounds, theObject.data.a.topLeft.h, 
				theObject.data.a.topLeft.v);
		if ((bounds.bottom - bounds.top) > kDeadlyFlameHeight)
		{
			bounds.bottom -= kDeadlyFlameHeight;
			hotSpotNumber = AddActiveRect(&bounds, kLiftIt, who, true, false);
			bounds.bottom += kDeadlyFlameHeight;
			bounds.top = bounds.bottom - kDeadlyFlameHeight + 2;
			hotSpotNumber = AddActiveRect(&bounds, kBurnIt, who, true, false);
		}
		else
			hotSpotNumber = AddActiveRect(&bounds, kBurnIt, who, true, false);
		QSetRect(&bounds, 0, 0, 52, 17);
		QOffsetRect(&bounds, 
				theObject.data.a.topLeft.h + 6, 
				theObject.data.a.topLeft.v + 8);
		hotSpotNumber = AddActiveRect(&bounds, kDissolveIt, who, true, true);
		break;
		
		case kInvisBlower:
		switch (theObject.data.a.vector & 0x0F)
		{
			case 1:		// up
			QSetRect(&bounds, 0, -theObject.data.a.distance - 24, 
					kFloorColumnWide, 0);
			QOffsetRect(&bounds, 12 - kFloorColumnWide / 2, 24);
			QOffsetRect(&bounds, theObject.data.a.topLeft.h, 
					theObject.data.a.topLeft.v);
			hotSpotNumber = AddActiveRect(&bounds, kLiftIt, who, theObject.data.a.state, 
					false);
			break;
			
			case 2:		// right
			QSetRect(&bounds, 0, 0, theObject.data.a.distance + 24, 
					kFanColumnThick);
			QOffsetRect(&bounds, 0, 12 - kFanColumnThick / 2);
			QOffsetRect(&bounds, theObject.data.a.topLeft.h, 
					theObject.data.a.topLeft.v);
			hotSpotNumber = AddActiveRect(&bounds, kPushItRight, who, 
					theObject.data.a.state, false);
			break;
			
			case 4:		// down
			QSetRect(&bounds, 0, 0, kFloorColumnWide, 
					theObject.data.a.distance + 24);
			QOffsetRect(&bounds, 12 - kFloorColumnWide / 2, 0);
			QOffsetRect(&bounds, theObject.data.a.topLeft.h, 
					theObject.data.a.topLeft.v);
			hotSpotNumber = AddActiveRect(&bounds, kDropIt, who, 
					theObject.data.a.state, false);
			break;
			
			case 8:		// left
			QSetRect(&bounds, 0, 0, theObject.data.a.distance + 24, kFanColumnThick);
			QOffsetRect(&bounds, -(theObject.data.a.distance), 12 - kFanColumnThick / 2);
			QOffsetRect(&bounds, theObject.data.a.topLeft.h, 
					theObject.data.a.topLeft.v);
			hotSpotNumber = AddActiveRect(&bounds, kPushItLeft, who, 
					theObject.data.a.state, false);
			break;
		}
		break;
		
		case kGrecoVent:
		QSetRect(&bounds, 0, -theObject.data.a.distance, kFloorColumnWide, 0);
		QOffsetRect(&bounds, 
				HalfRectWide(&srcRects[kGrecoVent]) - kFloorColumnWide / 2, 
				0);
		QOffsetRect(&bounds, 
				theObject.data.a.topLeft.h, 
				theObject.data.a.topLeft.v);
		hotSpotNumber = AddActiveRect(&bounds, kLiftIt, who, 
				theObject.data.a.state, false);
		break;
		
		case kSewerBlower:
		QSetRect(&bounds, 0, -theObject.data.a.distance, kFloorColumnWide, 0);
		QOffsetRect(&bounds, 
				HalfRectWide(&srcRects[kSewerBlower]) - kFloorColumnWide / 2, 
				0);
		QOffsetRect(&bounds, 
				theObject.data.a.topLeft.h, 
				theObject.data.a.topLeft.v);
		hotSpotNumber = AddActiveRect(&bounds, kLiftIt, who, 
				theObject.data.a.state, false);
		break;
		
		case kLiftArea:
		QSetRect(&bounds, 0, 0, theObject.data.a.distance, theObject.data.a.tall * 2);
		QOffsetRect(&bounds, 
				theObject.data.a.topLeft.h, 
				theObject.data.a.topLeft.v);
		switch (theObject.data.a.vector & 0x0F)
		{
			case 1:		// up
			hotSpotNumber = AddActiveRect(&bounds, kLiftIt, who, theObject.data.a.state, 
					false);
			break;
			
			case 2:		// right
			hotSpotNumber = AddActiveRect(&bounds, kPushItRight, who, 
					theObject.data.a.state, false);
			break;
			
			case 4:		// down
			hotSpotNumber = AddActiveRect(&bounds, kDropIt, who, 
					theObject.data.a.state, false);
			break;
			
			case 8:		// left
			hotSpotNumber = AddActiveRect(&bounds, kPushItLeft, who, 
					theObject.data.a.state, false);
			break;
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
		case kTrunk:
		case kInvisObstacle:
		bounds = theObject.data.b.bounds;
		hotSpotNumber = AddActiveRect(&bounds, kDissolveIt, who, true, true);
		break;
		
		case kBooks:
		bounds = theObject.data.b.bounds;
		bounds.right -= 2;
		hotSpotNumber = AddActiveRect(&bounds, kDissolveIt, who, true, true);
		break;
		
		case kManhole:
		bounds = theObject.data.b.bounds;
		bounds.left += kGliderWide + 3;
		bounds.right -= kGliderWide + 3;
		bounds.top = kFloorLimit - 1;
		bounds.bottom = kTileHigh;
		hotSpotNumber = AddActiveRect(&bounds, kIgnoreGround, who, true, false);
		break;
		
		case kInvisBounce:
		bounds = theObject.data.b.bounds;
		hotSpotNumber = AddActiveRect(&bounds, kBounceIt, who, true, true);
		break;
		
		case kStool:
		bounds = theObject.data.b.bounds;
		InsetRect(&bounds, 1, 1);
		bounds.bottom = bounds.top + kStoolThick;
		hotSpotNumber = AddActiveRect(&bounds, kDissolveIt, who, true, true);
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
		case kHelium:
		bounds = srcRects[theObject.what];
		ZeroRectCorner(&bounds);
		QOffsetRect(&bounds, 
				theObject.data.c.topLeft.h, 
				theObject.data.c.topLeft.v);
		hotSpotNumber = AddActiveRect(&bounds, kRewardIt, who, 
				theObject.data.c.state, false);
		break;
		
		case kGreaseRt:
		if (theObject.data.c.state)
		{
			bounds = srcRects[theObject.what];
			ZeroRectCorner(&bounds);
			QOffsetRect(&bounds, theObject.data.c.topLeft.h, 
					theObject.data.c.topLeft.v);
			hotSpotNumber = AddActiveRect(&bounds, kRewardIt, who, true, false);
		}
		else
		{
			QSetRect(&bounds, 0, -2, theObject.data.c.length - 5, 0);
			QOffsetRect(&bounds, 32 - 1, 27);
			QOffsetRect(&bounds, theObject.data.c.topLeft.h, 
					theObject.data.c.topLeft.v);
			hotSpotNumber = AddActiveRect(&bounds, kSlideIt, who, true, false);
		}
		break;
		
		case kGreaseLf:
		if (theObject.data.c.state)
		{
			bounds = srcRects[theObject.what];
			ZeroRectCorner(&bounds);
			QOffsetRect(&bounds, theObject.data.c.topLeft.h, 
					theObject.data.c.topLeft.v);
			hotSpotNumber = AddActiveRect(&bounds, kRewardIt, who, true, false);
		}
		else
		{
			QSetRect(&bounds, -theObject.data.c.length + 5, -2, 0, 0);
			QOffsetRect(&bounds, 1, 27);
			QOffsetRect(&bounds, theObject.data.c.topLeft.h, 
					theObject.data.c.topLeft.v);
			hotSpotNumber = AddActiveRect(&bounds, kSlideIt, who, true, false);
		}
		break;
		
		case kSparkle:
		bounds = srcRects[theObject.what];
		ZeroRectCorner(&bounds);
		QOffsetRect(&bounds, 
				theObject.data.c.topLeft.h, 
				theObject.data.c.topLeft.v);
		break;
		
		case kSlider:
		QSetRect(&bounds, 0, 0, theObject.data.c.length, 16);
		QOffsetRect(&bounds, theObject.data.c.topLeft.h, 
				theObject.data.c.topLeft.v);
		hotSpotNumber = AddActiveRect(&bounds, kSlideIt, who, true, false);
		break;
		
		case kUpStairs:
		QSetRect(&bounds, 0, 0, 112, 32);
		QOffsetRect(&bounds, 
				theObject.data.d.topLeft.h, 
				theObject.data.d.topLeft.v);
		hotSpotNumber = AddActiveRect(&bounds, kMoveItUp, who, true, false);
		break;
		
		case kDownStairs:
		QSetRect(&bounds, -80, -56, 0, 0);
		QOffsetRect(&bounds, srcRects[kDownStairs].right, 170);
		QOffsetRect(&bounds, 
				theObject.data.d.topLeft.h, 
				theObject.data.d.topLeft.v);
		hotSpotNumber = AddActiveRect(&bounds, kMoveItDown, who, true, false);
		break;
		
		case kMailboxLf:
		if (theObject.data.d.who != 255)
		{
			QSetRect(&bounds, -72, 0, 0, 40);
			QOffsetRect(&bounds, 30, 16);
			QOffsetRect(&bounds, 
					theObject.data.d.topLeft.h, 
					theObject.data.d.topLeft.v);
			hotSpotNumber = AddActiveRect(&bounds, kMailItLeft, who, true, false);
		}
		break;
		
		case kMailboxRt:
		if (theObject.data.d.who != 255)
		{
			QSetRect(&bounds, 0, 0, 72, 40);
			QOffsetRect(&bounds, 79, 16);
			QOffsetRect(&bounds, 
					theObject.data.d.topLeft.h, 
					theObject.data.d.topLeft.v);
			hotSpotNumber = AddActiveRect(&bounds, kMailItRight, who, true, false);
		}
		break;
		
		case kFloorTrans:
		if (theObject.data.d.who != 255)
		{
			QSetRect(&bounds, 0, -48, 76, 0);
			QOffsetRect(&bounds, -8, RectTall(&srcRects[kFloorTrans]));
			QOffsetRect(&bounds, 
					theObject.data.d.topLeft.h, 
					theObject.data.d.topLeft.v);
			hotSpotNumber = AddActiveRect(&bounds, kDuctItDown, who, true, false);
		}
		break;
		
		case kCeilingTrans:
		if (theObject.data.d.who != 255)
		{
			QSetRect(&bounds, 0, 0, 76, 48);
			QOffsetRect(&bounds, -8, 0);
			QOffsetRect(&bounds, 
					theObject.data.d.topLeft.h, 
					theObject.data.d.topLeft.v);
			hotSpotNumber = AddActiveRect(&bounds, kDuctItUp, who, true, false);
		}
		break;
		
		case kDoorInLf:
		QSetRect(&bounds, 0, 0, 16, 240);
		QOffsetRect(&bounds, 0, 52);
		QOffsetRect(&bounds, 
				theObject.data.d.topLeft.h, 
				theObject.data.d.topLeft.v);
		hotSpotNumber = AddActiveRect(&bounds, kIgnoreLeftWall, who, true, false);
		break;
		
		case kDoorInRt:
		QSetRect(&bounds, 0, 0, 16, 240);
		QOffsetRect(&bounds, 128, 52);
		QOffsetRect(&bounds, 
				theObject.data.d.topLeft.h, 
				theObject.data.d.topLeft.v);
		hotSpotNumber = AddActiveRect(&bounds, kIgnoreRightWall, who, true, false);
		break;
		
		case kDoorExRt:
		QSetRect(&bounds, 0, 0, 16, 240);
		QOffsetRect(&bounds, 0, 52);
		QOffsetRect(&bounds, 
				theObject.data.d.topLeft.h, 
				theObject.data.d.topLeft.v);
		hotSpotNumber = AddActiveRect(&bounds, kIgnoreRightWall, who, true, false);
		break;
		
		case kDoorExLf:
		QSetRect(&bounds, 0, 0, 16, 240);
		QOffsetRect(&bounds, 0, 52);
		QOffsetRect(&bounds, 
				theObject.data.d.topLeft.h, 
				theObject.data.d.topLeft.v);
		hotSpotNumber = AddActiveRect(&bounds, kIgnoreLeftWall, who, true, false);
		break;
		
		case kWindowInLf:
		QSetRect(&bounds, 0, 0, 16, 44);
		QOffsetRect(&bounds, 0, 96);
		QOffsetRect(&bounds, 
				theObject.data.d.topLeft.h, 
				theObject.data.d.topLeft.v);
		hotSpotNumber = AddActiveRect(&bounds, kIgnoreLeftWall, who, true, false);
		break;
		
		case kWindowInRt:
		QSetRect(&bounds, 0, 0, 16, 44);
		QOffsetRect(&bounds, 4, 96);
		QOffsetRect(&bounds, 
				theObject.data.d.topLeft.h, 
				theObject.data.d.topLeft.v);
		hotSpotNumber = AddActiveRect(&bounds, kIgnoreRightWall, who, true, false);
		break;
		
		case kWindowExRt:
		QSetRect(&bounds, 0, 0, 16, 44);
		QOffsetRect(&bounds, 0, 96);
		QOffsetRect(&bounds, 
				theObject.data.d.topLeft.h, 
				theObject.data.d.topLeft.v);
		hotSpotNumber = AddActiveRect(&bounds, kIgnoreRightWall, who, true, false);
		break;
		
		case kWindowExLf:
		QSetRect(&bounds, 0, 0, 16, 44);
		QOffsetRect(&bounds, 0, 96);
		QOffsetRect(&bounds, 
				theObject.data.d.topLeft.h, 
				theObject.data.d.topLeft.v);
		hotSpotNumber = AddActiveRect(&bounds, kIgnoreLeftWall, who, true, false);
		break;
		
		case kInvisTrans:
		if (theObject.data.d.who != 255)
		{
			QSetRect(&bounds, 0, 0, 64, 32);
			QOffsetRect(&bounds, 
					theObject.data.d.topLeft.h, 
					theObject.data.d.topLeft.v);
			bounds.bottom = bounds.top + theObject.data.d.tall;
			bounds.right += (short)theObject.data.d.wide;
			hotSpotNumber = AddActiveRect(&bounds, kTransportIt, who, true, false);
		}
		break;
		
		case kDeluxeTrans:
		if (theObject.data.d.who != 255)
		{
			wide = (theObject.data.d.tall & 0xFF00) >> 8;	// Get high byte
			tall = theObject.data.d.tall & 0x00FF;			// Get low byte
			QSetRect(&bounds, 0, 0, wide * 4, tall * 4);	// Scale by 4
			QOffsetRect(&bounds, 
					theObject.data.d.topLeft.h, 
					theObject.data.d.topLeft.v);
			isOn = theObject.data.d.wide & 0x0F;
			hotSpotNumber = AddActiveRect(&bounds, kTransportIt, who, isOn, false);
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
		bounds = srcRects[theObject.what];
		ZeroRectCorner(&bounds);
		QOffsetRect(&bounds, 
				theObject.data.e.topLeft.h, 
				theObject.data.e.topLeft.v);
		if ((theObject.what == kTrigger) || (theObject.what == kLgTrigger))
		{
			if (theObject.data.e.where != -1)
				hotSpotNumber = AddActiveRect(&bounds, kTriggerIt, who, true, false);
		}
		else
		{
			if (theObject.data.e.where != -1)
				hotSpotNumber = AddActiveRect(&bounds, kSwitchIt, who, true, false);
		}
		break;
		
		case kSoundTrigger:
		QSetRect(&bounds, 0, 0, 48, 48);
		QOffsetRect(&bounds, theObject.data.e.topLeft.h, theObject.data.e.topLeft.v);
		if (LoadTriggerSound(theObject.data.e.where) == noErr)
			hotSpotNumber = AddActiveRect(&bounds, kSoundIt, who, true, false);
		break;
		
		case kCeilingLight:
		case kLightBulb:
		case kTableLamp:
		case kHipLamp:
		case kDecoLamp:
		case kFlourescent:
		case kTrackLight:
		case kInvisLight:
		break;
		
		case kShredder:
		bounds = srcRects[theObject.what];
		bounds.bottom = bounds.top + kShredderActiveHigh;
		bounds.right += 48;
		ZeroRectCorner(&bounds);
		QOffsetRect(&bounds, theObject.data.g.topLeft.h, 
				theObject.data.g.topLeft.v);
		QOffsetRect(&bounds, -24, -36);
		hotSpotNumber = AddActiveRect(&bounds, kShredIt, who, 
				theObject.data.g.state, true);
		break;
		
		case kGuitar:
		QSetRect(&bounds, 0, 0, 8, 96);
		QOffsetRect(&bounds, theObject.data.g.topLeft.h + 34, 
				theObject.data.g.topLeft.v + 32);
		hotSpotNumber = AddActiveRect(&bounds, kStrumIt, who, true, false);
		break;
		
		case kOutlet:
		bounds = srcRects[theObject.what];
		ZeroRectCorner(&bounds);
		QOffsetRect(&bounds, 
				theObject.data.g.topLeft.h, 
				theObject.data.g.topLeft.v);
		hotSpotNumber = AddActiveRect(&bounds, kIgnoreIt, who, 
				theObject.data.g.state, false);
		break;
		
		case kMicrowave:
		bounds = srcRects[theObject.what];
		ZeroRectCorner(&bounds);
		QOffsetRect(&bounds, 
				theObject.data.g.topLeft.h, 
				theObject.data.g.topLeft.v);
		hotSpotNumber = AddActiveRect(&bounds, kDissolveIt, who, true, true);
		bounds.bottom = bounds.top;
		bounds.top = 0;
		hotSpotNumber = AddActiveRect(&bounds, kMicrowaveIt, who, true, true);
		break;
		
		case kToaster:
		case kMacPlus:
		case kTV:
		case kCoffee:
		case kVCR:
		case kStereo:
		case kCinderBlock:
		case kFlowerBox:
		case kCDs:
		bounds = srcRects[theObject.what];
		ZeroRectCorner(&bounds);
		QOffsetRect(&bounds, 
				theObject.data.g.topLeft.h, 
				theObject.data.g.topLeft.v);
		hotSpotNumber = AddActiveRect(&bounds, kDissolveIt, who, true, true);
		break;
		
		case kCustomPict:
		break;
		
		case kBalloon:
		case kCopterLf:
		case kCopterRt:
		case kDartLf:
		case kDartRt:
		case kBall:
		case kDrip:
		bounds = srcRects[theObject.what];
		ZeroRectCorner(&bounds);
		QOffsetRect(&bounds, 
				theObject.data.h.topLeft.h, 
				theObject.data.h.topLeft.v);
		hotSpotNumber = AddActiveRect(&bounds, kIgnoreIt, who, true, false);
		break;
		
		case kFish:
		bounds = srcRects[theObject.what];
		ZeroRectCorner(&bounds);
		QOffsetRect(&bounds, 
				theObject.data.h.topLeft.h, 
				theObject.data.h.topLeft.v);
		hotSpotNumber = AddActiveRect(&bounds, kDissolveIt, who, true, true);
		break;
		
		case kCobweb:
		bounds = srcRects[theObject.what];
		ZeroRectCorner(&bounds);
		QOffsetRect(&bounds, 
				theObject.data.h.topLeft.h, 
				theObject.data.h.topLeft.v);
		InsetRect(&bounds, -24, -10);
		hotSpotNumber = AddActiveRect(&bounds, kWebIt, who, true, true);
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
		break;
		
		case kChimes:
		numChimes++;
		bounds = srcRects[kChimes];
		ZeroRectCorner(&bounds);
		QOffsetRect(&bounds, 
				theObject.data.i.bounds.left, 
				theObject.data.i.bounds.top);
		hotSpotNumber = AddActiveRect(&bounds, kChimeIt, who, true, false);
		break;
	}
	
	return (hotSpotNumber);
}

//--------------------------------------------------------------  VerticalRoomOffset

short VerticalRoomOffset (short neighbor)
{
	short		offset;
	
	offset = 0;
	
	switch (neighbor)
	{
		case kNorthRoom:
		case kNorthEastRoom:
		case kNorthWestRoom:
		offset -= kVertLocalOffset;
		break;
		
		case kSouthEastRoom:
		case kSouthRoom:
		case kSouthWestRoom:
		offset += kVertLocalOffset;
		break;
	}
	
	return (offset);
}

//--------------------------------------------------------------  OffsetRectRoomRelative

void OffsetRectRoomRelative (Rect *theRect, short neighbor)
{
	QOffsetRect(theRect, playOriginH, playOriginV);
	
	switch (neighbor)
	{
		case kNorthRoom:
		QOffsetRect(theRect, 0, -kVertLocalOffset);
		break;
		
		case kNorthEastRoom:
		QOffsetRect(theRect, kRoomWide, -kVertLocalOffset);
		break;
		
		case kEastRoom:
		QOffsetRect(theRect, kRoomWide, 0);
		break;
		
		case kSouthEastRoom:
		QOffsetRect(theRect, kRoomWide, kVertLocalOffset);
		break;
		
		case kSouthRoom:
		QOffsetRect(theRect, 0, kVertLocalOffset);
		break;
		
		case kSouthWestRoom:
		QOffsetRect(theRect, -kRoomWide, kVertLocalOffset);
		break;
		
		case kWestRoom:
		QOffsetRect(theRect, -kRoomWide, 0);
		break;
		
		case kNorthWestRoom:
		QOffsetRect(theRect, -kRoomWide, -kVertLocalOffset);
		break;
	}
}

//--------------------------------------------------------------  GetUpStairsRightEdge

short GetUpStairsRightEdge (void)
{
	objectType	thisObject;
	short		i, rightEdge;
	char		wasState;
	
	rightEdge = kRoomWide;
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	
	for (i = 0; i < kMaxRoomObs; i++)
	{
		thisObject = (*thisHouse)->rooms[thisRoomNumber].objects[i];
		if (thisObject.what == kDownStairs)
		{
			rightEdge = thisObject.data.d.topLeft.h + srcRects[kDownStairs].right - 1;
			break;
		}
	}
	
	HSetState((Handle)thisHouse, wasState);
	
	return (rightEdge);
}

//--------------------------------------------------------------  GetDownStairsLeftEdge

short GetDownStairsLeftEdge (void)
{
	objectType	thisObject;
	short		i, leftEdge;
	char		wasState;
	
	leftEdge = 0;
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	
	for (i = 0; i < kMaxRoomObs; i++)
	{
		thisObject = (*thisHouse)->rooms[thisRoomNumber].objects[i];
		if (thisObject.what == kUpStairs)
		{
			leftEdge = thisObject.data.d.topLeft.h + 1;
			break;
		}
	}
	
	HSetState((Handle)thisHouse, wasState);
	
	return (leftEdge);
}

