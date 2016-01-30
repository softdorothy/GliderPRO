
//============================================================================
//----------------------------------------------------------------------------
//								   ObjectAdd.c
//----------------------------------------------------------------------------
//============================================================================


#include <ToolUtils.h>
#include "Externs.h"
#include "ObjectEdit.h"
#include "RectUtils.h"


#define kNoMoreObjectsAlert		1008
#define kNoMoreSpecialAlert		1028
#define kMaxSoundTriggers		1
#define kMaxStairs				1
#define kMouseholeBottom		295
#define kFireplaceBottom		297
#define kManholeSits			322
#define kGrecoVentTop			303
#define kSewerBlowerTop			292


short FindEmptyObjectSlot (void);
short HowManyCandleObjects (void);
short HowManyTikiObjects (void);
short HowManyBBQObjects (void);
short HowManyCuckooObjects (void);
short HowManyBandsObjects (void);
short HowManyGreaseObjects (void);
short HowManyStarsObjects (void);
short HowManySoundObjects (void);
short HowManyUpStairsObjects (void);
short HowManyDownStairsObjects (void);
short HowManyShredderObjects (void);
short HowManyDynamicObjects (void);
void ShoutNoMoreSpecialObjects (void);


short		wasFlower;


//==============================================================  Functions
//--------------------------------------------------------------  AddNewObject

