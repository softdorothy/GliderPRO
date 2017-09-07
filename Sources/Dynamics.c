
//============================================================================
//----------------------------------------------------------------------------
//								   Dynamics.c
//----------------------------------------------------------------------------
//============================================================================


#include "Externs.h"
#include "Environ.h"
#include "MainWindow.h"
#include "Objects.h"
#include "RectUtils.h"
#include "Room.h"


#define kShoveVelocity		8


Rect		breadSrc[kNumBreadPicts];

extern	dynaPtr		dinahs;
extern	bandPtr		bands;
extern	short		numBands, tvWithMovieNumber;
extern	Boolean		evenFrame, twoPlayerGame, onePlayerLeft, playerDead;


//==============================================================  Functions
//--------------------------------------------------------------  CheckDynamicCollision

// Checks for a collision betwen the glider and one of the dynamic objects.
// For example, did the glider hit a flying piece of toast?

void CheckDynamicCollision (short who, gliderPtr thisGlider, Boolean doOffset)
{
	Rect		dinahRect;
	
	dinahRect = dinahs[who].dest;
	if (doOffset)
		QOffsetRect(&dinahRect, -playOriginH, -playOriginV);
	
	if (SectGlider(thisGlider, &dinahRect, true))
	{
		if ((thisGlider->mode == kGliderNormal) || 
				(thisGlider->mode == kGliderFaceLeft) || 
				(thisGlider->mode == kGliderFaceRight) || 
				(thisGlider->mode == kGliderBurning) || 
				(thisGlider->mode == kGliderGoingFoil) || 
				(thisGlider->mode == kGliderLosingFoil))
		{
			if ((foilTotal > 0) || (thisGlider->mode == kGliderLosingFoil))
			{
				if (IsRectLeftOfRect(&dinahRect, &thisGlider->dest))
					thisGlider->hDesiredVel = kShoveVelocity;
				else
					thisGlider->hDesiredVel = -kShoveVelocity;
				if (dinahs[who].vVel < 0)
					thisGlider->vDesiredVel = dinahs[who].vVel;
				PlayPrioritySound(kFoilHitSound, kFoilHitPriority);
				if ((evenFrame) && (foilTotal > 0))
				{
					foilTotal--;
					if (foilTotal <= 0)
						StartGliderFoilLosing(thisGlider);
				}
			}
			else
			{
				StartGliderFadingOut(thisGlider);
				PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
			}
		}
	}
}

//--------------------------------------------------------------  DidBandHitDynamic

// Checks to see if a rubber band struck a dynamic.

Boolean DidBandHitDynamic (short who)
{
	Rect		dinahRect;
	short		i;
	Boolean		collided;
	
	dinahRect = dinahs[who].dest;
	
	for (i = 0; i < numBands; i++)
	{
		if (bands[i].dest.bottom < dinahRect.top)
			collided = false;
		else if (bands[i].dest.top > dinahRect.bottom)
			collided = false;
		else if (bands[i].dest.right < dinahRect.left)
			collided = false;
		else if (bands[i].dest.left > dinahRect.right)
			collided = false;
		else
			collided = true;
		
		if (collided)
			break;
	}
	
	return (collided);
}

//--------------------------------------------------------------  RenderToast

// The following handful of functions handle drawing specific "dynamic" objecsts.

void RenderToast (short who)
{
	Rect		src, dest;
	short		vClip;
	
	if (dinahs[who].moving)
	{
		dest = dinahs[who].dest;
		QOffsetRect(&dest, playOriginH, playOriginV);
		src = breadSrc[dinahs[who].frame];
		vClip = dinahs[who].dest.bottom - dinahs[who].hVel;
		if (vClip > 0)
		{
			src.bottom -= vClip;
			dest.bottom -= vClip;
		}
		
		CopyMask((BitMap *)*GetGWorldPixMap(toastSrcMap), 
				(BitMap *)*GetGWorldPixMap(toastMaskMap), 
				(BitMap *)*GetGWorldPixMap(workSrcMap), 
				&src, &src, &dest);
		
		AddRectToBackRects(&dest);
		dest = dinahs[who].whole;
		QOffsetRect(&dest, playOriginH, playOriginV);
		AddRectToWorkRects(&dest);
	}
}

