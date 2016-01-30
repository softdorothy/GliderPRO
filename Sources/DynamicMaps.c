//============================================================================
//----------------------------------------------------------------------------
//								  DynamicMaps.c
//----------------------------------------------------------------------------
//============================================================================


#include "Externs.h"
#include "Environ.h"
#include "MainWindow.h"
#include "Objects.h"
#include "RectUtils.h"
#include "Room.h"
#include "Utilities.h"


void BackUpFlames (Rect *, short);
void BackUpTikiFlames (Rect *, short);
void BackUpBBQCoals (Rect *, short);
void BackUpPendulum (Rect *, short);
void BackUpStar (Rect *, short);


sparklePtr	sparkles;
flyingPtPtr	flyingPoints;
flamePtr	flames, tikiFlames, bbqCoals;
pendulumPtr	pendulums;
starPtr		theStars;
shredPtr	shreds;
Rect		pointsSrc[15];
short		numSparkles, numFlyingPts, numChimes;
short		numFlames, numSavedMaps, numTikiFlames, numCoals;
short		numPendulums, clockFrame, numStars, numShredded;


extern	savedType	savedMaps[];
extern	Rect		flame[], tikiFlame[], coals[], pendulumSrc[];
extern	Rect		starSrc[];
extern	short		numGrease, numDynamics;


//==============================================================  Functions
//--------------------------------------------------------------  NilSavedMaps
// Deletes array of "dyanmics" offscreen pixmaps.

void NilSavedMaps (void)
{
	short		i;
	
	for (i = 0; i < kMaxSavedMaps; i++)
	{
		if (savedMaps[i].map != nil)
		{
			DisposeGWorld(savedMaps[i].map);
//			KillOffScreenPixMap(savedMaps[i].map);
			savedMaps[i].map = nil;
		}
		savedMaps[i].where = -1;
		savedMaps[i].who = -1;
	}
	numSavedMaps = 0;
}

//--------------------------------------------------------------  BackUpToSavedMap
// Saves a copy of the room behind an object to an array of pixmaps.
// Then when the object in question is drawn, there is a copy of the…
// room that it obscured so that, should the player get the object,…
// it can be made to "disappear".

short BackUpToSavedMap (Rect *theRect, short where, short who)
{
	Rect		mapRect;
	OSErr		theErr;
	
	if (numSavedMaps >= kMaxSavedMaps)
		return(-1);
	
	mapRect = *theRect;
	ZeroRectCorner(&mapRect);
	savedMaps[numSavedMaps].dest = *theRect;
//	CreateOffScreenPixMap(&mapRect, &savedMaps[numSavedMaps].map);
	theErr = CreateOffScreenGWorld(&savedMaps[numSavedMaps].map, &mapRect, kPreferredDepth);
	
	CopyBits((BitMap *)*GetGWorldPixMap(backSrcMap), 
			GetPortBitMapForCopyBits(savedMaps[numSavedMaps].map), 
			theRect, &mapRect, srcCopy, nil);
	
	savedMaps[numSavedMaps].where = where;
	savedMaps[numSavedMaps].who = who;
	numSavedMaps++;
	
	return (numSavedMaps - 1);	// return array index
}

//--------------------------------------------------------------  ReBackUpSavedMap
// This function is similar to the above, but assumes there is already…
// a slot in the pixmap array for the object.  It re-copies the background…
// and is needed when the lights in the room go on or off.

short ReBackUpSavedMap (Rect *theRect, short where, short who)
{
	Rect		mapRect;
	short		i, foundIndex;
	
	foundIndex = -1;
	
	for (i = 0; i < numSavedMaps; i++)
	{
		if ((savedMaps[i].where == where) && (savedMaps[i].who == who))
		{
			foundIndex = i;
			mapRect = *theRect;
			ZeroRectCorner(&mapRect);
			
			CopyBits((BitMap *)*GetGWorldPixMap(backSrcMap), 
					GetPortBitMapForCopyBits(savedMaps[foundIndex].map), 
					theRect, &mapRect, srcCopy, nil);
			
			return (foundIndex);
		}
	}
	
	return (foundIndex);
}