Boolean AddNewObject (Point where, short what, Boolean showItNow)
{
	KeyMap		theseKeys;
	Rect		srcRect, newRect;
	short		direction, dist;
	Boolean		handled, drawWholeRoom;
	
#ifndef COMPILEDEMO
	
	objActive = FindEmptyObjectSlot();
	if (objActive == -1)
	{
		ShoutNoMoreObjects();
		return (false);
	}
	
	drawWholeRoom = false;
	
	switch (what)
	{
		case kFloorVent:
		case kFloorBlower:
		case kSewerGrate:
		case kTaper:
		case kCandle:
		case kStubby:
		case kTiki:
		case kBBQ:
		case kInvisBlower:
		case kGrecoVent:
		case kSewerBlower:
		case kLiftArea:
		if (((what == kTaper) || (what == kCandle) || (what == kStubby)) && 
				(HowManyCandleObjects() >= kMaxCandles))
		{
			ShoutNoMoreSpecialObjects();
			return (false);
		}
		else if ((what == kTiki) && (HowManyTikiObjects() >= kMaxTikis))
		{
			ShoutNoMoreSpecialObjects();
			return (false);
		}
		else if ((what == kBBQ) && (HowManyBBQObjects() >= kMaxCoals))
		{
			ShoutNoMoreSpecialObjects();
			return (false);
		}
		srcRect = srcRects[what];
		thisRoom->objects[objActive].data.a.topLeft.h = where.h - HalfRectWide(&srcRect);
		QSetRect(&newRect, 0, 0, RectWide(&srcRect), RectTall(&srcRect));
		if (what == kFloorVent)
			thisRoom->objects[objActive].data.a.topLeft.v = kFloorVentTop;
		else if (what == kFloorBlower)
			thisRoom->objects[objActive].data.a.topLeft.v = kFloorBlowerTop;
		else if ((what == kTaper) || (what == kCandle) || (what == kStubby) || 
				(what == kTiki) || (what == kBBQ) || (what == kInvisBlower) || 
				(what == kLiftArea))
			thisRoom->objects[objActive].data.a.topLeft.v = where.v - HalfRectTall(&srcRect);
		else if (what == kGrecoVent)
			thisRoom->objects[objActive].data.a.topLeft.v = kGrecoVentTop;
		else if (what == kSewerBlower)
			thisRoom->objects[objActive].data.a.topLeft.v = kSewerBlowerTop;
		QOffsetRect(&newRect, thisRoom->objects[objActive].data.a.topLeft.h, 
				thisRoom->objects[objActive].data.a.topLeft.v);
		thisRoom->objects[objActive].data.a.distance = 64;
		thisRoom->objects[objActive].data.a.initial = true;
		thisRoom->objects[objActive].data.a.state = true;
		thisRoom->objects[objActive].data.a.vector = 0x01;
		if (what == kLiftArea)
			thisRoom->objects[objActive].data.a.tall = 0x10;
		else
			thisRoom->objects[objActive].data.a.tall = 0x00;
		break;
		
		case kCeilingVent:
		case kCeilingBlower:
		srcRect = srcRects[what];
		thisRoom->objects[objActive].data.a.topLeft.h = where.h - HalfRectWide(&srcRect);
		QSetRect(&newRect, 0, 0, RectWide(&srcRect), RectTall(&srcRect));
		if (what == kCeilingVent)
			thisRoom->objects[objActive].data.a.topLeft.v = kCeilingVentTop;
		else if (what == kCeilingBlower)
			thisRoom->objects[objActive].data.a.topLeft.v = kCeilingBlowerTop;
		QOffsetRect(&newRect, thisRoom->objects[objActive].data.a.topLeft.h, 
				thisRoom->objects[objActive].data.a.topLeft.v);
		thisRoom->objects[objActive].data.a.distance = 32;
		thisRoom->objects[objActive].data.a.initial = true;
		thisRoom->objects[objActive].data.a.state = true;
		thisRoom->objects[objActive].data.a.vector = 0x04;
		break;
		
		case kLeftFan:
		thisRoom->objects[objActive].data.a.topLeft.h = 
				where.h - HalfRectWide(&srcRects[kLeftFan]);
		thisRoom->objects[objActive].data.a.topLeft.v = 
				where.v - HalfRectTall(&srcRects[kLeftFan]);
		QSetRect(&newRect, 0, 0, RectWide(&srcRects[kLeftFan]), 
				RectTall(&srcRects[kLeftFan]));
		QOffsetRect(&newRect, thisRoom->objects[objActive].data.a.topLeft.h, 
				thisRoom->objects[objActive].data.a.topLeft.v);
		thisRoom->objects[objActive].data.a.distance = 32;
		thisRoom->objects[objActive].data.a.initial = true;
		thisRoom->objects[objActive].data.a.state = true;
		thisRoom->objects[objActive].data.a.vector = 0x08;
		break;
		
		case kRightFan:
		thisRoom->objects[objActive].data.a.topLeft.h = 
				where.h - HalfRectWide(&srcRects[kRightFan]);
		thisRoom->objects[objActive].data.a.topLeft.v = 
				where.v - HalfRectTall(&srcRects[kRightFan]);
		QSetRect(&newRect, 0, 0, RectWide(&srcRects[kRightFan]), RectTall(&srcRects[kRightFan]));
		QOffsetRect(&newRect, thisRoom->objects[objActive].data.a.topLeft.h, 
				thisRoom->objects[objActive].data.a.topLeft.v);
		thisRoom->objects[objActive].data.a.distance = 32;
		thisRoom->objects[objActive].data.a.initial = true;
		thisRoom->objects[objActive].data.a.state = true;
		thisRoom->objects[objActive].data.a.vector = 0x02;
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
		case kBooks:
		case kInvisBounce:
		newRect = srcRects[what];
		CenterRectOnPoint(&newRect, where);
		if (what == kCounter)
			newRect.bottom = kCounterBottom;
		else if (what == kDresser)
			newRect.bottom = kDresserBottom;
		thisRoom->objects[objActive].data.b.bounds = newRect;
		thisRoom->objects[objActive].data.b.pict = 0;
		break;
		
		case kManhole:
		newRect = srcRects[kManhole];
		CenterRectOnPoint(&newRect, where);
		newRect.left = (((newRect.left - 3) / 64) * 64) + 3;
		newRect.right = newRect.left + RectWide(&srcRects[kManhole]);
		newRect.bottom = kManholeSits;
		newRect.top = newRect.bottom - RectTall(&srcRects[kManhole]);
		thisRoom->objects[objActive].data.b.bounds = newRect;
		thisRoom->objects[objActive].data.b.pict = 0;
		break;
		
		case kRedClock:
		case kBlueClock:
		case kYellowClock:
		case kCuckoo:
		case kPaper:
		case kBattery:
		case kBands:
		case kFoil:
		case kStar:
		case kSparkle:
		case kHelium:
		if ((what == kCuckoo) && (HowManyCuckooObjects() >= kMaxPendulums))
		{
			ShoutNoMoreSpecialObjects();
			return (false);
		}
		else if ((what == kBands) && (HowManyBandsObjects() >= kMaxRubberBands))
		{
			ShoutNoMoreSpecialObjects();
			return (false);
		}
		else if ((what == kStar) && (HowManyStarsObjects() >= kMaxStars))
		{
			ShoutNoMoreSpecialObjects();
			return (false);
		}
		else if ((what == kSparkle) && (HowManyDynamicObjects() >= kMaxDynamicObs))
		{
			ShoutNoMoreSpecialObjects();
			return (false);
		}
		thisRoom->objects[objActive].data.c.topLeft.h = 
				where.h - HalfRectWide(&srcRects[what]);
		thisRoom->objects[objActive].data.c.topLeft.v = 
				where.v - HalfRectTall(&srcRects[what]);
		QSetRect(&newRect, 0, 0, RectWide(&srcRects[what]), 
				RectTall(&srcRects[what]));
		QOffsetRect(&newRect, thisRoom->objects[objActive].data.c.topLeft.h, 
				thisRoom->objects[objActive].data.c.topLeft.v);
		thisRoom->objects[objActive].data.c.length = 0;
		thisRoom->objects[objActive].data.c.points = 0;
		thisRoom->objects[objActive].data.c.state = true;
		thisRoom->objects[objActive].data.c.initial = true;
		break;
		
		case kGreaseRt:
		case kGreaseLf:
		if (HowManyGreaseObjects() >= kMaxGrease)
		{
			ShoutNoMoreSpecialObjects();
			return (false);
		}
		thisRoom->objects[objActive].data.c.topLeft.h = 
				where.h - HalfRectWide(&srcRects[what]);
		thisRoom->objects[objActive].data.c.topLeft.v = 
				where.v - HalfRectTall(&srcRects[what]);
		QSetRect(&newRect, 0, 0, RectWide(&srcRects[what]), RectTall(&srcRects[what]));
		QOffsetRect(&newRect, thisRoom->objects[objActive].data.c.topLeft.h, 
				thisRoom->objects[objActive].data.c.topLeft.v);
		thisRoom->objects[objActive].data.c.length = 64;
		thisRoom->objects[objActive].data.c.points = 0;
		thisRoom->objects[objActive].data.c.state = true;
		thisRoom->objects[objActive].data.c.initial = true;
		break;
		
		case kInvisBonus:
		thisRoom->objects[objActive].data.c.topLeft.h = 
				where.h - HalfRectWide(&srcRects[what]);
		thisRoom->objects[objActive].data.c.topLeft.v = 
				where.v - HalfRectTall(&srcRects[what]);
		QSetRect(&newRect, 0, 0, RectWide(&srcRects[what]), RectTall(&srcRects[what]));
		QOffsetRect(&newRect, thisRoom->objects[objActive].data.c.topLeft.h, 
				thisRoom->objects[objActive].data.c.topLeft.v);
		thisRoom->objects[objActive].data.c.length = 0;
		thisRoom->objects[objActive].data.c.points = 100;
		thisRoom->objects[objActive].data.c.state = true;
		thisRoom->objects[objActive].data.c.initial = true;
		break;
		
		case kSlider:
		thisRoom->objects[objActive].data.c.topLeft.h = 
				where.h - HalfRectWide(&srcRects[what]);
		thisRoom->objects[objActive].data.c.topLeft.v = 
				where.v - HalfRectTall(&srcRects[what]);
		QSetRect(&newRect, 0, 0, RectWide(&srcRects[what]), RectTall(&srcRects[what]));
		QOffsetRect(&newRect, thisRoom->objects[objActive].data.c.topLeft.h, 
				thisRoom->objects[objActive].data.c.topLeft.v);
		thisRoom->objects[objActive].data.c.length = 64;
		thisRoom->objects[objActive].data.c.points = 0;
		thisRoom->objects[objActive].data.c.state = true;
		thisRoom->objects[objActive].data.c.initial = true;
		break;
		
		case kUpStairs:
		case kDownStairs:
		if ((what == kUpStairs) && (HowManyUpStairsObjects() >= kMaxStairs))
		{
			ShoutNoMoreSpecialObjects();
			return (false);
		}
		else if ((what == kDownStairs) && (HowManyDownStairsObjects() >= kMaxStairs))
		{
			ShoutNoMoreSpecialObjects();
			return (false);
		}
		thisRoom->objects[objActive].data.d.topLeft.h = 
				where.h - HalfRectWide(&srcRects[what]);
		thisRoom->objects[objActive].data.d.topLeft.v = kStairsTop;
		QSetRect(&newRect, 0, 0, RectWide(&srcRects[what]), 
				RectTall(&srcRects[what]));
		QOffsetRect(&newRect, thisRoom->objects[objActive].data.d.topLeft.h, 
				thisRoom->objects[objActive].data.d.topLeft.v);
		thisRoom->objects[objActive].data.d.tall = 0;
		thisRoom->objects[objActive].data.d.where = -1;
		thisRoom->objects[objActive].data.d.who = 255;
		thisRoom->objects[objActive].data.d.wide = 0;
		break;
		
		case kMailboxLf:
		case kMailboxRt:
		thisRoom->objects[objActive].data.d.topLeft.h = 
				where.h - HalfRectWide(&srcRects[what]);
		thisRoom->objects[objActive].data.d.topLeft.v = 
				where.v - HalfRectTall(&srcRects[what]);
		QSetRect(&newRect, 0, 0, RectWide(&srcRects[what]), 
				RectTall(&srcRects[what]));
		QOffsetRect(&newRect, thisRoom->objects[objActive].data.d.topLeft.h, 
				thisRoom->objects[objActive].data.d.topLeft.v);
		thisRoom->objects[objActive].data.d.tall = 0;
		thisRoom->objects[objActive].data.d.where = -1;
		thisRoom->objects[objActive].data.d.who = 255;
		thisRoom->objects[objActive].data.d.wide = 0;
		break;
		
		case kFloorTrans:
		thisRoom->objects[objActive].data.d.topLeft.h = 
				where.h - HalfRectWide(&srcRects[what]);
		thisRoom->objects[objActive].data.d.topLeft.v = kFloorTransTop;
		QSetRect(&newRect, 0, 0, RectWide(&srcRects[what]), 
				RectTall(&srcRects[what]));
		QOffsetRect(&newRect, thisRoom->objects[objActive].data.d.topLeft.h, 
				thisRoom->objects[objActive].data.d.topLeft.v);
		thisRoom->objects[objActive].data.d.tall = 0;
		thisRoom->objects[objActive].data.d.where = -1;
		thisRoom->objects[objActive].data.d.who = 255;
		thisRoom->objects[objActive].data.d.wide = 0;
		break;
		
		case kCeilingTrans:
		thisRoom->objects[objActive].data.d.topLeft.h = 
				where.h - HalfRectWide(&srcRects[what]);
		thisRoom->objects[objActive].data.d.topLeft.v = kCeilingTransTop;
		QSetRect(&newRect, 0, 0, RectWide(&srcRects[what]), 
				RectTall(&srcRects[what]));
		QOffsetRect(&newRect, thisRoom->objects[objActive].data.d.topLeft.h, 
				thisRoom->objects[objActive].data.d.topLeft.v);
		thisRoom->objects[objActive].data.d.tall = 0;
		thisRoom->objects[objActive].data.d.where = -1;
		thisRoom->objects[objActive].data.d.who = 255;
		thisRoom->objects[objActive].data.d.wide = 0;
		break;
		
		case kDoorInLf:
		case kDoorInRt:
		case kDoorExRt:
		case kDoorExLf:
		case kWindowInLf:
		case kWindowInRt:
		case kWindowExRt:
		case kWindowExLf:
		if ((what == kDoorInLf) || (what == kDoorInRt))
		{
			if (where.h > (kRoomWide / 2))
			{
				what = kDoorInRt;
				thisRoom->objects[objActive].what = kDoorInRt;
				thisRoom->objects[objActive].data.d.topLeft.h = kDoorInRtLeft;
				thisRoom->objects[objActive].data.d.topLeft.v = kDoorInTop;
			}
			else
			{
				what = kDoorInLf;
				thisRoom->objects[objActive].what = kDoorInLf;
				thisRoom->objects[objActive].data.d.topLeft.h = kDoorInLfLeft;
				thisRoom->objects[objActive].data.d.topLeft.v = kDoorInTop;
			}
		}
		else if ((what == kDoorExRt) || (what == kDoorExLf))
		{
			if (where.h > (kRoomWide / 2))
			{
				what = kDoorExRt;
				thisRoom->objects[objActive].what = kDoorExRt;
				thisRoom->objects[objActive].data.d.topLeft.h = kDoorExRtLeft;
				thisRoom->objects[objActive].data.d.topLeft.v = kDoorExTop;
			}
			else
			{
				what = kDoorExLf;
				thisRoom->objects[objActive].what = kDoorExLf;
				thisRoom->objects[objActive].data.d.topLeft.h = kDoorExLfLeft;
				thisRoom->objects[objActive].data.d.topLeft.v = kDoorExTop;
			}
		}
		else if ((what == kWindowInLf) || (what == kWindowInRt))
		{
			if (where.h > (kRoomWide / 2))
			{
				what = kWindowInRt;
				thisRoom->objects[objActive].what = kWindowInRt;
				thisRoom->objects[objActive].data.d.topLeft.h = kWindowInRtLeft;
				thisRoom->objects[objActive].data.d.topLeft.v = kWindowInTop;
			}
			else
			{
				what = kWindowInLf;
				thisRoom->objects[objActive].what = kWindowInLf;
				thisRoom->objects[objActive].data.d.topLeft.h = kWindowInLfLeft;
				thisRoom->objects[objActive].data.d.topLeft.v = kWindowInTop;
			}
		}
		else if ((what == kWindowExRt) || (what == kWindowExLf))
		{
			if (where.h > (kRoomWide / 2))
			{
				what = kWindowExRt;
				thisRoom->objects[objActive].what = kWindowExRt;
				thisRoom->objects[objActive].data.d.topLeft.h = kWindowExRtLeft;
				thisRoom->objects[objActive].data.d.topLeft.v = kWindowExTop;
			}
			else
			{
				what = kWindowExLf;
				thisRoom->objects[objActive].what = kWindowExLf;
				thisRoom->objects[objActive].data.d.topLeft.h = kWindowExLfLeft;
				thisRoom->objects[objActive].data.d.topLeft.v = kWindowExTop;
			}
		}
		QSetRect(&newRect, 0, 0, RectWide(&srcRects[what]), 
				RectTall(&srcRects[what]));
		QOffsetRect(&newRect, thisRoom->objects[objActive].data.d.topLeft.h, 
				thisRoom->objects[objActive].data.d.topLeft.v);
		thisRoom->objects[objActive].data.d.tall = 0;
		thisRoom->objects[objActive].data.d.where = -1;
		thisRoom->objects[objActive].data.d.who = 255;
		thisRoom->objects[objActive].data.d.wide = 0;
		drawWholeRoom = true;
		break;
		
		case kInvisTrans:
		newRect = srcRects[what];
		CenterRectOnPoint(&newRect, where);
		thisRoom->objects[objActive].data.d.topLeft.h = newRect.left;
		thisRoom->objects[objActive].data.d.topLeft.v = newRect.top;
		thisRoom->objects[objActive].data.d.tall = newRect.bottom - newRect.top;
		thisRoom->objects[objActive].data.d.where = -1;
		thisRoom->objects[objActive].data.d.who = 255;
		thisRoom->objects[objActive].data.d.wide = 0;
		break;
		
		case kDeluxeTrans:
		newRect = srcRects[what];
		CenterRectOnPoint(&newRect, where);
		thisRoom->objects[objActive].data.d.topLeft.h = newRect.left;
		thisRoom->objects[objActive].data.d.topLeft.v = newRect.top;
		thisRoom->objects[objActive].data.d.tall = 0x1010;			// 64 x 64
		thisRoom->objects[objActive].data.d.where = -1;
		thisRoom->objects[objActive].data.d.who = 255;
		thisRoom->objects[objActive].data.d.wide = 0x10;			// Initially on
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
		if ((what == kSoundTrigger) && (HowManySoundObjects() >= kMaxSoundTriggers))
		{
			ShoutNoMoreSpecialObjects();
			return (false);
		}
		thisRoom->objects[objActive].data.e.topLeft.h = 
				where.h - HalfRectWide(&srcRects[what]);
		thisRoom->objects[objActive].data.e.topLeft.v = 
				where.v - HalfRectTall(&srcRects[what]);
		QSetRect(&newRect, 0, 0, RectWide(&srcRects[what]), 
				RectTall(&srcRects[what]));
		QOffsetRect(&newRect, thisRoom->objects[objActive].data.e.topLeft.h, 
				thisRoom->objects[objActive].data.e.topLeft.v);
		thisRoom->objects[objActive].data.e.delay = 0;
		if (what == kSoundTrigger)
			thisRoom->objects[objActive].data.e.where = 3000;
		else
			thisRoom->objects[objActive].data.e.where = -1;
		thisRoom->objects[objActive].data.e.who = 255;
		if ((what == kTrigger) || (what == kLgTrigger))
			thisRoom->objects[objActive].data.e.type = kOneShot;
		else
			thisRoom->objects[objActive].data.e.type = kToggle;
		break;
		
		case kCeilingLight:
		case kLightBulb:
		case kTableLamp:
		case kHipLamp:
		case kDecoLamp:
		case kFlourescent:
		case kTrackLight:
		case kInvisLight:
		if (what == kCeilingLight)
		{
			thisRoom->objects[objActive].data.f.topLeft.h = 
					where.h - HalfRectWide(&srcRects[what]);
			thisRoom->objects[objActive].data.f.topLeft.v = kCeilingLightTop;
			QSetRect(&newRect, 0, 0, RectWide(&srcRects[what]), 
					RectTall(&srcRects[what]));
			QOffsetRect(&newRect, thisRoom->objects[objActive].data.f.topLeft.h, 
					thisRoom->objects[objActive].data.f.topLeft.v);
			thisRoom->objects[objActive].data.f.length = 64;
		}
		else if (what == kHipLamp)
		{
			thisRoom->objects[objActive].data.f.topLeft.h = 
					where.h - HalfRectWide(&srcRects[what]);
			thisRoom->objects[objActive].data.f.topLeft.v = kHipLampTop;
			QSetRect(&newRect, 0, 0, RectWide(&srcRects[what]), 
					RectTall(&srcRects[what]));
			QOffsetRect(&newRect, thisRoom->objects[objActive].data.f.topLeft.h, 
					thisRoom->objects[objActive].data.f.topLeft.v);
			thisRoom->objects[objActive].data.f.length = 0;
		}
		else if (what == kDecoLamp)
		{
			thisRoom->objects[objActive].data.f.topLeft.h = 
					where.h - HalfRectWide(&srcRects[what]);
			thisRoom->objects[objActive].data.f.topLeft.v = kDecoLampTop;
			QSetRect(&newRect, 0, 0, RectWide(&srcRects[what]), 
					RectTall(&srcRects[what]));
			QOffsetRect(&newRect, thisRoom->objects[objActive].data.f.topLeft.h, 
					thisRoom->objects[objActive].data.f.topLeft.v);
			thisRoom->objects[objActive].data.f.length = 0;
		}
		else if (what == kFlourescent)
		{
			thisRoom->objects[objActive].data.f.topLeft.h = 
					where.h - HalfRectWide(&srcRects[what]);
			thisRoom->objects[objActive].data.f.topLeft.v = kFlourescentTop;
			newRect = srcRects[what];
			QOffsetRect(&newRect, thisRoom->objects[objActive].data.f.topLeft.h, 
					thisRoom->objects[objActive].data.f.topLeft.v);
			thisRoom->objects[objActive].data.f.length = 64;
		}
		else if (what == kTrackLight)
		{
			thisRoom->objects[objActive].data.f.topLeft.h = 
					where.h - HalfRectWide(&srcRects[what]);
			thisRoom->objects[objActive].data.f.topLeft.v = kTrackLightTop;
			newRect = srcRects[what];
			QOffsetRect(&newRect, thisRoom->objects[objActive].data.f.topLeft.h, 
					thisRoom->objects[objActive].data.f.topLeft.v);
			thisRoom->objects[objActive].data.f.length = 64;
		}
		else
		{
			thisRoom->objects[objActive].data.f.topLeft.h = 
					where.h - HalfRectWide(&srcRects[what]);
			thisRoom->objects[objActive].data.f.topLeft.v = 
					where.v - HalfRectTall(&srcRects[what]);
			QSetRect(&newRect, 0, 0, RectWide(&srcRects[what]), 
					RectTall(&srcRects[what]));
			QOffsetRect(&newRect, thisRoom->objects[objActive].data.f.topLeft.h, 
					thisRoom->objects[objActive].data.f.topLeft.v);
			thisRoom->objects[objActive].data.f.length = 0;
		}
		thisRoom->objects[objActive].data.f.initial = true;
		thisRoom->objects[objActive].data.f.state = true;
		thisRoom->objects[objActive].data.f.byte0 = 0;
		thisRoom->objects[objActive].data.f.byte1 = 0;
		drawWholeRoom = true;
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
		if ((what != kGuitar) && (what != kCinderBlock) && (what != kFlowerBox) && 
				(what != kCDs) && (what != kCustomPict) && 
				(HowManyShredderObjects() >= kMaxShredded))
		{
			ShoutNoMoreSpecialObjects();
			return (false);
		}
		thisRoom->objects[objActive].data.g.topLeft.h = 
				where.h - HalfRectWide(&srcRects[what]);
		thisRoom->objects[objActive].data.g.topLeft.v = 
				where.v - HalfRectTall(&srcRects[what]);
		QSetRect(&newRect, 0, 0, RectWide(&srcRects[what]), 
				RectTall(&srcRects[what]));
		QOffsetRect(&newRect, thisRoom->objects[objActive].data.g.topLeft.h, 
				thisRoom->objects[objActive].data.g.topLeft.v);
		if (what == kToaster)
		{
			thisRoom->objects[objActive].data.g.height = 64;
			thisRoom->objects[objActive].data.g.delay = 10 + RandomInt(10);
		}
		else if (what == kOutlet)
		{
			thisRoom->objects[objActive].data.g.height = 0;
			thisRoom->objects[objActive].data.g.delay = 10 + RandomInt(10);
		}
		else if (what == kCustomPict)
		{
			thisRoom->objects[objActive].data.g.height = 10000;
			thisRoom->objects[objActive].data.g.delay = 0;
		}
		else
		{
			thisRoom->objects[objActive].data.g.height = 0;
			thisRoom->objects[objActive].data.g.delay = 0;
		}
		if (what == kMicrowave)
			thisRoom->objects[objActive].data.g.byte0 = 7;
		else
			thisRoom->objects[objActive].data.g.byte0 = 0;
		thisRoom->objects[objActive].data.g.initial = true;
		thisRoom->objects[objActive].data.g.state = true;
		break;
		
		case kBalloon:
		case kCopterLf:
		case kCopterRt:
		case kDartLf:
		case kDartRt:
		case kCobweb:
		if ((what != kCobweb) && (HowManyDynamicObjects() >= kMaxDynamicObs))
		{
			ShoutNoMoreSpecialObjects();
			return (false);
		}
		if (what == kDartLf)
		{
			thisRoom->objects[objActive].data.h.topLeft.h = 
					kRoomWide - RectWide(&srcRects[what]);
		}
		else if (what == kDartRt)
		{
			thisRoom->objects[objActive].data.h.topLeft.h = 0;
		}
		else
		{
			thisRoom->objects[objActive].data.h.topLeft.h = 
					where.h - HalfRectWide(&srcRects[what]);
		}
		if ((what == kDartLf) || (what == kDartRt) || (what == kCobweb))
		{
			thisRoom->objects[objActive].data.h.topLeft.v = 
					where.v - HalfRectTall(&srcRects[what]);
		}
		else
		{
			thisRoom->objects[objActive].data.h.topLeft.v = 
					(kTileHigh / 2) - HalfRectTall(&srcRects[what]);
		}
		QSetRect(&newRect, 0, 0, RectWide(&srcRects[what]), 
				RectTall(&srcRects[what]));
		QOffsetRect(&newRect, thisRoom->objects[objActive].data.h.topLeft.h, 
				thisRoom->objects[objActive].data.h.topLeft.v);
		thisRoom->objects[objActive].data.h.length = 0;
		if (what == kCobweb)
			thisRoom->objects[objActive].data.h.delay = 0;
		else
			thisRoom->objects[objActive].data.h.delay = 10 + RandomInt(10);
		thisRoom->objects[objActive].data.h.byte0 = 0;
		thisRoom->objects[objActive].data.h.initial = true;
		thisRoom->objects[objActive].data.h.state = true;
		break;
		
		case kBall:
		case kDrip:
		case kFish:
		if (HowManyDynamicObjects() >= kMaxDynamicObs)
		{
			ShoutNoMoreSpecialObjects();
			return (false);
		}
		thisRoom->objects[objActive].data.h.topLeft.h = 
				where.h - HalfRectWide(&srcRects[what]);
		thisRoom->objects[objActive].data.h.topLeft.v = 
				where.v - HalfRectTall(&srcRects[what]);
		QSetRect(&newRect, 0, 0, RectWide(&srcRects[what]), 
				RectTall(&srcRects[what]));
		QOffsetRect(&newRect, thisRoom->objects[objActive].data.h.topLeft.h, 
				thisRoom->objects[objActive].data.h.topLeft.v);
		thisRoom->objects[objActive].data.h.length = 64;
		if (what == kBall)
			thisRoom->objects[objActive].data.h.delay = 0;
		else
			thisRoom->objects[objActive].data.h.delay = 10 + RandomInt(10);
		thisRoom->objects[objActive].data.h.byte0 = 0;
		thisRoom->objects[objActive].data.h.initial = true;
		thisRoom->objects[objActive].data.h.state = true;
		break;
		
		case kMousehole:
		newRect = srcRects[what];
		CenterRectOnPoint(&newRect, where);
		newRect.bottom = kMouseholeBottom;
		newRect.top = newRect.bottom - RectTall(&srcRects[what]);
		thisRoom->objects[objActive].data.i.bounds = newRect;
		thisRoom->objects[objActive].data.i.pict = 0;
		break;
		
		case kFireplace:
		newRect = srcRects[what];
		CenterRectOnPoint(&newRect, where);
		newRect.bottom = kFireplaceBottom;
		newRect.top = newRect.bottom - RectTall(&srcRects[what]);
		thisRoom->objects[objActive].data.i.bounds = newRect;
		thisRoom->objects[objActive].data.i.pict = 0;
		break;
		
		case kFlower:
		GetKeys(theseKeys);
		if (!BitTst(&theseKeys, kShiftKeyMap))
			wasFlower = RandomInt(kNumFlowers);
		newRect = flowerSrc[wasFlower];
		CenterRectOnPoint(&newRect, where);
		thisRoom->objects[objActive].data.i.bounds = newRect;
		thisRoom->objects[objActive].data.i.pict = wasFlower;
		break;
		
		case kOzma:
		case kMirror:
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
		newRect = srcRects[what];
		CenterRectOnPoint(&newRect, where);
		thisRoom->objects[objActive].data.i.bounds = newRect;
		thisRoom->objects[objActive].data.i.pict = 0;
		break;
		
		default:
		return (false);
		break;
	}
	
	thisRoom->objects[objActive].what = what;
	thisRoom->numObjects++;
	if (KeepObjectLegal())
	{
	}
	fileDirty = true;
	UpdateMenus(false);
	handled = ObjectHasHandle(&direction, &dist);
	
	if (showItNow)
	{
		if (drawWholeRoom)
			ReadyBackground(thisRoom->background, thisRoom->tiles);
		GetThisRoomsObjRects();
		DrawThisRoomsObjects();
		InvalWindowRect(mainWindow, &mainWindowRect);
		
		if (handled)
		{
			StartMarqueeHandled(&roomObjectRects[objActive], direction, dist);
			HandleBlowerGlider();
		}
		else
			StartMarquee(&roomObjectRects[objActive]);
	}
	
#endif
	
	return (true);
}

