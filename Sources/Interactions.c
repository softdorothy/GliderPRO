
//============================================================================
//----------------------------------------------------------------------------
//								Interactions.c
//----------------------------------------------------------------------------
//============================================================================


#include "Externs.h"
#include "RectUtils.h"


#define kFloorVentLift			-6
#define kCeilingVentDrop		8
#define kFanStrength			12
#define kBatterySupply			50		// about 2 rooms worth of thrust
#define kHeliumSupply			150
#define kBandsSupply			8
#define kFoilSupply				8


Boolean GliderHitTop (gliderPtr, Rect *);
Boolean GliderInRect (gliderPtr, Rect *);
void BounceGlider (gliderPtr, Rect *);
void CheckEscapeUpTwo (gliderPtr);
void CheckEscapeUp (gliderPtr);
void CheckEscapeDownTwo (gliderPtr);
void CheckEscapeDown (gliderPtr);
void CheckRoofCollision (gliderPtr);
void CheckEscapeLeftTwo (gliderPtr);
void CheckEscapeLeft (gliderPtr);
void CheckEscapeRightTwo (gliderPtr);
void CheckEscapeRight (gliderPtr);
void CheckGliderInRoom (gliderPtr);
void HandleRewards (gliderPtr, hotPtr);
void HandleMicrowaveAction (hotPtr, gliderPtr);
void HandleHotSpotCollision (gliderPtr, hotPtr, short);
void CheckForHotSpots (void);
void WebGlider (gliderPtr, Rect *);


short		otherPlayerEscaped, activeRectEscaped;

extern	hotPtr		hotSpots;
extern	short		nHotSpots, leftThresh, rightThresh, thisTiles[];
extern	short		localNumbers[], thisBackground, numStarsRemaining;
extern	Boolean		leftOpen, rightOpen, topOpen, bottomOpen, evenFrame;
extern	Boolean		twoPlayerGame, newState, onePlayerLeft, playerDead;


//==============================================================  Functions
//--------------------------------------------------------------  GliderHitSides

Boolean GliderHitTop (gliderPtr thisGlider, Rect *theRect)
{
	Rect		glideBounds;
	short		offset;
	Boolean		hitTop;
	
	glideBounds.left = thisGlider->dest.left + 5;
	glideBounds.top = thisGlider->dest.top + 5;
	glideBounds.right = thisGlider->dest.right - 5;
	glideBounds.bottom = thisGlider->dest.bottom - 5;
	
	glideBounds.left -= thisGlider->wasHVel;
	glideBounds.right -= thisGlider->wasHVel;
	
	if (theRect->bottom < glideBounds.top)
		hitTop = false;
	else if (theRect->top > glideBounds.bottom)
		hitTop = false;
	else if (theRect->right < glideBounds.left)
		hitTop = false;
	else if (theRect->left > glideBounds.right)
		hitTop = false;
	else
		hitTop = true;
	
	if (!hitTop)
	{
		PlayPrioritySound(kFoilHitSound, kFoilHitPriority);
		foilTotal--;
		if (foilTotal <= 0)
			StartGliderFoilLosing(thisGlider);
		
		glideBounds.left += thisGlider->wasHVel;
		glideBounds.right += thisGlider->wasHVel;
		if (thisGlider->hVel > 0)
			offset = 2 + glideBounds.right - theRect->left;
		else
			offset = 2 + glideBounds.left - theRect->right;
		
		thisGlider->hVel = -thisGlider->hVel - offset;
	}
	
	return (hitTop);
}

//--------------------------------------------------------------  SectGlider

Boolean SectGlider (gliderPtr thisGlider, Rect *theRect, Boolean scrutinize)
{
	Rect		glideBounds;
	Boolean		itHit;
	
	glideBounds = thisGlider->dest;
	if (thisGlider->mode == kGliderBurning)
		glideBounds.top += 6;
	
	if (scrutinize)
	{
		glideBounds.left += 5;
		glideBounds.top += 5;
		glideBounds.right -= 5;
		glideBounds.bottom -= 5;
	}
	
	if (theRect->bottom < glideBounds.top)
		itHit = false;
	else if (theRect->top > glideBounds.bottom)
		itHit = false;
	else if (theRect->right < glideBounds.left)
		itHit = false;
	else if (theRect->left > glideBounds.right)
		itHit = false;
	else
		itHit = true;
	
	return (itHit);
}

//--------------------------------------------------------------  GliderInRect

Boolean GliderInRect (gliderPtr thisGlider, Rect *theRect)
{
	Rect		glideBounds;
	
	glideBounds = thisGlider->dest;
	
	if (glideBounds.top < theRect->top)
		return (false);
	else if (glideBounds.bottom > theRect->bottom)
		return (false);
	else if (glideBounds.left < theRect->left)
		return (false);
	else if (glideBounds.right > theRect->right)
		return (false);
	else
		return (true);
}

//--------------------------------------------------------------  BounceGlider

void BounceGlider (gliderPtr thisGlider, Rect *theRect)
{
	Rect		glideBounds;
	
	glideBounds = thisGlider->dest;
	if ((theRect->right - glideBounds.left) < (glideBounds.right - theRect->left))
		thisGlider->hVel = theRect->right - glideBounds.left;
	else
		thisGlider->hVel = theRect->left - glideBounds.right;
	if (foilTotal > 0)
		PlayPrioritySound(kFoilHitSound, kFoilHitPriority);
	else
		PlayPrioritySound(kHitWallSound, kHitWallPriority);
}

//--------------------------------------------------------------  CheckEscapeUpTwo

void CheckEscapeUpTwo (gliderPtr thisGlider)
{
	short		offset, leftTile, rightTile;
	
	if (topOpen)
	{
		if (thisGlider->dest.top < kNoCeilingLimit)
		{
			if (otherPlayerEscaped == kNoOneEscaped)
			{
				otherPlayerEscaped = kPlayerEscapedUp;
				RefreshScoreboard(kEscapedTitleMode);
				FlagGliderInLimbo(thisGlider, true);
			}
			else if (otherPlayerEscaped == kPlayerEscapedUp)
			{
				otherPlayerEscaped = kNoOneEscaped;
				MoveRoomToRoom(thisGlider, kAbove);
			}
			else
			{
				PlayPrioritySound(kDontExitSound, kDontExitPriority);
				offset = kNoCeilingLimit - thisGlider->dest.top;
				thisGlider->vVel = -thisGlider->vVel + offset;
			}
		}
	}
	else if (thisBackground == kDirt)
	{
		leftTile = thisGlider->dest.left >> 6;		// Ö 64
		rightTile = thisGlider->dest.right >> 6;	// Ö 64
		
		if ((leftTile >= 0) && (leftTile < 8) && 
				(rightTile >= 0) && (rightTile < 8))
		{
			if (((thisTiles[leftTile] == 5) || 
					(thisTiles[leftTile] == 6)) && 
					((thisTiles[rightTile] == 5) || 
					(thisTiles[rightTile] == 6)))
			{
				if (thisGlider->dest.top < kNoCeilingLimit)
				{
					if (otherPlayerEscaped == kNoOneEscaped)
					{
						otherPlayerEscaped = kPlayerEscapedUp;
						RefreshScoreboard(kEscapedTitleMode);
						FlagGliderInLimbo(thisGlider, true);
					}
					else if (otherPlayerEscaped == kPlayerEscapedUp)
					{
						otherPlayerEscaped = kNoOneEscaped;
						MoveRoomToRoom(thisGlider, kAbove);
					}
					else
					{
						PlayPrioritySound(kDontExitSound, kDontExitPriority);
						offset = kNoCeilingLimit - thisGlider->dest.top;
						thisGlider->vVel = -thisGlider->vVel + offset;
					}
				}
			}
			else
				thisGlider->vVel = kCeilingLimit - thisGlider->dest.top;
		}
		else
			thisGlider->vVel = kCeilingLimit - thisGlider->dest.top;
	}
	else
		thisGlider->vVel = kCeilingLimit - thisGlider->dest.top;
}