//--------------------------------------------------------------  RestoreFromSavedMap

// This copies the saved background swatch to the screen - effectively…
// covering up or "erasing" the object.

void RestoreFromSavedMap (short where, short who, Boolean doSparkle)
{
	Rect		mapRect, bounds;
	short		i;
	
	for (i = 0; i < numSavedMaps; i++)
	{
		if ((savedMaps[i].where == where) && (savedMaps[i].who == who) && 
				(savedMaps[i].map != nil))
		{
			mapRect = savedMaps[i].dest;
			ZeroRectCorner(&mapRect);
			
			CopyBits(GetPortBitMapForCopyBits(savedMaps[i].map), 
					(BitMap *)*GetGWorldPixMap(backSrcMap), 
					&mapRect, &savedMaps[i].dest, srcCopy, nil);
			CopyBits(GetPortBitMapForCopyBits(savedMaps[i].map), 
					(BitMap *)*GetGWorldPixMap(workSrcMap), 
					&mapRect, &savedMaps[i].dest, srcCopy, nil);
			
			AddRectToWorkRects(&savedMaps[i].dest);
			
			if (doSparkle)
			{
				bounds = savedMaps[i].dest;
				QOffsetRect(&bounds, -playOriginH, -playOriginV);
				AddSparkle(&bounds);
				PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
			}
			break;
		}
	}
}

//--------------------------------------------------------------  AddSparkle

// This adds a "sparkle" object to the fixed array of sparkles.

void AddSparkle (Rect *theRect)
{
	Rect		centeredRect;
	short		i;
	
	if (numSparkles < kMaxSparkles)
	{
		theRect->left += playOriginH;
		theRect->right += playOriginH;
		theRect->top += playOriginV;
		theRect->bottom += playOriginV;
		
		centeredRect = sparkleSrc[0];
		CenterRectInRect(&centeredRect, theRect);
		
		for (i = 0; i < kMaxSparkles; i++)
			if (sparkles[i].mode == -1)
			{
				sparkles[i].bounds = centeredRect;
				sparkles[i].mode = 0;
				numSparkles++;
				break;
			}
	}
}

//--------------------------------------------------------------  AddFlyingPoint

// This adds a "flying point" object to the array of flying points.

void AddFlyingPoint (Rect *theRect, short points, short hVel, short vVel)
{
	Rect		centeredRect;
	short		i;
	
	if (numFlyingPts < kMaxFlyingPts)
	{
		theRect->left += playOriginH;
		theRect->right += playOriginH;
		theRect->top += playOriginV;
		theRect->bottom += playOriginV;
		
		centeredRect = pointsSrc[0];
		CenterRectInRect(&centeredRect, theRect);
		
		for (i = 0; i < kMaxFlyingPts; i++)
			if (flyingPoints[i].mode == -1)
			{
				flyingPoints[i].dest = centeredRect;
				flyingPoints[i].whole = centeredRect;
				flyingPoints[i].loops = 0;
				flyingPoints[i].hVel = hVel;
				flyingPoints[i].vVel = vVel;
				switch (points)
				{
					case 100:
					flyingPoints[i].start = 12;
					flyingPoints[i].stop = 14;
					break;
					
					case 250:
					flyingPoints[i].start = 9;
					flyingPoints[i].stop = 11;
					break;
					
					case 300:
					flyingPoints[i].start = 6;
					flyingPoints[i].stop = 8;
					break;
					
					case 500:
					flyingPoints[i].start = 3;
					flyingPoints[i].stop = 5;
					break;
					
					default:
					flyingPoints[i].start = 0;
					flyingPoints[i].stop = 2;
					break;
				}
				flyingPoints[i].mode = flyingPoints[i].start;
				numFlyingPts++;
				break;
			}
	}
}

//--------------------------------------------------------------  BackUpFlames

// This makes copies of the area of the room behind a flame.  The flame…
// graphic can be "copy masked" to this pixmap then and then simple…
// CopyBits() calls will properly draw the flame on screen with the…
// proper background.