//--------------------------------------------------------------  FindEmptyObjectSlot

#ifndef COMPILEDEMO
short FindEmptyObjectSlot (void)
{
	short		i, emptySlot;
	
	emptySlot = -1;
	for (i = 0; i < kMaxRoomObs; i++)
		if (thisRoom->objects[i].what == kObjectIsEmpty)
		{
			emptySlot = i;
			break;
		}
	
	return (emptySlot);
}

//--------------------------------------------------------------  FindObjectSlotInRoom

short FindObjectSlotInRoom (short roomNumber)
{
	roomType	*testRoomPtr;
	short		i, emptySlot;
	char		wasState;
	
	emptySlot = -1;
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	testRoomPtr = &((*thisHouse)->rooms[roomNumber]);
	
	for (i = 0; i < kMaxRoomObs; i++)
		if (testRoomPtr->objects[i].what == kObjectIsEmpty)
		{
			emptySlot = i;
			break;
		}
	
	
	HSetState((Handle)thisHouse, wasState);
	
	return (emptySlot);
}

//--------------------------------------------------------------  DoesRoomNumHaveObject

Boolean DoesRoomNumHaveObject (short room, short what)
{
	roomType	*testRoomPtr;
	short		i;
	char		wasState;
	Boolean		hasIt;
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	testRoomPtr = &((*thisHouse)->rooms[room]);
	
	hasIt = false;
	
	for (i = 0; i < kMaxRoomObs; i++)
		if (testRoomPtr->objects[i].what == what)
		{
			hasIt = true;
			break;
		}
	
	
	HSetState((Handle)thisHouse, wasState);
	
	return (hasIt);
}