//--------------------------------------------------------------  CheckEscapeUp

void CheckEscapeUp (gliderPtr thisGlider)
{
	short		leftTile, rightTile;
	
	if (topOpen)
	{
		if (thisGlider->dest.top < kNoCeilingLimit)
		{
			MoveRoomToRoom(thisGlider, kAbove);
		}
	}
	else if (thisBackground == kDirt)
	{
		leftTile = thisGlider->dest.left >> 6;		// Ö 64
		rightTile = thisGlider->dest.right >> 6;	// Ö 64
		
		if ((leftTile >= 0) && (leftTile < 8) && 
				(rightTile >= 0) && (rightTile < 8))
		{
			if (((thisTiles[leftTile] == 5) || 
					(thisTiles[leftTile] == 6)) && 
					((thisTiles[rightTile] == 5) || 
					(thisTiles[rightTile] == 6)))
			{
				if (thisGlider->dest.top < kNoCeilingLimit)
					MoveRoomToRoom(thisGlider, kAbove);
			}
			else
				thisGlider->vVel = kCeilingLimit - thisGlider->dest.top;
		}
		else
			thisGlider->vVel = kCeilingLimit - thisGlider->dest.top;
	}
	else
		thisGlider->vVel = kCeilingLimit - thisGlider->dest.top;
}

//--------------------------------------------------------------  CheckEscapeDownTwo

void CheckEscapeDownTwo (gliderPtr thisGlider)
{
	short		offset, leftTile, rightTile;
	
	if (bottomOpen)
	{
		if (thisGlider->dest.bottom > kNoFloorLimit)
		{
			if (otherPlayerEscaped == kNoOneEscaped)
			{
				otherPlayerEscaped = kPlayerEscapedDown;
				RefreshScoreboard(kEscapedTitleMode);
				FlagGliderInLimbo(thisGlider, true);
			}
			else if (otherPlayerEscaped == kPlayerEscapedDown)
			{
				otherPlayerEscaped = kNoOneEscaped;
				MoveRoomToRoom(thisGlider, kBelow);
			}
			else
			{
				PlayPrioritySound(kDontExitSound, kDontExitPriority);
				offset = kNoFloorLimit - thisGlider->dest.bottom;
				thisGlider->vVel = -thisGlider->vVel + offset;
			}
		}
	}
	else if (thisBackground == kDirt)
	{
		leftTile = thisGlider->dest.left >> 6;		// Ö 64
		rightTile = thisGlider->dest.right >> 6;	// Ö 64
		
		if ((leftTile >= 0) && (leftTile < 8) && 
				(rightTile >= 0) && (rightTile < 8))
		{
			if (((thisTiles[leftTile] == 2) || 
					(thisTiles[leftTile] == 3)) && 
					((thisTiles[rightTile] == 2) || 
					(thisTiles[rightTile] == 3)))
			{
				if (thisGlider->dest.bottom > kNoFloorLimit)
				{
					if (otherPlayerEscaped == kNoOneEscaped)
					{
						otherPlayerEscaped = kPlayerEscapedDown;
						RefreshScoreboard(kEscapedTitleMode);
						FlagGliderInLimbo(thisGlider, true);
					}
					else if (otherPlayerEscaped == kPlayerEscapedDown)
					{
						otherPlayerEscaped = kNoOneEscaped;
						MoveRoomToRoom(thisGlider, kBelow);
					}
					else
					{
						PlayPrioritySound(kDontExitSound, kDontExitPriority);
						offset = kNoFloorLimit - thisGlider->dest.bottom;
						thisGlider->vVel = -thisGlider->vVel + offset;
					}
				}
			}
			else
			{
				if (thisGlider->ignoreGround)
				{
					if (thisGlider->dest.bottom > kNoFloorLimit)
						MoveRoomToRoom(thisGlider, kBelow);
				}
				else
				{
					thisGlider->vVel = kFloorLimit - thisGlider->dest.bottom;
					StartGliderFadingOut(thisGlider);
					PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
				}
			}
		}
	}
	else
	{
		if (thisGlider->ignoreGround)
		{
			if (thisGlider->dest.bottom > kNoFloorLimit)
				MoveRoomToRoom(thisGlider, kBelow);
		}
		else
		{
			thisGlider->vVel = kFloorLimit - thisGlider->dest.bottom;
			StartGliderFadingOut(thisGlider);
			PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
		}
	}
}

//--------------------------------------------------------------  CheckEscapeDown

void CheckEscapeDown (gliderPtr thisGlider)
{
	short		leftTile, rightTile;
	
	if (bottomOpen)
	{
		if (thisGlider->dest.bottom > kNoFloorLimit)
		{
			MoveRoomToRoom(thisGlider, kBelow);
		}
	}
	else if (thisBackground == kDirt)
	{
		leftTile = thisGlider->dest.left >> 6;		// Ö 64
		rightTile = thisGlider->dest.right >> 6;	// Ö 64
		
		if ((leftTile >= 0) && (leftTile < 8) && (rightTile >= 0) && (rightTile < 8))
		{
			if (((thisTiles[leftTile] == 2) || (thisTiles[leftTile] == 3)) && 
					((thisTiles[rightTile] == 2) || (thisTiles[rightTile] == 3)))
			{
				if (thisGlider->dest.bottom > kNoFloorLimit)
					MoveRoomToRoom(thisGlider, kBelow);
			}
			else
			{
				if (thisGlider->ignoreGround)
				{
					if (thisGlider->dest.bottom > kNoFloorLimit)
						MoveRoomToRoom(thisGlider, kBelow);
				}
				else
				{
					thisGlider->vVel = kFloorLimit - thisGlider->dest.bottom;
					StartGliderFadingOut(thisGlider);
					PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
				}
			}
		}
		else
		{
			if (thisGlider->ignoreGround)
			{
				if (thisGlider->dest.bottom > kNoFloorLimit)
					MoveRoomToRoom(thisGlider, kBelow);
			}
			else
			{
				thisGlider->vVel = kFloorLimit - thisGlider->dest.bottom;
				StartGliderFadingOut(thisGlider);
				PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
			}
		}
	}
	else
	{
		if (thisGlider->ignoreGround)
		{
			if (thisGlider->dest.bottom > kNoFloorLimit)
				MoveRoomToRoom(thisGlider, kBelow);
		}
		else
		{
			thisGlider->vVel = kFloorLimit - thisGlider->dest.bottom;
			StartGliderFadingOut(thisGlider);
			PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
		}
	}
}

