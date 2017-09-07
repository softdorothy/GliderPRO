//============================================================================
//----------------------------------------------------------------------------
//								  RubberBands.c
//----------------------------------------------------------------------------
//============================================================================


#include "Externs.h"
#include "RectUtils.h"


#define kRubberBandVelocity		20
#define kBandFallCount			4
#define kKillBandMode			-1


void CheckBandCollision (short);
void KillBand (short);


bandPtr		bands;
Rect		bandsSrcRect;
Rect		bandRects[3];
GWorldPtr	bandsSrcMap;
GWorldPtr	bandsMaskMap;
short		numBands, bandHitLast;

extern	hotPtr		hotSpots;
extern	long		gameFrame;
extern	short		nHotSpots, leftThresh, rightThresh;
extern	Boolean		twoPlayerGame, onePlayerLeft, playerDead;


//==============================================================  Functions
//--------------------------------------------------------------  CheckBandCollision

void CheckBandCollision (short who)
{
	short		i, action, whoLinked;
	Boolean		collided, nothingCollided;
	
	nothingCollided = true;
	
	if ((leftThresh == kLeftWallLimit) && (bands[who].dest.left < kLeftWallLimit))
	{
		if (bands[who].hVel < 0)
			bands[who].hVel = -bands[who].hVel;
		bands[who].dest.left = kLeftWallLimit;
		bands[who].dest.right = bands[who].dest.left + 16;
		PlayPrioritySound(kBandReboundSound, kBandReboundPriority);
		collided = true;
	}
	else if ((rightThresh == kRightWallLimit) && (bands[who].dest.right > kRightWallLimit))
	{
		if (bands[who].hVel > 0)
			bands[who].hVel = -bands[who].hVel;
		bands[who].dest.right = kRightWallLimit;
		bands[who].dest.left = bands[who].dest.right - 16;
		PlayPrioritySound(kBandReboundSound, kBandReboundPriority);
		collided = true;
	}
	
	for (i = 0; i < nHotSpots; i++)
	{
		if (hotSpots[i].isOn)
		{
			action = hotSpots[i].action;
			if ((action == kDissolveIt) || (action == kRewardIt) || 
					(action == kSwitchIt) || (action == kTriggerIt) || 
					(action == kBounceIt))
			{
				if (bands[who].dest.bottom < hotSpots[i].bounds.top)
					collided = false;
				else if (bands[who].dest.top > hotSpots[i].bounds.bottom)
					collided = false;
				else if (bands[who].dest.right < hotSpots[i].bounds.left)
					collided = false;
				else if (bands[who].dest.left > hotSpots[i].bounds.right)
					collided = false;
				else
					collided = true;
				
				if (collided)
				{
					nothingCollided = false;	// we have detected a collision
					if (bandHitLast != i)		// don't count it if same as last frame
					{							// we don't want rapid on/off toggles
						bandHitLast = i;		// note who so we don't double-toggle it
						if ((action == kDissolveIt) || (action == kBounceIt))
						{
							if (bands[who].hVel > 0)
							{
								if ((bands[who].dest.right - bands[who].hVel) < 
										hotSpots[i].bounds.left)
								{
									bands[who].hVel = -bands[who].hVel;
									bands[who].dest.right = hotSpots[i].bounds.left;
									bands[who].dest.left = bands[who].dest.right - 16;
								}
								else
									bands[who].mode = kKillBandMode;
							}
							else
							{
								if ((bands[who].dest.left - bands[who].hVel) > 
										hotSpots[i].bounds.right)
								{
									bands[who].hVel = -bands[who].hVel;
									bands[who].dest.left = hotSpots[i].bounds.right;
									bands[who].dest.right = bands[who].dest.left + 16;
								}
								else
									bands[who].mode = kKillBandMode;
							}
							PlayPrioritySound(kBandReboundSound, kBandReboundPriority);
							break;
						}
						else if (action == kRewardIt)
						{
							whoLinked = hotSpots[i].who;
							if ((masterObjects[whoLinked].theObject.what == kGreaseRt) || 
									(masterObjects[whoLinked].theObject.what == kGreaseLf))
							{
								if (SetObjectState(thisRoomNumber, 
										masterObjects[whoLinked].objectNum, 0, whoLinked))
									SpillGrease(masterObjects[whoLinked].dynaNum, 
											masterObjects[whoLinked].hotNum);
								hotSpots[i].isOn = false;
							}
						}
						else if (action == kSwitchIt)
						{
							HandleSwitches(&hotSpots[i]);
						}
						else if (action == kTriggerIt)
						{
							ArmTrigger(&hotSpots[i]);
						}
					}
				}
			}
		}
	}
	
	if (nothingCollided)		// the rubberband has hit nothing
		bandHitLast = -1;		// so make note of that for the next time
	
	if (bands[who].hVel != 0)
	{
		if (bands[who].dest.bottom < theGlider.dest.top)
			collided = false;
		else if (bands[who].dest.top > theGlider.dest.bottom)
			collided = false;
		else if (bands[who].dest.right < theGlider.dest.left)
			collided = false;
		else if (bands[who].dest.left > theGlider.dest.right)
			collided = false;
		else
			collided = true;
		
		if (collided)
		{
			if ((!twoPlayerGame) || (!onePlayerLeft) || (playerDead == kPlayer2))
			{
				theGlider.hVel += (bands[who].hVel / 2);
				bands[who].hVel = 0;
				PlayPrioritySound(kHitWallSound, kHitWallPriority);
			}
		}
		
		if (twoPlayerGame)
		{
			if (bands[who].dest.bottom < theGlider2.dest.top)
				collided = false;
			else if (bands[who].dest.top > theGlider2.dest.bottom)
				collided = false;
			else if (bands[who].dest.right < theGlider2.dest.left)
				collided = false;
			else if (bands[who].dest.left > theGlider2.dest.right)
				collided = false;
			else
				collided = true;
			
			if (collided)
			{
				if ((!onePlayerLeft) || (playerDead == kPlayer1))
				{
					theGlider2.hVel += (bands[who].hVel / 2);
					bands[who].hVel = 0;
					PlayPrioritySound(kHitWallSound, kHitWallPriority);
				}
			}
		}
	}
	if ((bands[who].dest.left < kLeftWallLimit) || 
			(bands[who].dest.right > kRightWallLimit))
	{
		bands[who].mode = kKillBandMode;
	}
	else if (bands[who].dest.bottom > kFloorLimit)
	{
		bands[who].mode = kKillBandMode;
	}
}