//--------------------------------------------------------------  ShoutNoMoreObjects

void ShoutNoMoreObjects (void)
{
	short		hitWhat;
	
//	CenterAlert(kNoMoreObjectsAlert);
	hitWhat = Alert(kNoMoreObjectsAlert, nil);
}

//--------------------------------------------------------------  HowManyCandleObjects

short HowManyCandleObjects (void)
{
	short		i, aCandle;
	
	aCandle = 0;
	for (i = 0; i < kMaxRoomObs; i++)
		if ((thisRoom->objects[i].what == kTaper) || 
				(thisRoom->objects[i].what == kCandle) || 
				(thisRoom->objects[i].what == kStubby))
			aCandle++;
	
	return (aCandle);
}

//--------------------------------------------------------------  HowManyTikiObjects

short HowManyTikiObjects (void)
{
	short		i, aTiki;
	
	aTiki = 0;
	for (i = 0; i < kMaxRoomObs; i++)
		if (thisRoom->objects[i].what == kTiki)
			aTiki++;
	
	return (aTiki);
}

//--------------------------------------------------------------  HowManyBBQObjects

short HowManyBBQObjects (void)
{
	short		i, aBBQ;
	
	aBBQ = 0;
	for (i = 0; i < kMaxRoomObs; i++)
		if (thisRoom->objects[i].what == kBBQ)
			aBBQ++;
	
	return (aBBQ);
}