void BackUpFlames (Rect *src, short index)
{
	Rect		dest;
	short		i;
	
	QSetRect(&dest, 0, 0, 16, 15);
	for (i = 0; i < kNumCandleFlames; i++)
	{
				// Copy background to map.
		CopyBits((BitMap *)*GetGWorldPixMap(backSrcMap), 
				GetPortBitMapForCopyBits(savedMaps[index].map), 
				src, &dest, srcCopy, nil);
		
				// Copy flame to map.
		CopyMask((BitMap *)*GetGWorldPixMap(blowerSrcMap), 
				(BitMap *)*GetGWorldPixMap(blowerMaskMap), 
				GetPortBitMapForCopyBits(savedMaps[index].map), 
				&flame[i], &flame[i], &dest);
		
		
		QOffsetRect(&dest, 0, 15);
	}
}

//--------------------------------------------------------------  ReBackUpFlames

// Like the above function but this is called when the lighting changes…
// in a room (lights go on or off).

void ReBackUpFlames (short where, short who)
{
	short		i, f;
	
	for (i = 0; i < numSavedMaps; i++)
	{
		if ((savedMaps[i].where == where) && (savedMaps[i].who == who))
		{
			for (f = 0; f < numFlames; f++)
			{
				if (flames[f].who == i)
				{
					BackUpFlames(&flames[f].dest, i);
					return;
				}
			}
		}
	}
}

//--------------------------------------------------------------  AddCandleFlame

// This adds a candle flame to tha array of flames.

void AddCandleFlame (short where, short who, short h, short v)
{
	Rect		src, bounds;
	short		savedNum;
	
	if ((numFlames >= kMaxCandles) || (h < 16) || (v < 15))
		return;
	
	QSetRect(&src, 0, 0, 16, 15);
	QOffsetRect(&src, h - 8, v - 15);
	if ((thisMac.isDepth == 4) && ((src.left % 2) == 1))
	{
		QOffsetRect(&src, -1, 0);
		if (src.left < 0)
			QOffsetRect(&src, 2, 0);
	}
	QSetRect(&bounds, 0, 0, 16, 15 * kNumCandleFlames);
	savedNum = BackUpToSavedMap(&bounds, where, who);
	if (savedNum != -1)
	{
		BackUpFlames(&src, savedNum);
		flames[numFlames].dest = src;
		flames[numFlames].mode = RandomInt(kNumCandleFlames);
		QSetRect(&flames[numFlames].src, 0, 0, 16, 15);
		QOffsetRect(&flames[numFlames].src, 0, flames[numFlames].mode * 15);
		flames[numFlames].who = savedNum;
		numFlames++;
	}
}

//--------------------------------------------------------------  BackUpTikiFlames
// This is like the function BackUpFlames() but customized for Tiki torches.

void BackUpTikiFlames (Rect *src, short index)
{
	Rect		dest;
	short		i;
	
	QSetRect(&dest, 0, 0, 8, 10);
	for (i = 0; i < kNumTikiFlames; i++)
	{
				// copy background to map
		CopyBits((BitMap *)*GetGWorldPixMap(backSrcMap), 
				GetPortBitMapForCopyBits(savedMaps[index].map), 
				src, &dest, srcCopy, nil);
		
				// copy flame to map
		CopyMask((BitMap *)*GetGWorldPixMap(blowerSrcMap), 
				(BitMap *)*GetGWorldPixMap(blowerMaskMap), 
				GetPortBitMapForCopyBits(savedMaps[index].map), 
				&tikiFlame[i], &tikiFlame[i], &dest);
		
		QOffsetRect(&dest, 0, 10);
	}
}

//--------------------------------------------------------------  ReBackUpTikiFlames

// This is like the function ReBackUpFlames() but customized for Tiki torches.

void ReBackUpTikiFlames (short where, short who)
{
	short		i, f;
	
	for (i = 0; i < numSavedMaps; i++)
	{
		if ((savedMaps[i].where == where) && (savedMaps[i].who == who))
		{
			for (f = 0; f < numTikiFlames; f++)
			{
				if (tikiFlames[f].who == i)
				{
					BackUpTikiFlames(&tikiFlames[f].dest, i);
					return;
				}
			}
		}
	}
}