//--------------------------------------------------------------  HandleBands

void HandleBands (void)
{
	Rect		dest;
	short		i, count;
	
	if (numBands == 0)
		return;
	
	for (i = 0; i < numBands; i++)
	{
		bands[i].mode++;
		if (bands[i].mode > 2)
			bands[i].mode = 0;
		
		bands[i].count++;
		if (bands[i].count >= kBandFallCount)
		{
			bands[i].vVel++;
			bands[i].count = 0;
		}
		
		dest = bands[i].dest;
		QOffsetRect(&dest, playOriginH, playOriginV);
		AddRectToWorkRects(&dest);
		
		bands[i].dest.left += bands[i].hVel;
		bands[i].dest.right += bands[i].hVel;
		bands[i].dest.top += bands[i].vVel;
		bands[i].dest.bottom += bands[i].vVel;
		
		CheckBandCollision(i);
	}
	
	count = 0;
	do
	{
		while (bands[count].mode == kKillBandMode)
		{
			bands[count].mode = 0;
			KillBand(count);
		}
		count++;
	}
	while (count < numBands);
}

//--------------------------------------------------------------  AddBand

Boolean AddBand (gliderPtr thisGlider, short h, short v, Boolean direction)
{
	if (numBands >= kMaxRubberBands)
		return (false);
	
	bands[numBands].mode = 0;
	bands[numBands].count = 0;
	if (thisGlider->tipped)
		bands[numBands].vVel = -2;
	else
		bands[numBands].vVel = 0;
	bands[numBands].dest.left = h - 8;
	bands[numBands].dest.right = h + 8;
	bands[numBands].dest.top = v - 3;
	bands[numBands].dest.bottom = v + 3;
	
	if (direction == kFaceLeft)
	{
		bands[numBands].dest.left -= 32;
		bands[numBands].dest.right -= 32;
		bands[numBands].hVel = -kRubberBandVelocity;
	}
	else
	{
		bands[numBands].dest.left += 32;
		bands[numBands].dest.right += 32;
		bands[numBands].hVel = kRubberBandVelocity;
	}
	
	thisGlider->hVel -= (bands[numBands].hVel / 2);
	numBands++;
	
	PlayPrioritySound(kFireBandSound, kFireBandPriority);
	return (true);
}

//--------------------------------------------------------------  KillBand

void KillBand (short which)
{
	short		lastBand;
	
	lastBand = numBands - 1;
	if (which != lastBand)
		bands[which] = bands[lastBand];
	
	numBands--;
}

//--------------------------------------------------------------  KillAllBands

void KillAllBands (void)
{
	short		i;
	
	for (i = 0; i < kMaxRubberBands; i++)
	{
		bands[i].mode = 0;
	}
	
	numBands = 0;
}