//--------------------------------------------------------------  HowManyCuckooObjects

short HowManyCuckooObjects (void)
{
	short		i, aCuckoo;
	
	aCuckoo = 0;
	for (i = 0; i < kMaxRoomObs; i++)
		if (thisRoom->objects[i].what == kCuckoo)
			aCuckoo++;
	
	return (aCuckoo);
}

//--------------------------------------------------------------  HowManyBandsObjects

short HowManyBandsObjects (void)
{
	short		i, aBands;
	
	aBands = 0;
	for (i = 0; i < kMaxRoomObs; i++)
		if (thisRoom->objects[i].what == kBands)
			aBands++;
	
	return (aBands);
}

//--------------------------------------------------------------  HowManyGreaseObjects

short HowManyGreaseObjects (void)
{
	short		i, aGrease;
	
	aGrease = 0;
	for (i = 0; i < kMaxRoomObs; i++)
		if ((thisRoom->objects[i].what == kGreaseRt) || 
				(thisRoom->objects[i].what == kGreaseLf))
			aGrease++;
	
	return (aGrease);
}

//--------------------------------------------------------------  HowManyStarsObjects

short HowManyStarsObjects (void)
{
	short		i, aStar;
	
	aStar = 0;
	for (i = 0; i < kMaxRoomObs; i++)
		if (thisRoom->objects[i].what == kStar)
			aStar++;
	
	return (aStar);
}