//--------------------------------------------------------------  CheckRoofCollision

void CheckRoofCollision (gliderPtr thisGlider)
{
	short		offset, tileOver;
	
	offset = (thisGlider->dest.left + kHalfGliderWide) >> 6;	// Ö 64
	if ((offset >= 0) && (offset <= 7) && (!thisGlider->sliding))
	{
		tileOver = thisTiles[offset];
		if (tileOver == 1)
		{
			if (((thisGlider->dest.left + kHalfGliderWide) - (offset << 6)) > 
					(250 - thisGlider->dest.bottom))
			{
				thisGlider->vVel = kFloorLimit - thisGlider->dest.bottom;
				StartGliderFadingOut(thisGlider);
				PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
			}
		}
		else if (tileOver == 2)
		{
			if (((thisGlider->dest.left + kHalfGliderWide) - (offset << 6)) > 
					(186 - thisGlider->dest.bottom))
			{
				thisGlider->vVel = kFloorLimit - thisGlider->dest.bottom;
				StartGliderFadingOut(thisGlider);
				PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
			}
		}
		else if (tileOver == 5)
		{
			if ((64 - ((thisGlider->dest.left + kHalfGliderWide) - (offset << 6))) > 
					(186 - thisGlider->dest.bottom))
			{
				thisGlider->vVel = kFloorLimit - thisGlider->dest.bottom;
				StartGliderFadingOut(thisGlider);
				PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
			}
		}
		else if (tileOver == 6)
		{
			if ((64 - ((thisGlider->dest.left + kHalfGliderWide) - (offset << 6))) > 
					(250 - thisGlider->dest.bottom))
			{
				thisGlider->vVel = kFloorLimit - thisGlider->dest.bottom;
				StartGliderFadingOut(thisGlider);
				PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
			}
		}
		else
		{
			thisGlider->vVel = kFloorLimit - thisGlider->dest.bottom;
			StartGliderFadingOut(thisGlider);
			PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
		}
	}
}

//--------------------------------------------------------------  CheckEscapeLeftTwo

void CheckEscapeLeftTwo (gliderPtr thisGlider)
{
	short		offset;
	
	if (leftThresh == kLeftWallLimit)
	{
		if (thisGlider->ignoreLeft)
		{
			if (thisGlider->dest.left < kNoLeftWallLimit)
			{
				if (otherPlayerEscaped == kNoOneEscaped)
				{
					otherPlayerEscaped = kPlayerEscapedLeft;
					RefreshScoreboard(kEscapedTitleMode);
					FlagGliderInLimbo(thisGlider, true);
				}
				else if (otherPlayerEscaped == kPlayerEscapedLeft)
				{
					otherPlayerEscaped = kNoOneEscaped;
					MoveRoomToRoom(thisGlider, kToLeft);
				}
				else
				{
					PlayPrioritySound(kDontExitSound, kDontExitPriority);
					offset = kNoLeftWallLimit - thisGlider->dest.left;
					thisGlider->hVel = -thisGlider->hVel + offset;
				}
			}
		}
		else
		{
			if (foilTotal > 0)
				PlayPrioritySound(kFoilHitSound, kFoilHitPriority);
			else
				PlayPrioritySound(kHitWallSound, kHitWallPriority);
			offset = kLeftWallLimit - thisGlider->dest.left;
			thisGlider->hVel = -thisGlider->hVel + offset;
		}
	}
	else
	{
		if (otherPlayerEscaped == kNoOneEscaped)
		{
			otherPlayerEscaped = kPlayerEscapedLeft;
			RefreshScoreboard(kEscapedTitleMode);
			FlagGliderInLimbo(thisGlider, true);
		}
		else if (otherPlayerEscaped == kPlayerEscapedLeft)
		{
			otherPlayerEscaped = kNoOneEscaped;
			MoveRoomToRoom(thisGlider, kToLeft);
		}
		else
		{
			PlayPrioritySound(kDontExitSound, kDontExitPriority);
			offset = kNoLeftWallLimit - thisGlider->dest.left;
			thisGlider->hVel = -thisGlider->hVel + offset;
		}
	}
}

//--------------------------------------------------------------  CheckEscapeLeft

void CheckEscapeLeft (gliderPtr thisGlider)
{
	short		offset;
	
	if (leftThresh == kLeftWallLimit)
	{
		if (thisGlider->ignoreLeft)
		{
			if (thisGlider->dest.left < kNoLeftWallLimit)
				MoveRoomToRoom(thisGlider, kToLeft);
		}
		else
		{
			if (foilTotal > 0)
				PlayPrioritySound(kFoilHitSound, kFoilHitPriority);
			else
				PlayPrioritySound(kHitWallSound, kHitWallPriority);
			offset = kLeftWallLimit - thisGlider->dest.left;
			thisGlider->hVel = -thisGlider->hVel + offset;
		}
	}
	else
		MoveRoomToRoom(thisGlider, kToLeft);
}

//--------------------------------------------------------------  CheckEscapeRightTwo

void CheckEscapeRightTwo (gliderPtr thisGlider)
{
	short		offset;
	
	if (rightThresh == kRightWallLimit)
	{
		if (thisGlider->ignoreRight)
		{
			if (thisGlider->dest.right > kNoRightWallLimit)
			{
				if (otherPlayerEscaped == kNoOneEscaped)
				{
					otherPlayerEscaped = kPlayerEscapedRight;
					RefreshScoreboard(kEscapedTitleMode);
					FlagGliderInLimbo(thisGlider, true);
				}
				else if (otherPlayerEscaped == kPlayerEscapedRight)
				{
					otherPlayerEscaped = kNoOneEscaped;
					MoveRoomToRoom(thisGlider, kToRight);
				}
				else
				{
					PlayPrioritySound(kDontExitSound, kDontExitPriority);
					offset = kNoRightWallLimit - thisGlider->dest.right;
					thisGlider->hVel = -thisGlider->hVel + offset;
				}
			}
		}
		else
		{
			if (foilTotal > 0)
				PlayPrioritySound(kFoilHitSound, kFoilHitPriority);
			else
				PlayPrioritySound(kHitWallSound, kHitWallPriority);
			offset = kRightWallLimit - thisGlider->dest.right;
			thisGlider->hVel = -thisGlider->hVel + offset;
		}
	}
	else
	{
		if (otherPlayerEscaped == kNoOneEscaped)
		{
			otherPlayerEscaped = kPlayerEscapedRight;
			RefreshScoreboard(kEscapedTitleMode);
			FlagGliderInLimbo(thisGlider, true);
		}
		else if (otherPlayerEscaped == kPlayerEscapedRight)
		{
			otherPlayerEscaped = kNoOneEscaped;
			MoveRoomToRoom(thisGlider, kToRight);
		}
		else
		{
			PlayPrioritySound(kDontExitSound, kDontExitPriority);
			offset = kNoRightWallLimit - thisGlider->dest.right;
			thisGlider->hVel = -thisGlider->hVel + offset;
		}
	}
}