//--------------------------------------------------------------  RenderBalloon

void RenderBalloon (short who)
{
	Rect		src, dest;
	
	if (dinahs[who].moving)
	{
		dest = dinahs[who].dest;
		QOffsetRect(&dest, playOriginH, playOriginV);
		src = balloonSrc[dinahs[who].frame];
		
		CopyMask((BitMap *)*GetGWorldPixMap(balloonSrcMap), 
				(BitMap *)*GetGWorldPixMap(balloonMaskMap), 
				(BitMap *)*GetGWorldPixMap(workSrcMap), 
				&src, &src, &dest);
		
		AddRectToBackRects(&dest);
		dest = dinahs[who].whole;
		QOffsetRect(&dest, playOriginH, playOriginV);
		AddRectToWorkRects(&dest);
	}
}

//--------------------------------------------------------------  RenderCopter

void RenderCopter (short who)
{
	Rect		src, dest;
	
	if (dinahs[who].moving)
	{
		dest = dinahs[who].dest;
		QOffsetRect(&dest, playOriginH, playOriginV);
		src = copterSrc[dinahs[who].frame];
		
		CopyMask((BitMap *)*GetGWorldPixMap(copterSrcMap), 
				(BitMap *)*GetGWorldPixMap(copterMaskMap), 
				(BitMap *)*GetGWorldPixMap(workSrcMap), 
				&src, &src, &dest);
		
		AddRectToBackRects(&dest);
		dest = dinahs[who].whole;
		QOffsetRect(&dest, playOriginH, playOriginV);
		AddRectToWorkRects(&dest);
	}
}

//--------------------------------------------------------------  RenderDart

void RenderDart (short who)
{
	Rect		src, dest;
	
	if (dinahs[who].moving)
	{
		dest = dinahs[who].dest;
		QOffsetRect(&dest, playOriginH, playOriginV);
		src = dartSrc[dinahs[who].frame];
		
		CopyMask((BitMap *)*GetGWorldPixMap(dartSrcMap), 
				(BitMap *)*GetGWorldPixMap(dartMaskMap), 
				(BitMap *)*GetGWorldPixMap(workSrcMap), 
				&src, &src, &dest);
		
		AddRectToBackRects(&dest);
		dest = dinahs[who].whole;
		QOffsetRect(&dest, playOriginH, playOriginV);
		AddRectToWorkRects(&dest);
	}
}

//--------------------------------------------------------------  RenderBall

void RenderBall (short who)
{
	Rect		src, dest;
	
	dest = dinahs[who].dest;
	QOffsetRect(&dest, playOriginH, playOriginV);
	src = ballSrc[dinahs[who].frame];
	
	CopyMask((BitMap *)*GetGWorldPixMap(ballSrcMap), 
			(BitMap *)*GetGWorldPixMap(ballMaskMap), 
			(BitMap *)*GetGWorldPixMap(workSrcMap), 
			&src, &src, &dest);
	
	AddRectToBackRects(&dest);
	dest = dinahs[who].whole;
	QOffsetRect(&dest, playOriginH, playOriginV);
	AddRectToWorkRects(&dest);
}

//--------------------------------------------------------------  RenderDrip

void RenderDrip (short who)
{
	Rect		src, dest;
	
	dest = dinahs[who].dest;
	QOffsetRect(&dest, playOriginH, playOriginV);
	src = dripSrc[dinahs[who].frame];
	
	CopyMask((BitMap *)*GetGWorldPixMap(dripSrcMap), 
			(BitMap *)*GetGWorldPixMap(dripMaskMap), 
			(BitMap *)*GetGWorldPixMap(workSrcMap), 
			&src, &src, &dest);
	
	AddRectToBackRects(&dest);
	dest = dinahs[who].whole;
	QOffsetRect(&dest, playOriginH, playOriginV);
	AddRectToWorkRects(&dest);
}

//--------------------------------------------------------------  RenderFish