//--------------------------------------------------------------  HowManySoundObjects

short HowManySoundObjects (void)
{
	short		i, aSound;
	
	aSound = 0;
	for (i = 0; i < kMaxRoomObs; i++)
		if (thisRoom->objects[i].what == kSoundTrigger)
			aSound++;
	
	return (aSound);
}

//--------------------------------------------------------------  HowManyUpStairsObjects

short HowManyUpStairsObjects (void)
{
	short		i, aStair;
	
	aStair = 0;
	for (i = 0; i < kMaxRoomObs; i++)
		if (thisRoom->objects[i].what == kUpStairs)
			aStair++;
	
	return (aStair);
}

//--------------------------------------------------------------  HowManyDownStairsObjects

short HowManyDownStairsObjects (void)
{
	short		i, aStair;
	
	aStair = 0;
	for (i = 0; i < kMaxRoomObs; i++)
		if (thisRoom->objects[i].what == kDownStairs)
			aStair++;
	
	return (aStair);
}

//--------------------------------------------------------------  HowManyShredderObjects

short HowManyShredderObjects (void)
{
	short		i, aShredder;
	
	aShredder = 0;
	for (i = 0; i < kMaxRoomObs; i++)
		if (thisRoom->objects[i].what == kShredder)
			aShredder++;
	
	return (aShredder);
}