//--------------------------------------------------------------  CheckEscapeRight

void CheckEscapeRight (gliderPtr thisGlider)
{
	short		offset;
	
	if (rightThresh == kRightWallLimit)
	{
		if (thisGlider->ignoreRight)
		{
			if (thisGlider->dest.right > kNoRightWallLimit)
				MoveRoomToRoom(thisGlider, kToRight);
		}
		else
		{
			if (foilTotal > 0)
				PlayPrioritySound(kFoilHitSound, kFoilHitPriority);
			else
				PlayPrioritySound(kHitWallSound, kHitWallPriority);
			offset = kRightWallLimit - thisGlider->dest.right;
			thisGlider->hVel = -thisGlider->hVel + offset;
		}
	}
	else
		MoveRoomToRoom(thisGlider, kToRight);
}

//--------------------------------------------------------------  CheckGliderInRoom

void CheckGliderInRoom (gliderPtr thisGlider)
{
	if ((thisGlider->mode == kGliderNormal) || 
			(thisGlider->mode == kGliderFaceLeft) || 
			(thisGlider->mode == kGliderFaceRight) || 
			(thisGlider->mode == kGliderBurning))
	{
		if (thisGlider->dest.top < kCeilingLimit)
		{
			if (thisGlider->mode == kGliderBurning)
			{
				thisGlider->wasMode = 0;
				StartGliderFadingOut(thisGlider);
				PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
			}
			else if ((twoPlayerGame) && (!onePlayerLeft))
				CheckEscapeUpTwo(thisGlider);
			else
				CheckEscapeUp(thisGlider);
		}
		else if (thisGlider->dest.bottom > kFloorLimit)
		{
			if (thisGlider->mode == kGliderBurning)
			{
				thisGlider->wasMode = 0;
				StartGliderFadingOut(thisGlider);
				PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
			}
			else if ((twoPlayerGame) && (!onePlayerLeft))
				CheckEscapeDownTwo(thisGlider);
			else
				CheckEscapeDown(thisGlider);
		}
		else if ((thisBackground == kRoof) && (thisGlider->dest.bottom > kRoofLimit))
			CheckRoofCollision(thisGlider);
		
		if (thisGlider->dest.left < leftThresh)
		{
			if (thisGlider->mode == kGliderBurning)
			{
				thisGlider->wasMode = 0;
				StartGliderFadingOut(thisGlider);
				PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
			}
			else if((twoPlayerGame) && (!onePlayerLeft))
				CheckEscapeLeftTwo(thisGlider);
			else
				CheckEscapeLeft(thisGlider);
		}
		else if (thisGlider->dest.right > rightThresh)
		{
			if (thisGlider->mode == kGliderBurning)
			{
				thisGlider->wasMode = 0;
				StartGliderFadingOut(thisGlider);
				PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
			}
			else if ((twoPlayerGame) && (!onePlayerLeft))
				CheckEscapeRightTwo(thisGlider);
			else
				CheckEscapeRight(thisGlider);
		}
	}
}

//--------------------------------------------------------------  HandleRewards