//--------------------------------------------------------------  AddTikiFlame

// This adds a tiki flame to the array of tiki flames.

void AddTikiFlame (short where, short who, short h, short v)
{
	Rect		src, bounds;
	short		savedNum;
	
	if ((numTikiFlames >= kMaxTikis) || (h < 8) || (v < 10))
		return;
	
	QSetRect(&src, 0, 0, 8, 10);
	if ((thisMac.isDepth == 4) && ((h % 2) == 1))
	{
		h--;
		if (h < 0)
			h += 2;
	}
	QOffsetRect(&src, h, v);
	QSetRect(&bounds, 0, 0, 8, 10 * kNumTikiFlames);
	savedNum = BackUpToSavedMap(&bounds, where, who);
	if (savedNum != -1)
	{
		BackUpTikiFlames(&src, savedNum);
		tikiFlames[numTikiFlames].dest = src;
		tikiFlames[numTikiFlames].mode = RandomInt(kNumTikiFlames);
		QSetRect(&tikiFlames[numTikiFlames].src, 0, 0, 8, 10);
		QOffsetRect(&tikiFlames[numTikiFlames].src, 0, 
				tikiFlames[numTikiFlames].mode * 10);
		tikiFlames[numTikiFlames].who = savedNum;
		numTikiFlames++;
	}
}

//--------------------------------------------------------------  BackUpBBQCoals

// Another one - but for BBQ coals.

void BackUpBBQCoals (Rect *src, short index)
{
	Rect		dest;
	short		i;
	
	QSetRect(&dest, 0, 0, 32, 9);
	for (i = 0; i < kNumBBQCoals; i++)
	{
				// copy background to map
		CopyBits((BitMap *)*GetGWorldPixMap(backSrcMap), 
				GetPortBitMapForCopyBits(savedMaps[index].map), 
				src, &dest, srcCopy, nil);
		
				// copy flame to map
		CopyMask((BitMap *)*GetGWorldPixMap(blowerSrcMap), 
				(BitMap *)*GetGWorldPixMap(blowerMaskMap), 
				GetPortBitMapForCopyBits(savedMaps[index].map), 
				&coals[i], &coals[i], &dest);
		
		QOffsetRect(&dest, 0, 9);
	}
}

//--------------------------------------------------------------  ReBackUpBBQCoals

// Sense a pattern here?

void ReBackUpBBQCoals (short where, short who)
{
	short		i, f;
	
	for (i = 0; i < numSavedMaps; i++)
	{
		if ((savedMaps[i].where == where) && (savedMaps[i].who == who))
		{
			for (f = 0; f < numCoals; f++)
			{
				if (bbqCoals[f].who == i)
				{
					BackUpBBQCoals(&bbqCoals[f].dest, i);
					return;
				}
			}
		}
	}
}

//--------------------------------------------------------------  AddBBQCoals

// Adds BBQ coals to the array of BBQ coals.

void AddBBQCoals (short where, short who, short h, short v)
{
	Rect		src, bounds;
	short		savedNum;
	
	if ((numCoals >= kMaxCoals) || (h < 32) || (v < 9))
		return;
	
	QSetRect(&src, 0, 0, 32, 9);
	if ((thisMac.isDepth == 4) && ((h % 2) == 1))
	{
		h--;
		if (h < 0)
			h += 2;
	}
	QOffsetRect(&src, h, v);
	QSetRect(&bounds, 0, 0, 32, 9 * kNumBBQCoals);
	savedNum = BackUpToSavedMap(&bounds, where, who);
	if (savedNum != -1)
	{
		BackUpBBQCoals(&src, savedNum);
		bbqCoals[numCoals].dest = src;
		bbqCoals[numCoals].mode = RandomInt(kNumBBQCoals);
		QSetRect(&bbqCoals[numCoals].src, 0, 0, 32, 9);
		QOffsetRect(&bbqCoals[numCoals].src, 0, bbqCoals[numCoals].mode * 9);
		bbqCoals[numCoals].who = savedNum;
		
		numCoals++;
	}
}