//--------------------------------------------------------------  HowManyDynamicObjects

short HowManyDynamicObjects (void)
{
	short		i, aDinah;
	
	aDinah = 0;
	for (i = 0; i < kMaxRoomObs; i++)
		if ((thisRoom->objects[i].what == kSparkle) || 
				(thisRoom->objects[i].what == kToaster) || 
				(thisRoom->objects[i].what == kMacPlus) || 
				(thisRoom->objects[i].what == kTV) || 
				(thisRoom->objects[i].what == kCoffee) || 
				(thisRoom->objects[i].what == kOutlet) || 
				(thisRoom->objects[i].what == kVCR) || 
				(thisRoom->objects[i].what == kStereo) || 
				(thisRoom->objects[i].what == kMicrowave) || 
				(thisRoom->objects[i].what == kBalloon) || 
				(thisRoom->objects[i].what == kCopterLf) || 
				(thisRoom->objects[i].what == kCopterRt) || 
				(thisRoom->objects[i].what == kDartLf) || 
				(thisRoom->objects[i].what == kDartRt) || 
				(thisRoom->objects[i].what == kBall) || 
				(thisRoom->objects[i].what == kDrip) || 
				(thisRoom->objects[i].what == kFish))
			aDinah++;
	
	return (aDinah);
}

//--------------------------------------------------------------  ShoutNoMoreSpecialObjects

void ShoutNoMoreSpecialObjects (void)
{
	short		hitWhat;
	
//	CenterAlert(kNoMoreSpecialAlert);
	hitWhat = Alert(kNoMoreSpecialAlert, nil);
}

#endif