void HandleRewards (gliderPtr thisGlider, hotPtr who)
{
	Rect		bounds;
	short		whoLinked, points;
	
	whoLinked = who->who;
	bounds =  who->bounds;
	
	switch (masterObjects[whoLinked].theObject.what)
	{
		case kRedClock:
		if (SetObjectState(thisRoomNumber, masterObjects[whoLinked].objectNum, 
				0, whoLinked))
		{
			PlayPrioritySound(kBeepsSound, kBeepsPriority);
			RestoreFromSavedMap(thisRoomNumber, 
					masterObjects[whoLinked].objectNum, false);
			AddFlyingPoint(&bounds, 100, thisGlider->hVel / 2, thisGlider->vVel / 2);
			thisGlider->hVel /= 4;
			thisGlider->vVel /= 4;
			theScore += kRedClockPoints;
			RedrawAllGrease();
		}
		who->isOn = false;
		break;
		
		case kBlueClock:
		if (SetObjectState(thisRoomNumber, masterObjects[whoLinked].objectNum, 
				0, whoLinked))
		{
			PlayPrioritySound(kBuzzerSound, kBuzzerPriority);
			RestoreFromSavedMap(thisRoomNumber, 
					masterObjects[whoLinked].objectNum, false);
			AddFlyingPoint(&bounds, 300, thisGlider->hVel / 2, thisGlider->vVel / 2);
			thisGlider->hVel /= 4;
			thisGlider->vVel /= 4;
			theScore += kBlueClockPoints;
			RedrawAllGrease();
		}
		who->isOn = false;
		break;
		
		case kYellowClock:
		if (SetObjectState(thisRoomNumber, masterObjects[whoLinked].objectNum, 
				0, whoLinked))
		{
			PlayPrioritySound(kDingSound, kDingPriority);
			RestoreFromSavedMap(thisRoomNumber, 
					masterObjects[whoLinked].objectNum, false);
			AddFlyingPoint(&bounds, 500, thisGlider->hVel / 2, thisGlider->vVel / 2);
			thisGlider->hVel /= 4;
			thisGlider->vVel /= 4;
			theScore += kYellowClockPoints;
			RedrawAllGrease();
		}
		who->isOn = false;
		break;
		
		case kCuckoo:
		if (SetObjectState(thisRoomNumber, masterObjects[whoLinked].objectNum, 
				0, whoLinked))
		{
			PlayPrioritySound(kCuckooSound, kCuckooPriority);
			RestoreFromSavedMap(thisRoomNumber, 
					masterObjects[whoLinked].objectNum, false);
			StopPendulum(thisRoomNumber, masterObjects[whoLinked].objectNum);
			AddFlyingPoint(&bounds, 1000, thisGlider->hVel / 2, thisGlider->vVel / 2);
			thisGlider->hVel /= 4;
			thisGlider->vVel /= 4;
			theScore += kCuckooClockPoints;
			RedrawAllGrease();
		}
		who->isOn = false;
		break;
		
		case kPaper:
		if (SetObjectState(thisRoomNumber, masterObjects[whoLinked].objectNum, 
				0, whoLinked))
		{
			PlayPrioritySound(kEnergizeSound, kEnergizePriority);
			RestoreFromSavedMap(thisRoomNumber, 
					masterObjects[whoLinked].objectNum, false);
			AddSparkle(&bounds);
			thisGlider->hVel /= 2;
			thisGlider->vVel /= 2;
			mortals++;
			if ((twoPlayerGame) && (!onePlayerLeft))
				mortals++;
			QuickGlidersRefresh();
			RedrawAllGrease();
		}
		who->isOn = false;
		break;
		
		case kBattery:
		if (SetObjectState(thisRoomNumber, masterObjects[whoLinked].objectNum, 
				0, whoLinked))
		{
			PlayPrioritySound(kEnergizeSound, kEnergizePriority);
			RestoreFromSavedMap(thisRoomNumber, 
					masterObjects[whoLinked].objectNum, false);
			AddSparkle(&bounds);
			thisGlider->hVel /= 2;
			thisGlider->vVel /= 2;
			if (batteryTotal > 0)		// positive number means battery power
				batteryTotal += kBatterySupply;
			else						// negative number means helium gas
				batteryTotal = kBatterySupply;
			if ((twoPlayerGame) && (!onePlayerLeft))
				batteryTotal += kBatterySupply;
			QuickBatteryRefresh(false);
			RedrawAllGrease();
		}
		who->isOn = false;
		break;
		
		case kBands:
		if (SetObjectState(thisRoomNumber, masterObjects[whoLinked].objectNum, 
				0, whoLinked))
		{
			PlayPrioritySound(kEnergizeSound, kEnergizePriority);
			RestoreFromSavedMap(thisRoomNumber, 
					masterObjects[whoLinked].objectNum, false);
			AddSparkle(&bounds);
			thisGlider->hVel /= 2;
			thisGlider->vVel /= 2;
			bandsTotal += kBandsSupply;
			if ((twoPlayerGame) && (!onePlayerLeft))
				bandsTotal += kBandsSupply;
			QuickBandsRefresh(false);
			RedrawAllGrease();
		}
		who->isOn = false;
		break;
		
		case kGreaseRt:
		case kGreaseLf:
		if (SetObjectState(thisRoomNumber, masterObjects[whoLinked].objectNum, 
				0, whoLinked))
			SpillGrease(masterObjects[whoLinked].dynaNum, 
					masterObjects[whoLinked].hotNum);
		who->isOn = false;
		break;
		
		case kFoil:
		if (SetObjectState(thisRoomNumber, masterObjects[whoLinked].objectNum, 
				0, whoLinked))
		{
			PlayPrioritySound(kEnergizeSound, kEnergizePriority);
			RestoreFromSavedMap(thisRoomNumber, 
					masterObjects[whoLinked].objectNum, false);
			AddSparkle(&bounds);
			thisGlider->hVel /= 2;
			thisGlider->vVel /= 2;
			foilTotal += kFoilSupply;
			if ((twoPlayerGame) && (!onePlayerLeft))
				foilTotal += kFoilSupply;
			StartGliderFoilGoing(thisGlider);
			RedrawAllGrease();
		}
		who->isOn = false;
		break;
		
		case kInvisBonus:
		points = masterObjects[whoLinked].theObject.data.c.points;
		if (SetObjectState(thisRoomNumber, masterObjects[whoLinked].objectNum, 
				0, whoLinked))
		{
			PlayPrioritySound(kBonusSound, kBonusPriority);
			AddFlyingPoint(&bounds, points, thisGlider->hVel / 2, thisGlider->vVel / 2);
			thisGlider->hVel /= 4;
			thisGlider->vVel /= 4;
			theScore += points;
		}
		who->isOn = false;
		break;
		
		case kStar:
		if (SetObjectState(thisRoomNumber, masterObjects[whoLinked].objectNum, 
				0, whoLinked))
		{
			PlayPrioritySound(kEnergizeSound, kEnergizePriority);
			RestoreFromSavedMap(thisRoomNumber, 
					masterObjects[whoLinked].objectNum, false);
			AddSparkle(&bounds);
			StopStar(thisRoomNumber, masterObjects[whoLinked].objectNum);
			numStarsRemaining--;
			if (numStarsRemaining <= 0)
				FlagGameOver();
			else
				DisplayStarsRemaining();
			RedrawAllGrease();
			theScore += kStarPoints;
		}
		who->isOn = false;
		break;
		
		case kSparkle:
		break;
		
		case kHelium:
		if (SetObjectState(thisRoomNumber, masterObjects[whoLinked].objectNum, 
				0, whoLinked))
		{
			PlayPrioritySound(kEnergizeSound, kEnergizePriority);
			RestoreFromSavedMap(thisRoomNumber, 
					masterObjects[whoLinked].objectNum, false);
			AddSparkle(&bounds);
			thisGlider->hVel /= 2;
			thisGlider->vVel /= 2;
			if (batteryTotal < 0)	// if negative, it is already helium gas
				batteryTotal -= kHeliumSupply;
			else					// if positive, it is battery power
				batteryTotal = -kHeliumSupply;
			if ((twoPlayerGame) && (!onePlayerLeft))
				batteryTotal -= kHeliumSupply;
			QuickBatteryRefresh(false);
			RedrawAllGrease();
		}
		who->isOn = false;
		break;
		
		case kSlider:
		break;
	}
}

//--------------------------------------------------------------  HandleSwitches

