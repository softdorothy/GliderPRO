
//============================================================================
//----------------------------------------------------------------------------
//								   Dynamics2.c
//----------------------------------------------------------------------------
//============================================================================


#include "Externs.h"
#include "RectUtils.h"


#define kBalloonStop		8
#define kBalloonStart		310
#define kCopterStart		8
#define kCopterStop			310
#define kDartVelocity		6
#define kDartStop			310
#define kEnemyDropSpeed		8


extern	dynaPtr		dinahs;
extern	short		numBands;
extern	Boolean		evenFrame, twoPlayerGame, onePlayerLeft, playerDead;


//==============================================================  Functions
//--------------------------------------------------------------  HandleBalloon

void HandleBalloon (short who)
{
	Rect		dest;
	
	if (dinahs[who].moving)
	{
		if (dinahs[who].vVel < 0)
		{
			if (evenFrame)
			{
				dinahs[who].frame++;
				if (dinahs[who].frame >= 6)
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
			
			if ((numBands > 0) && (DidBandHitDynamic(who)))
			{
				dinahs[who].frame = 6;
				dinahs[who].vVel = kEnemyDropSpeed;
				PlayPrioritySound(kPopSound, kPopPriority);
			}
			else
			{
				VOffsetRect(&dinahs[who].dest, dinahs[who].vVel);
				dinahs[who].whole = dinahs[who].dest;
				dinahs[who].whole.bottom -= dinahs[who].vVel;
			}
		}
		else
		{
			if (evenFrame)
			{
				dinahs[who].frame++;
				if (dinahs[who].frame >= 8)
					dinahs[who].frame = 6;
			}
			
			VOffsetRect(&dinahs[who].dest, dinahs[who].vVel);
			dinahs[who].whole = dinahs[who].dest;
			dinahs[who].whole.top -= dinahs[who].vVel;
		}
		
		if ((dinahs[who].dest.top <= kBalloonStop) || 
				(dinahs[who].dest.bottom >= kBalloonStart))
		{
			dest = dinahs[who].whole;
			QOffsetRect(&dest, playOriginH, playOriginV);
			AddRectToWorkRects(&dest);
			dest = dinahs[who].dest;
			AddSparkle(&dest);
			PlayPrioritySound(kEnemyOutSound, kEnemyOutPriority);
			dinahs[who].moving = false;
			dinahs[who].vVel = -2;
			dinahs[who].timer = dinahs[who].count;
			dinahs[who].dest.bottom = kBalloonStart;
			dinahs[who].dest.top = dinahs[who].dest.bottom - 
					RectTall(&balloonSrc[0]);
			dinahs[who].whole = dinahs[who].dest;
		}
	}
	else		// balloon is idle, waiting to appear
	{
		if (dinahs[who].active)
		{
			dinahs[who].timer--;
			if (dinahs[who].timer <= 0)
			{
				dinahs[who].moving = true;
				if (dinahs[who].count < kStartSparkle)
				{
					dest = dinahs[who].dest;
					AddSparkle(&dest);
					PlayPrioritySound(kEnemyInSound, kEnemyInPriority);
				}
			}
			else if (dinahs[who].timer == kStartSparkle)
			{
				dest = dinahs[who].dest;
				AddSparkle(&dest);
				PlayPrioritySound(kEnemyInSound, kEnemyInPriority);
			}
		}
	}
}

//--------------------------------------------------------------  HandleCopter

void HandleCopter (short who)
{
	Rect		dest;
	
	if (dinahs[who].moving)					// is 'copter about?
	{
		if (dinahs[who].hVel != 0)			// 'copter was not shot
		{
			dinahs[who].frame++;
			if (dinahs[who].frame >= 8)
				dinahs[who].frame = 0;
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
			if ((numBands > 0) && (DidBandHitDynamic(who)))
			{
				dinahs[who].frame = 8;
				dinahs[who].hVel = 0;
				dinahs[who].vVel = kEnemyDropSpeed;
				PlayPrioritySound(kPaperCrunchSound, kPaperCrunchPriority);
			}
			else
			{
				HOffsetRect(&dinahs[who].dest, dinahs[who].hVel);
				VOffsetRect(&dinahs[who].dest, dinahs[who].vVel);
				dinahs[who].whole = dinahs[who].dest;
				dinahs[who].whole.top -= dinahs[who].vVel;
				if (dinahs[who].hVel < 0)
					dinahs[who].whole.right -= dinahs[who].hVel;
				else
					dinahs[who].whole.left -= dinahs[who].hVel;
			}
		}
		else								// 'copter was shot
		{
			dinahs[who].frame++;
			if (dinahs[who].frame >= 10)
				dinahs[who].frame = 8;
			VOffsetRect(&dinahs[who].dest, dinahs[who].vVel);
			dinahs[who].whole = dinahs[who].dest;
			dinahs[who].whole.top -= dinahs[who].vVel;
		}
		
		if ((dinahs[who].dest.top <= kCopterStart) || 
				(dinahs[who].dest.bottom >= kCopterStop))
		{
			dest = dinahs[who].whole;
			QOffsetRect(&dest, playOriginH, playOriginV);
			AddRectToWorkRects(&dest);
			dest = dinahs[who].dest;
			AddSparkle(&dest);
			PlayPrioritySound(kEnemyOutSound, kEnemyOutPriority);
			dinahs[who].moving = false;
			dinahs[who].vVel = 2;
			if (dinahs[who].type == kCopterLf)
				dinahs[who].hVel = -1;
			else
				dinahs[who].hVel = 1;
			dinahs[who].timer = dinahs[who].count;
			dinahs[who].dest.top = kCopterStart;
			dinahs[who].dest.bottom = dinahs[who].dest.top + 
					RectTall(&copterSrc[0]);
			dinahs[who].dest.left = dinahs[who].position;
			dinahs[who].dest.right = dinahs[who].dest.left + 32;
			dinahs[who].whole = dinahs[who].dest;
		}
	}
	else
	{
		if (dinahs[who].active)
		{
			dinahs[who].timer--;
			if (dinahs[who].timer <= 0)
			{
				dinahs[who].moving = true;
				if (dinahs[who].count < kStartSparkle)
				{
					dest = dinahs[who].dest;
					AddSparkle(&dest);
					PlayPrioritySound(kEnemyInSound, kEnemyInPriority);
				}
			}
			else if (dinahs[who].timer == kStartSparkle)
			{
				dest = dinahs[who].dest;
				AddSparkle(&dest);
				PlayPrioritySound(kEnemyInSound, kEnemyInPriority);
			}
		}
	}
}

//--------------------------------------------------------------  HandleDart

void HandleDart (short who)
{
	Rect		dest;
	
	if (dinahs[who].moving)					// Dart has appeared
	{
		if (dinahs[who].hVel != 0)			// meaning it isn't falling
		{
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
			if ((numBands > 0) && (DidBandHitDynamic(who)))
			{
				if (dinahs[who].type == kDartLf)
					dinahs[who].frame = 1;
				else
					dinahs[who].frame = 3;
				dinahs[who].hVel = 0;
				dinahs[who].vVel = kEnemyDropSpeed;
				PlayPrioritySound(kPaperCrunchSound, kPaperCrunchPriority);
			}
			else
			{
				HOffsetRect(&dinahs[who].dest, dinahs[who].hVel);
				VOffsetRect(&dinahs[who].dest, dinahs[who].vVel);
				dinahs[who].whole = dinahs[who].dest;
				dinahs[who].whole.top -= dinahs[who].vVel;
				if (dinahs[who].hVel < 0)
					dinahs[who].whole.right -= dinahs[who].hVel;
				else
					dinahs[who].whole.left -= dinahs[who].hVel;
			}
		}
		else							// dart is falling straight down
		{
			VOffsetRect(&dinahs[who].dest, dinahs[who].vVel);
			dinahs[who].whole = dinahs[who].dest;
			dinahs[who].whole.top -= dinahs[who].vVel;
		}
		
		if ((dinahs[who].dest.left <= 0) || 
				(dinahs[who].dest.right >= kRoomWide) || 
				(dinahs[who].dest.bottom >= kDartStop))
		{
			dest = dinahs[who].whole;
			QOffsetRect(&dest, playOriginH, playOriginV);
			AddRectToWorkRects(&dest);
			dest = dinahs[who].dest;
			AddSparkle(&dest);
			PlayPrioritySound(kEnemyOutSound, kEnemyOutPriority);
			dinahs[who].moving = false;
			dinahs[who].vVel = 2;
			if (dinahs[who].type == kDartLf)
			{
				dinahs[who].frame = 0;
				dinahs[who].hVel = -kDartVelocity;
				dinahs[who].dest.right = kRoomWide;
				dinahs[who].dest.left = dinahs[who].dest.right - 
						RectWide(&dartSrc[0]);
			}
			else
			{
				dinahs[who].frame = 2;
				dinahs[who].hVel = kDartVelocity;
				dinahs[who].dest.left = 0;
				dinahs[who].dest.right = dinahs[who].dest.left + 
						RectWide(&dartSrc[0]);
			}
			dinahs[who].timer = dinahs[who].count;
			dinahs[who].dest.top = dinahs[who].position;
			dinahs[who].dest.bottom = dinahs[who].dest.top + 
					RectTall(&dartSrc[0]);
			dinahs[who].whole = dinahs[who].dest;
		}
	}
	else
	{
		if (dinahs[who].active)
		{
			dinahs[who].timer--;
			if (dinahs[who].timer <= 0)
			{
				dinahs[who].moving = true;
				if (dinahs[who].count < kStartSparkle)
				{
					dest = dinahs[who].dest;
					AddSparkle(&dest);
					PlayPrioritySound(kEnemyInSound, kEnemyInPriority);
				}
			}
			else if (dinahs[who].timer == kStartSparkle)
			{
				dest = dinahs[who].dest;
				AddSparkle(&dest);
				PlayPrioritySound(kEnemyInSound, kEnemyInPriority);
			}
		}
	}
}

//--------------------------------------------------------------  HandleBall

void HandleBall (short who)
{
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
	
	if (dinahs[who].moving)										// is ball bouncing?
	{
		VOffsetRect(&dinahs[who].dest, dinahs[who].vVel);
		if (dinahs[who].dest.bottom >= dinahs[who].position)	// bounce!
		{
			dinahs[who].whole = dinahs[who].dest;
			dinahs[who].whole.top -= dinahs[who].vVel;
			dinahs[who].whole.bottom = dinahs[who].position;
			dinahs[who].dest.bottom = dinahs[who].position;
			dinahs[who].dest.top = dinahs[who].dest.bottom - 32;
			if (dinahs[who].active)
				dinahs[who].vVel = dinahs[who].count;
			else
			{
				dinahs[who].vVel = -((dinahs[who].vVel * 3) / 4);
				if (dinahs[who].vVel == 0)
					dinahs[who].moving = false;					// stop bounce
			}
			if (dinahs[who].whole.bottom < dinahs[who].dest.bottom)
				dinahs[who].whole.bottom = dinahs[who].dest.bottom;
			PlayPrioritySound(kBounceSound, kBouncePriority);
			if (dinahs[who].moving)
				dinahs[who].frame = 1;
		}
		else
		{
			dinahs[who].whole = dinahs[who].dest;
			if (dinahs[who].vVel > 0)
				dinahs[who].whole.top -= dinahs[who].vVel;
			else
				dinahs[who].whole.bottom -= dinahs[who].vVel;
			if (evenFrame)
				dinahs[who].vVel++;
			dinahs[who].frame = 0;
		}
	}
	else
	{
		if (dinahs[who].active)
		{
			dinahs[who].vVel = dinahs[who].count;
			dinahs[who].moving = true;
			evenFrame = true;
		}
	}
}

//--------------------------------------------------------------  HandleDrip

void HandleDrip (short who)
{
	Rect		dest;
	
	if (dinahs[who].moving)
	{
		if (evenFrame)
			dinahs[who].frame = 9 - dinahs[who].frame;
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
		if (dinahs[who].dest.bottom >= dinahs[who].position)
		{
			dest = dinahs[who].whole;
			QOffsetRect(&dest, playOriginH, playOriginV);
			AddRectToWorkRects(&dest);
			dinahs[who].dest.top = dinahs[who].hVel;
			dinahs[who].dest.bottom = dinahs[who].dest.top + 12;
			PlayPrioritySound(kDropSound, kDropPriority);
			dinahs[who].vVel = 0;
			dinahs[who].timer = dinahs[who].count;
			dinahs[who].frame = 3;
			dinahs[who].moving = false;
		}
		else
		{
			dinahs[who].whole = dinahs[who].dest;
			dinahs[who].whole.top -= dinahs[who].vVel;
			if (evenFrame)
				dinahs[who].vVel++;
		}
	}
	else
	{
		if (dinahs[who].active)
		{
			dinahs[who].timer--;
			
			if (dinahs[who].timer == 6)
				dinahs[who].frame = 0;
			else if (dinahs[who].timer == 4)
				dinahs[who].frame = 1;
			else if (dinahs[who].timer == 2)
				dinahs[who].frame = 2;
			else if (dinahs[who].timer <= 0)
			{
				VOffsetRect(&dinahs[who].dest, 3);
				dinahs[who].whole = dinahs[who].dest;
				dinahs[who].moving = true;
				dinahs[who].frame = 4;
				PlayPrioritySound(kDripSound, kDripPriority);
			}
		}
	}
}

//--------------------------------------------------------------  HandleFish

void HandleFish (short who)
{
	Rect		dest;
	
	if (dinahs[who].moving)										// fish leaping
	{
		if ((dinahs[who].vVel >= 0) && (dinahs[who].frame < 7))
			dinahs[who].frame++;
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
		if (dinahs[who].dest.bottom >= dinahs[who].position)	// splash down
		{
			dest = dinahs[who].whole;
			QOffsetRect(&dest, playOriginH, playOriginV);
			AddRectToWorkRects(&dest);
			dinahs[who].dest.bottom = dinahs[who].position;
			dinahs[who].dest.top = dinahs[who].dest.bottom - 16;
			dinahs[who].whole = dinahs[who].dest;
			dinahs[who].whole.top -= 2;
			PlayPrioritySound(kDropSound, kDropPriority);
			dinahs[who].vVel = dinahs[who].count;
			dinahs[who].timer = dinahs[who].hVel;
			dinahs[who].frame = 0;
			dinahs[who].moving = false;
			PlayPrioritySound(kFishInSound, kFishInPriority);
		}
		else
		{
			dinahs[who].whole = dinahs[who].dest;
			if (dinahs[who].vVel > 0)
				dinahs[who].whole.top -= dinahs[who].vVel;
			else
				dinahs[who].whole.bottom -= dinahs[who].vVel;
			if (evenFrame)
				dinahs[who].vVel++;
		}
	}
	else														// fish idle
	{
		dinahs[who].whole = dinahs[who].dest;
		if ((dinahs[who].timer & 0x0003) == 0x0003)
		{
			dinahs[who].frame++;
			if (dinahs[who].frame > 3)
				dinahs[who].frame = 0;
			if ((dinahs[who].frame == 1) || (dinahs[who].frame == 2))
			{
				dinahs[who].dest.top++;
				dinahs[who].dest.bottom++;
				dinahs[who].whole.bottom++;
			}
			else
			{
				dinahs[who].dest.top--;
				dinahs[who].dest.bottom--;
				dinahs[who].whole.top--;
			}
		}
		if (dinahs[who].active)
		{
			dinahs[who].timer--;
			if (dinahs[who].timer <= 0)								// fish leaps
			{
				dinahs[who].whole = dinahs[who].dest;
				dinahs[who].moving = true;
				dinahs[who].frame = 4;
				PlayPrioritySound(kFishOutSound, kFishOutPriority);
			}
		}
	}
}