void RenderFish (short who)
{
	Rect		src, dest;
	
	dest = dinahs[who].dest;
	QOffsetRect(&dest, playOriginH, playOriginV);
	src = fishSrc[dinahs[who].frame];
	
	if (dinahs[who].moving)
	{
		CopyMask((BitMap *)*GetGWorldPixMap(fishSrcMap), 
				(BitMap *)*GetGWorldPixMap(fishMaskMap), 
				(BitMap *)*GetGWorldPixMap(workSrcMap), 
				&src, &src, &dest);
		AddRectToBackRects(&dest);
		dest = dinahs[who].whole;
		QOffsetRect(&dest, playOriginH, playOriginV);
		AddRectToWorkRects(&dest);
	}
	else
	{
		CopyBits((BitMap *)*GetGWorldPixMap(fishSrcMap), 
				(BitMap *)*GetGWorldPixMap(workSrcMap), 
				&src, &dest, srcCopy, nil);
		AddRectToBackRects(&dest);
		dest = dinahs[who].whole;
		QOffsetRect(&dest, playOriginH, playOriginV);
		AddRectToWorkRects(&dest);
	}
}

//--------------------------------------------------------------  HandleSparkleObject

// The following handful of functions are called each game frame and handle…
// the movement and state of the various types of dynamic objects.

void HandleSparkleObject (short who)
{
	Rect		tempRect;
	
	if (dinahs[who].active)								// is it on?
	{
		if (dinahs[who].frame <= 0)						// is it idle?
		{												// it is idle
			dinahs[who].timer--;
			if (dinahs[who].timer <= 0)
			{
				dinahs[who].timer = RandomInt(240) + 60;// reset timer
				dinahs[who].frame = kNumSparkleModes;	// time to sparkle
				tempRect = dinahs[who].dest;
				AddSparkle(&tempRect);
				PlayPrioritySound(kMysticSound, kMysticPriority);
			}
		}
		else											// it's sparkling
			dinahs[who].frame--;
	}
	else
	{
	}
}

//--------------------------------------------------------------  HandleToast

void HandleToast (short who)
{
	Rect		dest;
	
	if (dinahs[who].moving)
	{
		if (evenFrame)
		{
			dinahs[who].frame++;
			if (dinahs[who].frame >= kNumBreadPicts)
				dinahs[who].frame = 0;
		}
		if (twoPlayerGame)
		{
			if (onePlayerLeft)
			{
				if (playerDead == theGlider.which)
					CheckDynamicCollision(who, &theGlider2, false);
				else
					CheckDynamicCollision(who, &theGlider, false);
			}
			else
			{
				CheckDynamicCollision(who, &theGlider, false);
				CheckDynamicCollision(who, &theGlider2, false);
			}
		}
		else
			CheckDynamicCollision(who, &theGlider, false);
		VOffsetRect(&dinahs[who].dest, dinahs[who].vVel);
		dinahs[who].whole = dinahs[who].dest;
		if (dinahs[who].vVel > 0)
			dinahs[who].whole.top -= dinahs[who].vVel;
		else
			dinahs[who].whole.bottom -= dinahs[who].vVel;
		dinahs[who].vVel++;									// falls
		if (dinahs[who].vVel > dinahs[who].count)
		{
			dest = dinahs[who].whole;
			QOffsetRect(&dest, playOriginH, playOriginV);
			AddRectToWorkRects(&dest);
			dinahs[who].moving = false;
			dinahs[who].frame = dinahs[who].timer;
			PlayPrioritySound(kToastLandSound, kToastLandPriority);
		}
	}
	else
	{
		if (dinahs[who].active)
			dinahs[who].frame--;
		if (dinahs[who].frame <= 0)
		{
			if (dinahs[who].active)
			{
				dinahs[who].vVel = (short)-dinahs[who].count;
				dinahs[who].frame = 0;
				dinahs[who].moving = true;
				PlayPrioritySound(kToastLaunchSound, kToastLaunchPriority);
			}
			else
				dinahs[who].frame = dinahs[who].timer;
		}
	}
}

//--------------------------------------------------------------  HandleMacPlus