void HandleSwitches (hotPtr who)
{
	Rect		newRect, bounds;
	short		whoLinked, roomLinked, objectLinked, linkIndex;
	
	if (who->stillOver)
		return;
	
	whoLinked = who->who;					// what is switch's obj. #
	roomLinked = masterObjects[whoLinked].roomLink;
	objectLinked = masterObjects[whoLinked].objectLink;
	linkIndex = masterObjects[whoLinked].localLink;
											// change state of linked obj.
	if (SetObjectState(roomLinked, objectLinked, 
			masterObjects[whoLinked].theObject.data.e.type, linkIndex))
	{
		newRect = who->bounds;
		QOffsetRect(&newRect, playOriginH, playOriginV);
		switch (masterObjects[whoLinked].theObject.what)
		{
			case kLightSwitch:
			PlayPrioritySound(kSwitchSound, kSwitchPriority);
			DrawLightSwitch(&newRect, newState);
			break;
			
			case kMachineSwitch:
			PlayPrioritySound(kSwitchSound, kSwitchPriority);
			DrawMachineSwitch(&newRect, newState);
			break;
			
			case kThermostat:
			PlayPrioritySound(kSwitchSound, kSwitchPriority);
			DrawThermostat(&newRect, newState);
			break;
			
			case kPowerSwitch:
			PlayPrioritySound(kSwitchSound, kSwitchPriority);
			DrawPowerSwitch(&newRect, newState);
			break;
			
			case kKnifeSwitch:
			PlayPrioritySound(kSwitchSound, kSwitchPriority);
			DrawKnifeSwitch(&newRect, newState);
			break;
			
			case kInvisSwitch:
			break;
		}
		CopyRectBackToWork(&newRect);
		AddRectToWorkRects(&newRect);
		
		if (linkIndex != -1)
		{
			switch (masterObjects[linkIndex].theObject.what)
			{
				case kRedClock:
				case kBlueClock:
				case kYellowClock:
				case kPaper:
				case kBattery:
				case kBands:
				case kFoil:
				case kStar:
				case kHelium:
				RestoreFromSavedMap(roomLinked, objectLinked, true);
				AddSparkle(&bounds);
				break;
				
				case kCuckoo:
				RestoreFromSavedMap(roomLinked, objectLinked, true);
				StopPendulum(roomLinked, objectLinked);
				break;
				
				case kGreaseRt:
				case kGreaseLf:
				SpillGrease(masterObjects[linkIndex].dynaNum, 
						masterObjects[linkIndex].hotNum);
				break;
				
				case kInvisBonus:
				case kSlider:
				break;
				
				case kDeluxeTrans:
				break;
				
				case kSoundTrigger:
				PlayPrioritySound(kTriggerSound, kTriggerPriority);
				break;
				
				case kCeilingLight:
				case kLightBulb:
				case kTableLamp:
				case kHipLamp:
				case kDecoLamp:
				case kFlourescent:
				case kTrackLight:
				case kInvisLight:
				RedrawRoomLighting();
				break;
				
				case kShredder:
				break;
				
				case kToaster:
				ToggleToaster(masterObjects[linkIndex].dynaNum);
				break;
				
				case kMacPlus:
				ToggleMacPlus(masterObjects[linkIndex].dynaNum);
				break;
				
				case kGuitar:
				PlayPrioritySound(kChordSound, kChordPriority);
				break;
				
				case kTV:
				ToggleTV(masterObjects[linkIndex].dynaNum);
				break;
				
				case kCoffee:
				ToggleCoffee(masterObjects[linkIndex].dynaNum);
				break;
				
				case kOutlet:
				ToggleOutlet(masterObjects[linkIndex].dynaNum);
				break;
				
				case kVCR:
				ToggleVCR(masterObjects[linkIndex].dynaNum);
				break;
				
				case kStereo:
				ToggleStereos(masterObjects[linkIndex].dynaNum);
				break;
				
				case kMicrowave:
				ToggleMicrowave(masterObjects[linkIndex].dynaNum);
				break;
				
				case kBalloon:
				ToggleBalloon(masterObjects[linkIndex].dynaNum);
				break;
				
				case kCopterLf:
				case kCopterRt:
				ToggleCopter(masterObjects[linkIndex].dynaNum);
				break;
				
				case kDartLf:
				case kDartRt:
				ToggleDart(masterObjects[linkIndex].dynaNum);
				break;
				
				case kBall:
				ToggleBall(masterObjects[linkIndex].dynaNum);
				break;
				
				case kDrip:
				ToggleDrip(masterObjects[linkIndex].dynaNum);
				break;
				
				case kFish:
				ToggleFish(masterObjects[linkIndex].dynaNum);
				break;
			}
		}
	}
	
	who->stillOver = true;
}

//--------------------------------------------------------------  HandleMicrowaveAction

void HandleMicrowaveAction (hotPtr who, gliderPtr thisGlider)
{
	short		whoLinked, kills;
	Boolean		killed;
	
	if (who->stillOver)
		return;
	
	killed = false;
	whoLinked = who->who;					// what is microwave's obj. #
	if (masterObjects[whoLinked].theObject.data.g.state)
	{
		kills = (short)masterObjects[whoLinked].theObject.data.g.byte0;
		if (((kills & 0x0001) == 0x0001) && (bandsTotal > 0))
		{
			bandsTotal = 0;
			killed = true;
			QuickBandsRefresh(false);
		}
		if (((kills & 0x0002) == 0x0002) && (batteryTotal != 0))
		{
			batteryTotal = 0;
			killed = true;
			QuickBatteryRefresh(false);
		}
		if (((kills & 0x0004) == 0x0004) && (foilTotal > 0))
		{
			foilTotal = 0;
			killed = true;
			StartGliderFoilLosing(thisGlider);
		}
	}
	
	if (killed)
		PlayPrioritySound(kMicrowavedSound, kMicrowavedPriority);
}

//--------------------------------------------------------------  HandleHotSpotCollision