//--------------------------------------------------------------  BackUpPendulum
// Just like many of the previous functions, but for the pendulum on the…
// cuckoo clock.

void BackUpPendulum (Rect *src, short index)
{
	Rect		dest;
	short		i;
	
	QSetRect(&dest, 0, 0, 32, 28);
	for (i = 0; i < kNumPendulums; i++)
	{
		CopyBits((BitMap *)*GetGWorldPixMap(backSrcMap), 
				GetPortBitMapForCopyBits(savedMaps[index].map), 
				src, &dest, srcCopy, nil);
		
		CopyMask((BitMap *)*GetGWorldPixMap(bonusSrcMap), 
				(BitMap *)*GetGWorldPixMap(bonusMaskMap), 
				GetPortBitMapForCopyBits(savedMaps[index].map), 
				&pendulumSrc[i], &pendulumSrc[i], &dest);
		
		QOffsetRect(&dest, 0, 28);
	}
}

//--------------------------------------------------------------  ReBackUpPendulum

// Backs up the pendulums in the event of lights going on or off.

void ReBackUpPendulum (short where, short who)
{
	short		i, f;
	
	for (i = 0; i < numSavedMaps; i++)
	{
		if ((savedMaps[i].where == where) && (savedMaps[i].who == who))
		{
			for (f = 0; f < numPendulums; f++)
			{
				if (pendulums[f].who == i)
				{
					BackUpPendulum(&pendulums[f].dest, i);
					return;
				}
			}
		}
	}
}

//--------------------------------------------------------------  AddPendulum

// Adds a pendulum to the array of pendulums.

void AddPendulum (short where, short who, short h, short v)
{
	Rect		src, bounds;
	short		savedNum;
	
	if ((numPendulums >= kMaxPendulums) || (h < 32) || (v < 28))
		return;
	
	clockFrame = 10;
	QSetRect(&bounds, 0, 0, 32, 28 * kNumPendulums);
	savedNum = BackUpToSavedMap(&bounds, where, who);
	if (savedNum != -1)
	{
		QSetRect(&src, 0, 0, 32, 28);
		if ((thisMac.isDepth == 4) && ((h % 2) == 1))
		{
			h--;
			if (h < 0)
				h += 2;
		}
		QOffsetRect(&src, h, v);
		BackUpPendulum (&src, savedNum);
		pendulums[numPendulums].dest = src;
		pendulums[numPendulums].mode = 1;
		if (RandomInt(2) == 0)
			pendulums[numPendulums].toOrFro = true;
		else
			pendulums[numPendulums].toOrFro = false;
		pendulums[numPendulums].active = true;
		QSetRect(&pendulums[numPendulums].src, 0, 0, 32, 28);
		QOffsetRect(&pendulums[numPendulums].src, 0, 28);
		pendulums[numPendulums].who = savedNum;
		pendulums[numPendulums].where = where;
		pendulums[numPendulums].link = who;
		numPendulums++;
	}
}

//--------------------------------------------------------------  BackUpStar

// Makes a copy of background beneath a star.

void BackUpStar (Rect *src, short index)
{
	Rect		dest;
	short		i;
	
	QSetRect(&dest, 0, 0, 32, 31);
	for (i = 0; i < 6; i++)
	{
		CopyBits((BitMap *)*GetGWorldPixMap(backSrcMap), 
				GetPortBitMapForCopyBits(savedMaps[index].map), 
				src, &dest, srcCopy, nil);
		
				// copy flame to map
		CopyMask((BitMap *)*GetGWorldPixMap(bonusSrcMap), 
				(BitMap *)*GetGWorldPixMap(bonusMaskMap), 
				GetPortBitMapForCopyBits(savedMaps[index].map), 
				&starSrc[i], &starSrc[i], &dest);
		
		QOffsetRect(&dest, 0, 31);
	}
}

//--------------------------------------------------------------  ReBackUpStar

// Re-backs up the stars - in the event of lighting switch.