void HandleMacPlus (short who)
{
	if (dinahs[who].timer > 0)
	{
		dinahs[who].timer--;
		if (dinahs[who].active)
		{
			if (dinahs[who].timer == 0)
				AddRectToWorkRects(&dinahs[who].dest);
			else if (dinahs[who].timer == 1)
			{
				PlayPrioritySound(kMacBeepSound, kMacBeepPriority);
				CopyBits((BitMap *)*GetGWorldPixMap(applianceSrcMap), 
						(BitMap *)*GetGWorldPixMap(backSrcMap), 
						&plusScreen2, &dinahs[who].dest, 
						srcCopy, nil);
				AddRectToBackRects(&dinahs[who].dest);
			}
			else if (dinahs[who].timer == 30)
				PlayPrioritySound(kMacOnSound, kMacOnPriority);
		}
		else
		{
			if (dinahs[who].timer == 0)
				AddRectToWorkRects(&dinahs[who].dest);
			else if (dinahs[who].timer == 1)
			{
				PlayPrioritySound(kMacOffSound, kMacOffPriority);
				CopyBits((BitMap *)*GetGWorldPixMap(applianceSrcMap), 
						(BitMap *)*GetGWorldPixMap(backSrcMap), 
						&plusScreen1, &dinahs[who].dest, 
						srcCopy, nil);
				AddRectToBackRects(&dinahs[who].dest);
			}
		}
	}
}

//--------------------------------------------------------------  HandleTV

void HandleTV (short who)
{
	if (dinahs[who].timer > 0)
	{
		dinahs[who].timer--;
		if (dinahs[who].active)
		{
			if (dinahs[who].timer == 0)
			{
				if ((thisMac.hasQT) && (hasMovie) && (tvInRoom) && 
						(who == tvWithMovieNumber))
				{
				}
				else
				{
					AddRectToWorkRects(&dinahs[who].dest);
				}
			}
			else if (dinahs[who].timer == 1)
			{
				PlayPrioritySound(kTVOnSound, kTVOnPriority);
				if ((thisMac.hasQT) && (hasMovie) && (tvInRoom) && 
						(who == tvWithMovieNumber))
				{
				}
				else
				{
					CopyBits((BitMap *)*GetGWorldPixMap(applianceSrcMap), 
							(BitMap *)*GetGWorldPixMap(backSrcMap), 
							&tvScreen2, &dinahs[who].dest, 
							srcCopy, nil);
					AddRectToBackRects(&dinahs[who].dest);
				}
			}
		}
		else
		{
			if (dinahs[who].timer == 0)
				AddRectToWorkRects(&dinahs[who].dest);
			else if (dinahs[who].timer == 1)
			{
				PlayPrioritySound(kTVOffSound, kTVOffPriority);
				CopyBits((BitMap *)*GetGWorldPixMap(applianceSrcMap), 
						(BitMap *)*GetGWorldPixMap(backSrcMap), 
						&tvScreen1, &dinahs[who].dest, 
						srcCopy, nil);
				AddRectToBackRects(&dinahs[who].dest);
			}
		}
	}
}

//--------------------------------------------------------------  HandleCoffee

void HandleCoffee (short who)
{
	if (dinahs[who].timer > 0)
	{
		dinahs[who].timer--;
		if (dinahs[who].active)
		{
			if (dinahs[who].timer == 0)
			{
				AddRectToWorkRects(&dinahs[who].dest);
				dinahs[who].timer = 200 + RandomInt(200);
			}
			else if (dinahs[who].timer == 1)
			{
				PlayPrioritySound(kMacOnSound, kMacOnPriority);
				CopyBits((BitMap *)*GetGWorldPixMap(applianceSrcMap), 
						(BitMap *)*GetGWorldPixMap(backSrcMap), 
						&coffeeLight2, &dinahs[who].dest, 
						srcCopy, nil);
				AddRectToBackRects(&dinahs[who].dest);
			}
			else if (dinahs[who].timer == 100)
			{
				PlayPrioritySound(kCoffeeSound, kCoffeePriority);
				dinahs[who].timer = 200 + RandomInt(200);
			}
		}
		else
		{
			if (dinahs[who].timer == 0)
				AddRectToWorkRects(&dinahs[who].dest);
			else if (dinahs[who].timer == 1)
			{
				PlayPrioritySound(kMacOffSound, kMacOffPriority);
				CopyBits((BitMap *)*GetGWorldPixMap(applianceSrcMap), 
						(BitMap *)*GetGWorldPixMap(backSrcMap), 
						&coffeeLight1, &dinahs[who].dest, 
						srcCopy, nil);
				AddRectToBackRects(&dinahs[who].dest);
			}
		}
	}
}

//--------------------------------------------------------------  HandleOutlet