void HandleHotSpotCollision (gliderPtr thisGlider, hotPtr who, short index)
{
	switch (who->action)
	{
		case kLiftIt:
		thisGlider->vDesiredVel = kFloorVentLift;
		break;
		
		case kDropIt:
		thisGlider->vDesiredVel = kCeilingVentDrop;
		break;
		
		case kPushItLeft:
		thisGlider->hDesiredVel += -kFanStrength;
		break;
		
		case kPushItRight:
		thisGlider->hDesiredVel += kFanStrength;
		break;
		
		case kDissolveIt:
		if (thisGlider->mode != kGliderFadingOut)
		{
			if ((foilTotal > 0) || (thisGlider->mode == kGliderLosingFoil))
			{
				if (GliderHitTop(thisGlider, &(who->bounds)))
				{
					StartGliderFadingOut(thisGlider);
					PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
				}
				else
				{
					if (foilTotal > 0)
					{
						foilTotal--;
						if (foilTotal <= 0)
							StartGliderFoilLosing(thisGlider);
					}
				}
			}
			else
			{
				StartGliderFadingOut(thisGlider);
				PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
			}
		}
		break;
		
		case kRewardIt:
		HandleRewards(thisGlider, who);
		break;
		
		case kMoveItUp:
		if (!thisGlider->heldRight && GliderInRect(thisGlider, &who->bounds))
		{
			if (thisGlider->mode == kGliderBurning)
			{
				thisGlider->wasMode = 0;
				StartGliderFadingOut(thisGlider);
				PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
			}
			else if ((twoPlayerGame) && (!onePlayerLeft))
			{
				if (otherPlayerEscaped == kNoOneEscaped)
				{
					if ((thisGlider->mode != kGliderGoingUp) && 
							(thisGlider->mode != kGliderInLimbo))
					{
						otherPlayerEscaped = kPlayerEscapingUpStairs;
						RefreshScoreboard(kEscapedTitleMode);
						StartGliderGoingUpStairs(thisGlider);
					}
				}
				else if (otherPlayerEscaped == kPlayerEscapedUpStairs)
				{
					if ((thisGlider->mode != kGliderGoingUp) && 
							(thisGlider->mode != kGliderInLimbo))
					{
						StartGliderGoingUpStairs(thisGlider);
					}
				}
			}
			else
				StartGliderGoingUpStairs(thisGlider);
		}
		break;
		
		case kMoveItDown:
		if (!thisGlider->heldLeft && GliderInRect(thisGlider, &who->bounds))
		{
			if (thisGlider->mode == kGliderBurning)
			{
				thisGlider->wasMode = 0;
				StartGliderFadingOut(thisGlider);
				PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
			}
			else if ((twoPlayerGame) && (!onePlayerLeft))
			{
				if (otherPlayerEscaped == kNoOneEscaped)
				{
					if ((thisGlider->mode != kGliderGoingDown) && 
							(thisGlider->mode != kGliderInLimbo))
					{
						otherPlayerEscaped = kPlayerEscapingDownStairs;
						RefreshScoreboard(kEscapedTitleMode);
						StartGliderGoingDownStairs(thisGlider);
					}
				}
				else if (otherPlayerEscaped == kPlayerEscapedDownStairs)
				{
					if ((thisGlider->mode != kGliderGoingDown) && 
							(thisGlider->mode != kGliderInLimbo))
					{
						StartGliderGoingDownStairs(thisGlider);
					}
				}
			}
			else
				StartGliderGoingDownStairs(thisGlider);
		}
		break;
		
		case kSwitchIt:
		HandleSwitches(who);
		break;
		
		case kShredIt:
		if ((thisGlider->mode != kGliderShredding) && 
				(GliderInRect(thisGlider, &who->bounds)))
		{
			if ((foilTotal > 0) || (thisGlider->mode == kGliderLosingFoil))
			{
				PlayPrioritySound(kFoilHitSound, kFoilHitPriority);
				if (foilTotal > 0)
				{
					foilTotal--;
					if (foilTotal <= 0)
						StartGliderFoilLosing(thisGlider);
				}
			}
			else
				FlagGliderShredding(thisGlider, &who->bounds);
		}
		break;
		
		case kStrumIt:
		if (!who->stillOver)
		{
			PlayPrioritySound(kChordSound, kChordPriority);
			who->stillOver = true;
		}
		break;
		
		case kTriggerIt:
		case kLgTrigger:
		ArmTrigger(who);
		break;
		
		case kBurnIt:
		if ((thisGlider->mode != kGliderBurning) && 
				(thisGlider->mode != kGliderFadingOut))
		{
			if ((foilTotal > 0) || (thisGlider->mode == kGliderLosingFoil))
			{
				thisGlider->vDesiredVel = kFloorVentLift;
				if (foilTotal > 0)
				{
					PlayPrioritySound(kSizzleSound, kSizzlePriority);
					foilTotal--;
					if (foilTotal <= 0)
						StartGliderFoilLosing(thisGlider);
				}
			}
			else
				FlagGliderBurning(thisGlider);
		}
		break;
		
		case kSlideIt:
		thisGlider->sliding = true;
		thisGlider->vVel = who->bounds.top - thisGlider->dest.bottom;
		break;
		
		case kTransportIt:
		if (thisGlider->mode == kGliderBurning)
		{
			thisGlider->wasMode = 0;
			StartGliderFadingOut(thisGlider);
			PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
		}
		else if ((GliderInRect(thisGlider, &who->bounds)) && 
				(thisGlider->mode != kGliderTransporting) && 
				(thisGlider->mode != kGliderFadingOut))
		{
			if ((twoPlayerGame) && (!onePlayerLeft))
			{
				if (otherPlayerEscaped == kNoOneEscaped)
				{
					if (thisGlider->mode != kGliderInLimbo)
					{
						activeRectEscaped = index;
						StartGliderTransporting(thisGlider, who);
					}
				}
				else if (otherPlayerEscaped == kPlayerTransportedOut)
				{
					if ((thisGlider->mode != kGliderInLimbo) && 
							(activeRectEscaped == index))
					{
						StartGliderTransporting(thisGlider, who);
					}
				}
			}
			else
				StartGliderTransporting(thisGlider, who);
		}
		break;
		
		case kIgnoreLeftWall:
		thisGlider->ignoreLeft = true;
		break;
		
		case kIgnoreRightWall:
		thisGlider->ignoreRight = true;
		break;
		
		case kMailItLeft:									// mailbox open to right
		if (thisGlider->mode == kGliderBurning)
		{
			thisGlider->wasMode = 0;
			StartGliderFadingOut(thisGlider);
			PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
		}
		else if ((GliderInRect(thisGlider, &who->bounds)) && 
				(thisGlider->mode != kGliderMailOutRight) && 
				(thisGlider->mode != kGliderMailInLeft) && 
				(thisGlider->mode != kGliderFadingOut) && 
				(((thisGlider->facing == kFaceRight) && (!thisGlider->tipped)) || 
				((thisGlider->facing == kFaceLeft) && (thisGlider->tipped))))
		{
			if ((twoPlayerGame) && (!onePlayerLeft))		// two gliders to handle
			{
				if (otherPlayerEscaped == kNoOneEscaped)	// other glider in room
				{
					if (thisGlider->mode != kGliderInLimbo)	// this glider is active
					{
						activeRectEscaped = index;
						StartGliderMailingIn(thisGlider, &who->bounds, who);
						thisGlider->mode = kGliderMailInLeft;
					}
				}
				else if (otherPlayerEscaped == kPlayerMailedOut)
				{											// other glider left here
					if ((thisGlider->mode != kGliderInLimbo) && 
							(activeRectEscaped == index))
					{										// []_ <--G
						StartGliderMailingIn(thisGlider, &who->bounds, who);
						thisGlider->mode = kGliderMailInLeft;
					}
				}
			}
			else											// only 1 glider in game
			{
				StartGliderMailingIn(thisGlider, &who->bounds, who);
				thisGlider->mode = kGliderMailInLeft;
			}
		}
		break;
		
		case kMailItRight:									// mailbox open to left
		if (thisGlider->mode == kGliderBurning)
		{
			thisGlider->wasMode = 0;
			StartGliderFadingOut(thisGlider);
			PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
		}
		else if ((GliderInRect(thisGlider, &who->bounds)) && 
				(thisGlider->mode != kGliderMailOutLeft) && 
				(thisGlider->mode != kGliderMailInRight) && 
				(thisGlider->mode != kGliderFadingOut) && 
				(((thisGlider->facing == kFaceRight) && (thisGlider->tipped)) || 
				((thisGlider->facing == kFaceLeft) && (!thisGlider->tipped))))
		{
			if ((twoPlayerGame) && (!onePlayerLeft))
			{
				if (otherPlayerEscaped == kNoOneEscaped)
				{
					if (thisGlider->mode != kGliderInLimbo)
					{
						activeRectEscaped = index;
						StartGliderMailingIn(thisGlider, &who->bounds, who);
						thisGlider->mode = kGliderMailInRight;
					}
				}
				else if (otherPlayerEscaped == kPlayerMailedOut)
				{
					if ((thisGlider->mode != kGliderInLimbo) && 
							(activeRectEscaped == index))
					{
						StartGliderMailingIn(thisGlider, &who->bounds, who);
						thisGlider->mode = kGliderMailInRight;
					}
				}
			}
			else
			{
				StartGliderMailingIn(thisGlider, &who->bounds, who);
				thisGlider->mode = kGliderMailInRight;
			}
		}
		break;
		
		case kDuctItDown:
		if (thisGlider->mode == kGliderBurning)
		{
			thisGlider->wasMode = 0;
			StartGliderFadingOut(thisGlider);
			PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
		}
		else if ((GliderInRect(thisGlider, &who->bounds)) && 
				(thisGlider->mode != kGliderDuctingDown) && 
				(thisGlider->mode != kGliderFadingOut))
		{
			if ((twoPlayerGame) && (!onePlayerLeft))
			{
				if (otherPlayerEscaped == kNoOneEscaped)
				{
					if (thisGlider->mode != kGliderInLimbo)
					{
						activeRectEscaped = index;
						StartGliderDuctingDown(thisGlider, &who->bounds, who);
					}
				}
				else if (otherPlayerEscaped == kPlayerDuckedOut)
				{
					if ((thisGlider->mode != kGliderInLimbo) && 
							(activeRectEscaped == index))
						StartGliderDuctingDown(thisGlider, &who->bounds, who);
				}
			}
			else
				StartGliderDuctingDown(thisGlider, &who->bounds, who);
		}
		break;
		
		case kDuctItUp:
		if (thisGlider->mode == kGliderBurning)
		{
			thisGlider->wasMode = 0;
			StartGliderFadingOut(thisGlider);
			PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
		}
		else if ((GliderInRect(thisGlider, &who->bounds)) && 
				(thisGlider->mode != kGliderDuctingUp) && 
				(thisGlider->mode != kGliderDuctingIn) && 
				(thisGlider->mode != kGliderFadingOut) && 
				(!who->stillOver))
		{
			if ((twoPlayerGame) && (!onePlayerLeft))
			{
				if (otherPlayerEscaped == kNoOneEscaped)
				{
					if (thisGlider->mode != kGliderInLimbo)
					{
						activeRectEscaped = index;
						StartGliderDuctingUp(thisGlider, &who->bounds, who);
					}
				}
				else if (otherPlayerEscaped == kPlayerDuckedOut)
				{
					if ((thisGlider->mode != kGliderInLimbo) && 
							(activeRectEscaped == index))
						StartGliderDuctingUp(thisGlider, &who->bounds, who);
				}
			}
			else
			{
				StartGliderDuctingUp(thisGlider, &who->bounds, who);
				who->stillOver = true;
			}
		}
		break;
		
		case kMicrowaveIt:
		if (GliderInRect(thisGlider, &who->bounds))
			HandleMicrowaveAction(who, thisGlider);
		break;
		
		case kIgnoreGround:
		thisGlider->ignoreGround = true;
		break;
		
		case kBounceIt:
		BounceGlider(thisGlider, &who->bounds);
		break;
		
		case kChimeIt:
		if (!who->stillOver)
		{
			StrikeChime();
			who->stillOver = true;
		}
		break;
		
		case kWebIt:
		if ((GliderInRect(thisGlider, &who->bounds)) && 
				(thisGlider->mode != kGliderBurning))
			WebGlider(thisGlider, &who->bounds);
		else if ((thisGlider->mode == kGliderBurning) && 
				(GliderInRect(thisGlider, &who->bounds)))
		{
			thisGlider->wasMode = 0;
			StartGliderFadingOut(thisGlider);
			PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
		}
		break;
		
		case kSoundIt:
		if (!who->stillOver)
		{
			PlayPrioritySound(kTriggerSound, kTriggerPriority);
			who->stillOver = true;
		}
		break;
	}
}