void ReBackUpStar (short where, short who)
{
	short		i, f;
	
	for (i = 0; i < numSavedMaps; i++)
	{
		if ((savedMaps[i].where == where) && (savedMaps[i].who == who))
		{
			for (f = 0; f < numStars; f++)
			{
				if (theStars[f].who == i)
				{
					BackUpStar(&theStars[f].dest, i);
					return;
				}
			}
		}
	}
}

//--------------------------------------------------------------  AddStar

// Adds a star to the star array.

void AddStar (short where, short who, short h, short v)
{
	Rect		src, bounds;
	short		savedNum;
	
	if (numStars >= kMaxStars)
		return;
	
	QSetRect(&src, 0, 0, 32, 31);
	if ((thisMac.isDepth == 4) && ((h % 2) == 1))
	{
		h--;
		if (h < 0)
			h += 2;
	}
	QOffsetRect(&src, h, v);
	
	QSetRect(&bounds, 0, 0, 32, 31 * 6);
	savedNum = BackUpToSavedMap(&bounds, where, who);
	if (savedNum != -1)
	{
		BackUpStar(&src, savedNum);
		theStars[numStars].dest = src;
		theStars[numStars].mode = RandomInt(6);
		QSetRect(&theStars[numStars].src, 0, 0, 32, 31);
		QOffsetRect(&theStars[numStars].src, 0, theStars[numStars].mode * 31);
		theStars[numStars].who = savedNum;
		theStars[numStars].link = who;
		theStars[numStars].where = where;
		
		numStars++;
	}
}

//--------------------------------------------------------------  StopPendulum

// Will set a flag to kill a pendulum.

void StopPendulum (short where, short who)
{
	short		i;
	
	for (i = 0; i < numPendulums; i++)
	{
		if ((pendulums[i].link == who) && (pendulums[i].where == where))
			pendulums[i].active = false;
	}
}

//--------------------------------------------------------------  StopStar

// Will set a flag to kill a star.

void StopStar (short where, short who)
{
	short		i;
	
	for (i = 0; i < numStars; i++)
	{
		if ((theStars[i].link == who) && (theStars[i].where == where))
			theStars[i].mode = -1;
	}
}

//--------------------------------------------------------------  AddAShreddedGlider

// Adds a shredded glider.

void AddAShreddedGlider (Rect *bounds)
{
	if (numShredded > kMaxShredded)
		return;
	
	shreds[numShredded].bounds.left = bounds->left + 4;
	shreds[numShredded].bounds.right = shreds[numShredded].bounds.left + 40;
	shreds[numShredded].bounds.top = bounds->top + 14;
	shreds[numShredded].bounds.bottom = shreds[numShredded].bounds.top;
	shreds[numShredded].frame = 0;
	
	numShredded++;
}

//--------------------------------------------------------------  RemoveShreds

// Remves the shredded glider.

void RemoveShreds (void)
{
	short		largest, who, i;
	
	largest = 0;
	who = -1;
	for (i = 0; i < numShredded; i++)
	{
		if (shreds[i].frame > largest)
		{
			largest = shreds[i].frame;
			who = i;
		}
	}
	
	if (who != -1)
	{
		if (who == (numShredded - 1))
		{
			numShredded--;
			shreds[who].frame = 0;
		}
		else
		{
			numShredded--;
			shreds[who].bounds = shreds[numShredded].bounds;
			shreds[who].frame = shreds[numShredded].frame;
			shreds[numShredded].frame = 0;
		}
	}
}

//--------------------------------------------------------------  ZeroFlamesAndTheLike

// Zeroes all counters that indicate the number of flames, pendulums, etc there…
// are in a room.  Called before a room is drawn.  As the room is drawn, the…
// above functions are called and the counters incremented as objects of the…
// specified types are drawn.

void ZeroFlamesAndTheLike (void)
{
	numFlames = 0;
	numTikiFlames = 0;
	numCoals = 0;
	numPendulums = 0;
	numGrease = 0;
	numStars = 0;
	numShredded = 0;
	numChimes = 0;
}