void HandleOutlet (short who)
{
	if (dinahs[who].position != 0)
	{
		dinahs[who].timer--;
		
		if (twoPlayerGame)
		{
			if (onePlayerLeft)
			{
				if (playerDead == theGlider.which)
					CheckDynamicCollision(who, &theGlider2, false);
				else
					CheckDynamicCollision(who, &theGlider, false);
			}
			else
			{
				CheckDynamicCollision(who, &theGlider, true);
				CheckDynamicCollision(who, &theGlider2, true);
			}
		}
		else
			CheckDynamicCollision(who, &theGlider, true);
		
		if (dinahs[who].timer <= 0)
		{
			dinahs[who].frame = 0;
			dinahs[who].position = 0;
			dinahs[who].timer = dinahs[who].count;
		}
		else
		{
			if ((dinahs[who].timer % 5) == 0)
				PlayPrioritySound(kZapSound, kZapPriority);
			dinahs[who].frame++;
			if (dinahs[who].frame >= kNumOutletPicts)
				dinahs[who].frame = 1;
		}
		
		if ((dinahs[who].position != 0) || (dinahs[who].hVel > 0))
		{
			CopyBits((BitMap *)*GetGWorldPixMap(applianceSrcMap), 
					(BitMap *)*GetGWorldPixMap(workSrcMap), 
					&outletSrc[dinahs[who].frame], 
					&dinahs[who].dest, 
					srcCopy, nil);
		}
		else
		{
//			SetPort((GrafPtr)workSrcMap);
			PaintRect(&dinahs[who].dest);
		}
		AddRectToWorkRects(&dinahs[who].dest);
	}
	else
	{
		if (dinahs[who].active)
			dinahs[who].timer--;
		
		if (dinahs[who].timer <= 0)
		{
			if (dinahs[who].active)
			{
				dinahs[who].position = 1;
				dinahs[who].timer = kLengthOfZap;
				PlayPrioritySound(kZapSound, kZapPriority);
			}
			else
				dinahs[who].timer = dinahs[who].count;
		}
	}
}

//--------------------------------------------------------------  HandleVCR

void HandleVCR (short who)
{
	if (dinahs[who].timer > 0)
	{
		dinahs[who].timer--;
		if (dinahs[who].active)
		{
			if (dinahs[who].timer == 0)
			{
				AddRectToWorkRects(&dinahs[who].dest);
				dinahs[who].timer = 115;
			}
			else if (dinahs[who].timer == 5)
				PlayPrioritySound(kMacOnSound, kMacOnPriority);
			else if (dinahs[who].timer == 1)
			{
				PlayPrioritySound(kVCRSound, kVCRPriority);
				CopyBits((BitMap *)*GetGWorldPixMap(applianceSrcMap), 
						(BitMap *)*GetGWorldPixMap(backSrcMap), 
						&vcrTime2, &dinahs[who].dest, 
						srcCopy, nil);
				AddRectToBackRects(&dinahs[who].dest);
			}
			else if (dinahs[who].timer == 100)
			{
				AddRectToWorkRects(&dinahs[who].dest);
				dinahs[who].timer = 115;
				dinahs[who].frame = 1 - dinahs[who].frame;
			}
			else if (dinahs[who].timer == 101)
			{
				if (dinahs[who].frame == 0)
				{
					CopyBits((BitMap *)*GetGWorldPixMap(applianceSrcMap), 
							(BitMap *)*GetGWorldPixMap(backSrcMap), 
							&vcrTime2, &dinahs[who].dest, 
							srcCopy, nil);
					AddRectToBackRects(&dinahs[who].dest);
				}
				else
				{
					CopyBits((BitMap *)*GetGWorldPixMap(applianceSrcMap), 
							(BitMap *)*GetGWorldPixMap(backSrcMap), 
							&vcrTime1, &dinahs[who].dest, 
							srcCopy, nil);
					AddRectToBackRects(&dinahs[who].dest);
				}
			}
		}
		else
		{
			if (dinahs[who].timer == 0)
				AddRectToWorkRects(&dinahs[who].dest);
			else if (dinahs[who].timer == 1)
			{
				PlayPrioritySound(kMacOffSound, kMacOffPriority);
				CopyBits((BitMap *)*GetGWorldPixMap(applianceSrcMap), 
						(BitMap *)*GetGWorldPixMap(backSrcMap), 
						&vcrTime1, &dinahs[who].dest, 
						srcCopy, nil);
				AddRectToBackRects(&dinahs[who].dest);
			}
		}
	}
}