//--------------------------------------------------------------  CheckForHotSpots

void CheckForHotSpots (void)
{
	short		i;
	Boolean		hitObject;
	
	for (i = 0; i < nHotSpots; i++)
	{
		if (hotSpots[i].isOn)
		{
			if (twoPlayerGame)
			{
				hitObject = false;
				if (SectGlider(&theGlider, &hotSpots[i].bounds, 
						hotSpots[i].doScrutinize))
				{
					if (onePlayerLeft)
					{
						if (playerDead == kPlayer2)
						{
							HandleHotSpotCollision(&theGlider, &hotSpots[i], i);
							hitObject = true;
						}
					}
					else
					{
						HandleHotSpotCollision(&theGlider, &hotSpots[i], i);
						hitObject = true;
					}
				}
				
				if (SectGlider(&theGlider2, &hotSpots[i].bounds, 
						hotSpots[i].doScrutinize))
				{
					if (onePlayerLeft)
					{
						if (playerDead == kPlayer1)
						{
							HandleHotSpotCollision(&theGlider2, &hotSpots[i], i);
							hitObject = true;
						}
					}
					else
					{
						HandleHotSpotCollision(&theGlider2, &hotSpots[i], i);
						hitObject = true;
					}
				}
				if (!hitObject)
					hotSpots[i].stillOver = false;
			}
			else
			{
				if (SectGlider(&theGlider, &hotSpots[i].bounds, 
						hotSpots[i].doScrutinize))
					HandleHotSpotCollision(&theGlider, &hotSpots[i], i);
				else
					hotSpots[i].stillOver = false;
			}
		}
	}
}

//--------------------------------------------------------------  HandleInteraction

void HandleInteraction (void)
{
	CheckForHotSpots();
	if (twoPlayerGame)
	{
		if (onePlayerLeft)
		{
			if (playerDead == kPlayer1)
				CheckGliderInRoom(&theGlider2);
			else
				CheckGliderInRoom(&theGlider);
		}
		else
		{
			CheckGliderInRoom(&theGlider);
			CheckGliderInRoom(&theGlider2);
		}
	}
	else
		CheckGliderInRoom(&theGlider);
}

//--------------------------------------------------------------  FlagStillOvers

void FlagStillOvers (gliderPtr thisGlider)
{
	short		i;
	
	for (i = 0; i < nHotSpots; i++)
	{
		if (hotSpots[i].isOn)
		{
			if (SectGlider(thisGlider, &hotSpots[i].bounds, 
					hotSpots[i].doScrutinize))
				hotSpots[i].stillOver = true;
			else
				hotSpots[i].stillOver = false;
		}
		else
			hotSpots[i].stillOver = false;
	}
}

//--------------------------------------------------------------  WebGlider

void WebGlider (gliderPtr thisGlider, Rect *webBounds)
{
	#define		kKillWebbedGlider	150
	short		hDist, vDist;
	
	if ((thisGlider->mode == kGliderBurning) && (GliderInRect(thisGlider, webBounds)))
	{
		thisGlider->wasMode = 0;
		StartGliderFadingOut(thisGlider);
		PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
		return;
	}
	
	hDist = ((webBounds->right - thisGlider->dest.right) + 
			(webBounds->left - thisGlider->dest.left)) >> 3;
	vDist = ((webBounds->bottom - thisGlider->dest.bottom) + 
			(webBounds->top - thisGlider->dest.top)) >> 3;
	
	if (thisGlider->hDesiredVel != 0)
	{
		if (evenFrame)
		{
			thisGlider->hVel = hDist;
			thisGlider->vVel = vDist;
			PlayPrioritySound(kWebTwangSound, kWebTwangPriority);
		}
	}
	else
	{
		thisGlider->hDesiredVel = 0;
		thisGlider->vDesiredVel = 0;
	}
	
	thisGlider->wasMode++;
	if (thisGlider->wasMode >= kKillWebbedGlider)
	{
		thisGlider->wasMode = 0;
		StartGliderFadingOut(thisGlider);
		PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
	}
}