//--------------------------------------------------------------  HandleStereo

void HandleStereo (short who)
{
	if (dinahs[who].timer > 0)
	{
		dinahs[who].timer--;
		if (dinahs[who].active)
		{
			if (dinahs[who].timer == 0)
			{
				AddRectToWorkRects(&dinahs[who].dest);
				ToggleMusicWhilePlaying();
			}
			else if (dinahs[who].timer == 1)
			{
				PlayPrioritySound(kMacOnSound, kMacOnPriority);
				CopyBits((BitMap *)*GetGWorldPixMap(applianceSrcMap), 
						(BitMap *)*GetGWorldPixMap(backSrcMap), 
						&stereoLight2, &dinahs[who].dest, 
						srcCopy, nil);
				AddRectToBackRects(&dinahs[who].dest);
			}
		}
		else
		{
			if (dinahs[who].timer == 0)
			{
				AddRectToWorkRects(&dinahs[who].dest);
				ToggleMusicWhilePlaying();
			}
			else if (dinahs[who].timer == 1)
			{
				PlayPrioritySound(kMacOffSound, kMacOffPriority);
				CopyBits((BitMap *)*GetGWorldPixMap(applianceSrcMap), 
						(BitMap *)*GetGWorldPixMap(backSrcMap), 
						&stereoLight1, &dinahs[who].dest, 
						srcCopy, nil);
				AddRectToBackRects(&dinahs[who].dest);
			}
		}
	}
}

//--------------------------------------------------------------  HandleMicrowave

void HandleMicrowave (short who)
{
	Rect		dest;
	
	if (dinahs[who].timer > 0)
	{
		dinahs[who].timer--;
		if (dinahs[who].active)
		{
			if (dinahs[who].timer == 0)
				AddRectToWorkRects(&dinahs[who].dest);
			else if (dinahs[who].timer == 1)
			{
				PlayPrioritySound(kMacOnSound, kMacOnPriority);
				dest = dinahs[who].dest;
				dest.right = dest.left + 16;
				CopyBits((BitMap *)*GetGWorldPixMap(applianceSrcMap), 
						(BitMap *)*GetGWorldPixMap(backSrcMap), 
						&microOn, &dest, 
						srcCopy, nil);
				QOffsetRect(&dest, 16, 0);
				CopyBits((BitMap *)*GetGWorldPixMap(applianceSrcMap), 
						(BitMap *)*GetGWorldPixMap(backSrcMap), 
						&microOn, &dest, 
						srcCopy, nil);
				QOffsetRect(&dest, 16, 0);
				CopyBits((BitMap *)*GetGWorldPixMap(applianceSrcMap), 
						(BitMap *)*GetGWorldPixMap(backSrcMap), 
						&microOn, &dest, 
						srcCopy, nil);
				AddRectToBackRects(&dinahs[who].dest);
			}
		}
		else
		{
			if (dinahs[who].timer == 0)
				AddRectToWorkRects(&dinahs[who].dest);
			else if (dinahs[who].timer == 1)
			{
				PlayPrioritySound(kMacOffSound, kMacOffPriority);
				dest = dinahs[who].dest;
				dest.right = dest.left + 16;
				CopyBits((BitMap *)*GetGWorldPixMap(applianceSrcMap), 
						(BitMap *)*GetGWorldPixMap(backSrcMap), 
						&microOff, &dest, 
						srcCopy, nil);
				QOffsetRect(&dest, 16, 0);
				CopyBits((BitMap *)*GetGWorldPixMap(applianceSrcMap), 
						(BitMap *)*GetGWorldPixMap(backSrcMap), 
						&microOff, &dest, 
						srcCopy, nil);
				QOffsetRect(&dest, 16, 0);
				CopyBits((BitMap *)*GetGWorldPixMap(applianceSrcMap), 
						(BitMap *)*GetGWorldPixMap(backSrcMap), 
						&microOff, &dest, 
						srcCopy, nil);
				AddRectToBackRects(&dinahs[who].dest);
			}
		}
	}
}

