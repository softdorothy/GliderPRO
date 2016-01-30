//============================================================================
//----------------------------------------------------------------------------
//									Player.c
//----------------------------------------------------------------------------
//============================================================================


#include "Externs.h"
#include "Play.h"
#include "RectUtils.h"


#define kGravity				3
#define kHImpulse				2
#define kVImpulse				2
#define kMaxHVel				16
#define kShredderCountdown		-68


void MoveGlider (gliderPtr);
void MoveGliderNormal (gliderPtr);
void MoveGliderBurning (gliderPtr);
void FadeGliderIn (gliderPtr);
void TransportGliderIn (gliderPtr);
void FadeGliderOut (gliderPtr);
void MoveGliderUpStairs (gliderPtr);
void MoveGliderDownStairs (gliderPtr);
void MoveGliderFaceLeft (gliderPtr);
void MoveGliderFaceRight (gliderPtr);
void TransportGliderOut (gliderPtr);
void MoveGliderDownDuct (gliderPtr);
void MoveGliderUpDuct (gliderPtr);
void MoveGliderInMailLeft (gliderPtr);
void MoveGliderInMailRight (gliderPtr);
void FinishGliderMailingLeft (gliderPtr);
void FinishGliderMailingRight (gliderPtr);
void MoveGliderFoilGoing (gliderPtr);
void MoveGliderFoilLosing (gliderPtr);
void MoveGliderShredding (gliderPtr);
void HandleIdleGlider (gliderPtr);


gliderType	theGlider, theGlider2;
Rect		shadowSrcRect;
GWorldPtr	shadowSrcMap;
GWorldPtr	shadowMaskMap;
Rect		shadowSrc[kNumShadowSrcRects];
Rect		gliderSrc[kNumGliderSrcRects];
Rect		transRect;
long		theScore;
short		fadeInSequence[kLastFadeSequence];
short		rightClip, leftClip, transRoom;
Boolean		shadowVisible, onePlayerLeft, playerDead;
				

extern	short		numShredded, otherPlayerEscaped;
extern	Boolean		playing, twoPlayerGame, gameOver, hasMirror;
extern	Boolean		takingTheStairs, playerSuicide;


//==============================================================  Functions
//--------------------------------------------------------------  MoveGlider

void MoveGlider (gliderPtr thisGlider)
{
	if (thisGlider->hVel > thisGlider->hDesiredVel)
	{
		thisGlider->hVel -= kHImpulse;
		if (thisGlider->hVel < thisGlider->hDesiredVel)
			thisGlider->hVel = thisGlider->hDesiredVel;
	}
	else if (thisGlider->hVel < thisGlider->hDesiredVel)
	{
		thisGlider->hVel += kHImpulse;
		if (thisGlider->hVel > thisGlider->hDesiredVel)
			thisGlider->hVel = thisGlider->hDesiredVel;
	}
	thisGlider->hDesiredVel = 0;
	
	if (thisGlider->vVel > thisGlider->vDesiredVel)
	{
		thisGlider->vVel -= kVImpulse;
		if (thisGlider->vVel < thisGlider->vDesiredVel)
			thisGlider->vVel = thisGlider->vDesiredVel;
	}
	else if (thisGlider->vVel < thisGlider->vDesiredVel)
	{
		thisGlider->vVel += kVImpulse;
		if (thisGlider->vVel > thisGlider->vDesiredVel)
			thisGlider->vVel = thisGlider->vDesiredVel;
	}
	thisGlider->vDesiredVel = kGravity;
	
	if (thisGlider->hVel < 0)
	{
		if (thisGlider->hVel < -kMaxHVel)
			thisGlider->hVel = -kMaxHVel;
		
		thisGlider->wasHVel = thisGlider->hVel;
		
		thisGlider->whole.right = thisGlider->dest.right;
		thisGlider->dest.left += thisGlider->hVel;
		thisGlider->dest.right += thisGlider->hVel;
		thisGlider->whole.left = thisGlider->dest.left;
		
		thisGlider->wholeShadow.right = thisGlider->destShadow.right;
		thisGlider->destShadow.left += thisGlider->hVel;
		thisGlider->destShadow.right += thisGlider->hVel;
		thisGlider->wholeShadow.left = thisGlider->destShadow.left;
	}
	else
	{
		if (thisGlider->hVel > kMaxHVel)
			thisGlider->hVel = kMaxHVel;
		
		thisGlider->wasHVel = thisGlider->hVel;
		
		thisGlider->whole.left = thisGlider->dest.left;
		thisGlider->dest.left += thisGlider->hVel;
		thisGlider->dest.right += thisGlider->hVel;
		thisGlider->whole.right = thisGlider->dest.right;
		
		thisGlider->wholeShadow.left = thisGlider->destShadow.left;
		thisGlider->destShadow.left += thisGlider->hVel;
		thisGlider->destShadow.right += thisGlider->hVel;
		thisGlider->wholeShadow.right = thisGlider->destShadow.right;
	}
	
	if (thisGlider->vVel < 0)
	{
		thisGlider->wasVVel = thisGlider->vVel;
		
		thisGlider->whole.bottom = thisGlider->dest.bottom;
		thisGlider->dest.top += thisGlider->vVel;
		thisGlider->dest.bottom += thisGlider->vVel;
		thisGlider->whole.top = thisGlider->dest.top;
	}
	else
	{
		thisGlider->wasVVel = thisGlider->vVel;
		
		thisGlider->whole.top = thisGlider->dest.top;
		thisGlider->dest.top += thisGlider->vVel;
		thisGlider->dest.bottom += thisGlider->vVel;
		thisGlider->whole.bottom = thisGlider->dest.bottom;
	}
}

//--------------------------------------------------------------  MoveGliderNormal

void MoveGliderNormal (gliderPtr thisGlider)
{
	if (thisGlider->facing == kFaceLeft)
	{
		if (thisGlider->sliding)
		{
			thisGlider->src = gliderSrc[30];
			thisGlider->mask = gliderSrc[30];
			thisGlider->sliding = false;
		}
		else
		{
			if (thisGlider->tipped)
			{
				thisGlider->src = gliderSrc[3];
				thisGlider->mask = gliderSrc[3];
			}
			else
			{
				thisGlider->src = gliderSrc[2];
				thisGlider->mask = gliderSrc[2];
			}
		}
	}
	else
	{
		if (thisGlider->sliding)
		{
			thisGlider->src = gliderSrc[29];
			thisGlider->mask = gliderSrc[29];
			thisGlider->sliding = false;
		}
		else
		{
			if (thisGlider->tipped)
			{
				thisGlider->src = gliderSrc[1];
				thisGlider->mask = gliderSrc[1];
			}
			else
			{
				thisGlider->src = gliderSrc[0];
				thisGlider->mask = gliderSrc[0];
			}
		}
	}
	
	MoveGlider(thisGlider);
}

//--------------------------------------------------------------  MoveGliderBurning

void MoveGliderBurning (gliderPtr thisGlider)
{
	thisGlider->frame++;
	if (thisGlider->frame > 3)
		thisGlider->frame = 0;
	
	if (thisGlider->facing == kFaceLeft)
	{
		thisGlider->src = gliderSrc[25 + thisGlider->frame];
		thisGlider->mask = gliderSrc[25 + thisGlider->frame];
	}
	else
	{
		thisGlider->src = gliderSrc[21 + thisGlider->frame];
		thisGlider->mask = gliderSrc[21 + thisGlider->frame];
	}
	
	thisGlider->wasMode--;
	if (thisGlider->wasMode <= 0)
	{
		StartGliderFadingOut(thisGlider);
		PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
	}
	MoveGlider(thisGlider);
}

//--------------------------------------------------------------  FadeGliderIn

void FadeGliderIn (gliderPtr thisGlider)
{
	if (thisGlider->frame == 0)
		PlayPrioritySound(kFadeInSound, kFadeInPriority);
	
	thisGlider->frame++;
	if (thisGlider->frame >= kLastFadeSequence)
	{
		FlagGliderNormal(thisGlider);
		thisGlider->enteredRect = thisGlider->dest;
	}
	else
	{
		if (thisGlider->facing == kFaceLeft)
		{
			thisGlider->src = gliderSrc[fadeInSequence[thisGlider->frame] + 
					kLeftFadeOffset];
			thisGlider->mask = gliderSrc[fadeInSequence[thisGlider->frame] + 
					kLeftFadeOffset];
		}
		else
		{
			thisGlider->src = gliderSrc[fadeInSequence[thisGlider->frame]];
			thisGlider->mask = gliderSrc[fadeInSequence[thisGlider->frame]];
		}
	}
}

//--------------------------------------------------------------  TransportGliderIn

void TransportGliderIn (gliderPtr thisGlider)
{
	if (thisGlider->frame == 0)
		PlayPrioritySound(kTransInSound, kTransInPriority);
	
	thisGlider->frame++;
	if (thisGlider->frame >= kLastFadeSequence)
	{
		FlagGliderNormal(thisGlider);
		thisGlider->enteredRect = thisGlider->dest;
	}
	else
	{
		if (thisGlider->facing == kFaceLeft)
		{
			thisGlider->src = gliderSrc[fadeInSequence[thisGlider->frame] + 
					kLeftFadeOffset];
			thisGlider->mask = gliderSrc[fadeInSequence[thisGlider->frame] + 
					kLeftFadeOffset];
		}
		else
		{
			thisGlider->src = gliderSrc[fadeInSequence[thisGlider->frame]];
			thisGlider->mask = gliderSrc[fadeInSequence[thisGlider->frame]];
		}
	}
}

//--------------------------------------------------------------  FadeGliderOut

void FadeGliderOut (gliderPtr thisGlider)
{
	thisGlider->frame--;
	if (thisGlider->frame < 0)
		OffAMortal(thisGlider);
	else
	{
		if (thisGlider->facing == kFaceLeft)
		{
			thisGlider->src = gliderSrc[fadeInSequence[thisGlider->frame] + 
					kLeftFadeOffset];
			thisGlider->mask = gliderSrc[fadeInSequence[thisGlider->frame] + 
					kLeftFadeOffset];
		}
		else
		{
			thisGlider->src = gliderSrc[fadeInSequence[thisGlider->frame]];
			thisGlider->mask = gliderSrc[fadeInSequence[thisGlider->frame]];
		}
	}
}

//--------------------------------------------------------------  MoveGliderUpStairs

void MoveGliderUpStairs (gliderPtr thisGlider)
{
	#define		kClimbStairsSpeed	-4
	short		vNotClipped;
	
	if (thisGlider->facing == kFaceLeft)
	{
		thisGlider->src = gliderSrc[2];
		thisGlider->mask = gliderSrc[2];
	}
	else
	{
		thisGlider->src = gliderSrc[1];
		thisGlider->mask = gliderSrc[1];
	}
	
	thisGlider->whole.bottom = thisGlider->dest.bottom;
	thisGlider->dest.top += kClimbStairsSpeed;
	thisGlider->dest.bottom += kClimbStairsSpeed;
	thisGlider->whole.top = thisGlider->dest.top;
	
	vNotClipped = thisGlider->dest.bottom - 29;
	if (vNotClipped < kGliderHigh)
	{
		if (vNotClipped <= 0)
		{
			thisGlider->dest.top = thisGlider->dest.bottom;
			thisGlider->src.top = thisGlider->src.bottom;
			thisGlider->mask.top = thisGlider->mask.bottom;
			takingTheStairs = true;
			if (twoPlayerGame)
			{
				if (onePlayerLeft)
				{
					if (playerDead == kPlayer1)
						MoveRoomToRoom(&theGlider2, kAbove);
					else
						MoveRoomToRoom(&theGlider, kAbove);
				}
				else
				{
					if (otherPlayerEscaped == kPlayerEscapedUpStairs)
					{
						otherPlayerEscaped = kNoOneEscaped;
						MoveRoomToRoom(thisGlider, kAbove);
					}
					else
					{
						otherPlayerEscaped = kPlayerEscapedUpStairs;
						RefreshScoreboard(kEscapedTitleMode);
						FlagGliderInLimbo(thisGlider, true);
					}
				}
			}
			else
				MoveRoomToRoom(thisGlider, kAbove);
		}
		else
		{
			thisGlider->dest.top = thisGlider->dest.bottom - vNotClipped;
			thisGlider->src.top = thisGlider->src.bottom - vNotClipped;
			thisGlider->mask.top = thisGlider->mask.bottom - vNotClipped;
		}
	}
}

//--------------------------------------------------------------  FinishGliderUpStairs

void FinishGliderUpStairs (gliderPtr thisGlider)
{
	#define		kVClimbStairsSpeed	-4
	#define		kHClimbStairsSpeed	-4
	short		hNotClipped;
	
	thisGlider->src = gliderSrc[2];
	thisGlider->mask = gliderSrc[2];
	
	thisGlider->whole.bottom = thisGlider->dest.bottom;
	thisGlider->dest.top += kVClimbStairsSpeed;
	thisGlider->dest.bottom += kVClimbStairsSpeed;
	thisGlider->whole.top = thisGlider->dest.top;
	
	thisGlider->whole.right = thisGlider->dest.right;
	thisGlider->dest.left += kHClimbStairsSpeed;
	thisGlider->dest.right += kHClimbStairsSpeed;
	thisGlider->whole.left = thisGlider->dest.left;
	
	thisGlider->wholeShadow.right = thisGlider->destShadow.right;
	thisGlider->destShadow.left += kHClimbStairsSpeed;
	thisGlider->destShadow.right += kHClimbStairsSpeed;
	thisGlider->wholeShadow.left = thisGlider->destShadow.left;
	
	hNotClipped = rightClip - thisGlider->dest.left;
	if (hNotClipped < kGliderWide)
	{
		thisGlider->dest.right = thisGlider->dest.left + hNotClipped;
		thisGlider->src.right = thisGlider->src.left + hNotClipped;
		thisGlider->mask.right = thisGlider->mask.left + hNotClipped;
		thisGlider->destShadow.right = thisGlider->dest.right;
	}
	else
	{
		if (thisGlider->frame == kWasBurning)
			FlagGliderBurning(thisGlider);
		else
			FlagGliderNormal(thisGlider);
		thisGlider->hVel = kHClimbStairsSpeed;
		thisGlider->hDesiredVel = kHClimbStairsSpeed;
		thisGlider->vVel = kVClimbStairsSpeed;
		thisGlider->vDesiredVel = kVClimbStairsSpeed;
		thisGlider->enteredRect = thisGlider->dest;
	}
}

//--------------------------------------------------------------  MoveGliderDownStairs

void MoveGliderDownStairs (gliderPtr thisGlider)
{
	#define		kVDropStairsSpeed	4
	#define		kHDropStairsSpeed	4
	short		hNotClipped;
	
	if (thisGlider->facing == kFaceLeft)
	{
		thisGlider->src = gliderSrc[2];
		thisGlider->mask = gliderSrc[2];
	}
	else
	{
		thisGlider->src = gliderSrc[0];
		thisGlider->mask = gliderSrc[0];
	}
	
	thisGlider->whole.left = thisGlider->dest.left;
	thisGlider->dest.left += kHDropStairsSpeed;
	thisGlider->dest.right += kHDropStairsSpeed;
	thisGlider->whole.right = thisGlider->dest.right;
	
	thisGlider->wholeShadow.left = thisGlider->destShadow.left;
	thisGlider->destShadow.left += kHDropStairsSpeed;
	thisGlider->destShadow.right += kHDropStairsSpeed;
	thisGlider->wholeShadow.right = thisGlider->destShadow.right;
	
	thisGlider->whole.top = thisGlider->dest.top;
	thisGlider->dest.top += kVDropStairsSpeed;
	thisGlider->dest.bottom += kVDropStairsSpeed;
	thisGlider->whole.bottom = thisGlider->dest.bottom;
	
	hNotClipped = rightClip - thisGlider->dest.left;
	if (hNotClipped < kGliderWide)
	{
		if (hNotClipped <= 0)
		{
			thisGlider->dest.right = thisGlider->dest.left;
			thisGlider->src.right = thisGlider->src.left;
			thisGlider->mask.right = thisGlider->mask.left;
			thisGlider->destShadow.right = thisGlider->dest.right;
			takingTheStairs = true;
			if (twoPlayerGame)
			{
				if (onePlayerLeft)
				{
					if (playerDead == kPlayer1)
						MoveRoomToRoom(&theGlider2, kBelow);
					else
						MoveRoomToRoom(&theGlider, kBelow);
				}
				else
				{
					if (otherPlayerEscaped == kPlayerEscapedDownStairs)
					{
						otherPlayerEscaped = kNoOneEscaped;
						MoveRoomToRoom(thisGlider, kBelow);
					}
					else
					{
						otherPlayerEscaped = kPlayerEscapedDownStairs;
						RefreshScoreboard(kEscapedTitleMode);
						FlagGliderInLimbo(thisGlider, true);
					}
				}
			}
			else
				MoveRoomToRoom(thisGlider, kBelow);
		}
		else
		{
			thisGlider->dest.right = thisGlider->dest.left + hNotClipped;
			thisGlider->src.right = thisGlider->src.left + hNotClipped;
			thisGlider->mask.right = thisGlider->mask.left + hNotClipped;
			thisGlider->destShadow.right = thisGlider->dest.right;
		}
	}
}

//--------------------------------------------------------------  FinishGliderDownStairs

void FinishGliderDownStairs (gliderPtr thisGlider)
{
	#define		kVDropStairsSpeed	4
	#define		kHDropStairsSpeed	4
	short		hNotClipped;
	
	thisGlider->src = gliderSrc[0];
	thisGlider->mask = gliderSrc[0];
	
	thisGlider->whole.left = thisGlider->dest.left;
	thisGlider->dest.left += kHDropStairsSpeed;
	thisGlider->dest.right += kHDropStairsSpeed;
	thisGlider->whole.right = thisGlider->dest.right;
	
	thisGlider->wholeShadow.left = thisGlider->destShadow.left;
	thisGlider->destShadow.left += kHDropStairsSpeed;
	thisGlider->destShadow.right += kHDropStairsSpeed;
	thisGlider->wholeShadow.right = thisGlider->destShadow.right;
	
	thisGlider->whole.top = thisGlider->dest.top;
	thisGlider->dest.top += kVDropStairsSpeed;
	thisGlider->dest.bottom += kVDropStairsSpeed;
	thisGlider->whole.bottom = thisGlider->dest.bottom;
	
	hNotClipped = thisGlider->dest.right - leftClip;
	if (hNotClipped < kGliderWide)
	{
		thisGlider->dest.left = thisGlider->dest.right - hNotClipped;
		thisGlider->src.left = thisGlider->src.right - hNotClipped;
		thisGlider->mask.left = thisGlider->mask.right - hNotClipped;
		thisGlider->destShadow.left = thisGlider->dest.left;
	}
	else
	{
		if (thisGlider->frame == kWasBurning)
			FlagGliderBurning(thisGlider);
		else
			FlagGliderNormal(thisGlider);
		thisGlider->hVel = kHDropStairsSpeed;
		thisGlider->hDesiredVel = kHDropStairsSpeed;
		thisGlider->vVel = kVDropStairsSpeed;
		thisGlider->vDesiredVel = kVDropStairsSpeed;
		thisGlider->enteredRect = thisGlider->dest;
	}
}

//--------------------------------------------------------------  MoveGliderFaceLeft

void MoveGliderFaceLeft (gliderPtr thisGlider)
{
	thisGlider->src = gliderSrc[thisGlider->frame];
	thisGlider->mask = gliderSrc[thisGlider->frame];
	
	MoveGlider(thisGlider);
	
	thisGlider->frame--;
	if (thisGlider->frame < kFirstAboutFaceFrame)
	{
		thisGlider->mode = kGliderNormal;
		thisGlider->facing = kFaceLeft;
	}
}

//--------------------------------------------------------------  MoveGliderFaceRight

void MoveGliderFaceRight (gliderPtr thisGlider)
{
	thisGlider->src = gliderSrc[thisGlider->frame];
	thisGlider->mask = gliderSrc[thisGlider->frame];
	
	MoveGlider(thisGlider);
	
	thisGlider->frame++;
	if (thisGlider->frame > kLastAboutFaceFrame)
	{
		thisGlider->mode = kGliderNormal;
		thisGlider->facing = kFaceRight;
	}
}

//--------------------------------------------------------------  TransportGliderOut

void TransportGliderOut (gliderPtr thisGlider)
{
	Rect		tempRect;
	
	thisGlider->frame--;
	if (thisGlider->frame < 0)
	{
		tempRect = thisGlider->whole;
		QOffsetRect(&tempRect, playOriginH, playOriginV);
		CopyRectWorkToMain(&tempRect);
		tempRect = thisGlider->wholeShadow;
		QOffsetRect(&tempRect, playOriginH, playOriginV);
		CopyRectWorkToMain(&tempRect);
		thisGlider->dontDraw = true;
		
		if (twoPlayerGame)
		{
			if (onePlayerLeft)
			{
				if (playerDead == kPlayer1)
					TransportRoomToRoom(&theGlider2);
				else
					TransportRoomToRoom(&theGlider);
			}
			else
			{
				if (otherPlayerEscaped == kPlayerTransportedOut)
				{
					otherPlayerEscaped = kNoOneEscaped;
					TransportRoomToRoom(thisGlider);
				}
				else
				{
					otherPlayerEscaped = kPlayerTransportedOut;
					RefreshScoreboard(kEscapedTitleMode);
					FlagGliderInLimbo(thisGlider, true);
				}
			}
		}
		else
		{
			TransportRoomToRoom(thisGlider);
		}
	}
	else
	{
		if (thisGlider->facing == kFaceLeft)
		{
			thisGlider->src = gliderSrc[fadeInSequence[thisGlider->frame] + 
					kLeftFadeOffset];
			thisGlider->mask = gliderSrc[fadeInSequence[thisGlider->frame] + 
					kLeftFadeOffset];
		}
		else
		{
			thisGlider->src = gliderSrc[fadeInSequence[thisGlider->frame]];
			thisGlider->mask = gliderSrc[fadeInSequence[thisGlider->frame]];
		}
	}
}

//--------------------------------------------------------------  MoveGliderDownDuct

void MoveGliderDownDuct (gliderPtr thisGlider)
{
	#define		kVDropDuctSpeed		4
	Rect		tempRect;
	short		vNotClipped;
	
	if (thisGlider->facing == kFaceLeft)
	{
		thisGlider->src = gliderSrc[2];
		thisGlider->mask = gliderSrc[2];
	}
	else
	{
		thisGlider->src = gliderSrc[0];
		thisGlider->mask = gliderSrc[0];
	}
	
	if (thisGlider->dest.left < thisGlider->clip.left)
	{
		thisGlider->whole.left = thisGlider->dest.left;
		thisGlider->dest.left++;
		thisGlider->dest.right++;
		thisGlider->whole.right = thisGlider->dest.right;
		
		thisGlider->wholeShadow.left = thisGlider->destShadow.left;
		thisGlider->destShadow.left++;
		thisGlider->destShadow.right++;
		thisGlider->wholeShadow.right = thisGlider->destShadow.right;
	}
	else if (thisGlider->dest.left > thisGlider->clip.left)
	{
		thisGlider->whole.right = thisGlider->dest.right;
		thisGlider->dest.left--;
		thisGlider->dest.right--;
		thisGlider->whole.left = thisGlider->dest.left;
		
		thisGlider->wholeShadow.right = thisGlider->destShadow.right;
		thisGlider->destShadow.left--;
		thisGlider->destShadow.right--;
		thisGlider->wholeShadow.left = thisGlider->destShadow.left;
	}
	
	thisGlider->whole.top = thisGlider->dest.top;
	thisGlider->dest.top += kVDropDuctSpeed;
	thisGlider->dest.bottom += kVDropDuctSpeed;
	thisGlider->whole.bottom = thisGlider->dest.bottom;
	
	vNotClipped = 315 - thisGlider->dest.top;
	if (vNotClipped < kGliderHigh)
	{
		if (vNotClipped <= 0)
		{
			tempRect = thisGlider->whole;
			QOffsetRect(&tempRect, playOriginH, playOriginV);
			CopyRectWorkToMain(&tempRect);
			tempRect = thisGlider->wholeShadow;
			QOffsetRect(&tempRect, playOriginH, playOriginV);
			CopyRectWorkToMain(&tempRect);
			thisGlider->dontDraw = true;
			if (twoPlayerGame)
			{
				if (onePlayerLeft)
				{
					if (playerDead == kPlayer1)
						MoveDuctToDuct(&theGlider2);
					else
						MoveDuctToDuct(&theGlider);
				}
				else
				{
					if (otherPlayerEscaped == kPlayerDuckedOut)
					{
						otherPlayerEscaped = kNoOneEscaped;
						MoveDuctToDuct(thisGlider);
					}
					else
					{
						otherPlayerEscaped = kPlayerDuckedOut;
						RefreshScoreboard(kEscapedTitleMode);
						FlagGliderInLimbo(thisGlider, true);
					}
				}
			}
			else
				MoveDuctToDuct(thisGlider);
		}
		else
		{
			thisGlider->dest.bottom = thisGlider->dest.top + vNotClipped;
			thisGlider->src.bottom = thisGlider->src.top + vNotClipped;
			thisGlider->mask.bottom = thisGlider->mask.top + vNotClipped;
		}
	}
}

//--------------------------------------------------------------  MoveGliderUpDuct

void MoveGliderUpDuct (gliderPtr thisGlider)
{
	#define		kVRiseDuctSpeed		-4
	Rect		tempRect;
	short		vNotClipped;
	
	if (thisGlider->facing == kFaceLeft)
	{
		thisGlider->src = gliderSrc[2];
		thisGlider->mask = gliderSrc[2];
	}
	else
	{
		thisGlider->src = gliderSrc[0];
		thisGlider->mask = gliderSrc[0];
	}
	
	if (thisGlider->dest.left < thisGlider->clip.left)
	{
		thisGlider->whole.left = thisGlider->dest.left;
		thisGlider->dest.left++;
		thisGlider->dest.right++;
		thisGlider->whole.right = thisGlider->dest.right;
		
		thisGlider->wholeShadow.left = thisGlider->destShadow.left;
		thisGlider->destShadow.left++;
		thisGlider->destShadow.right++;
		thisGlider->wholeShadow.right = thisGlider->destShadow.right;
	}
	else if (thisGlider->dest.left > thisGlider->clip.left)
	{
		thisGlider->whole.right = thisGlider->dest.right;
		thisGlider->dest.left--;
		thisGlider->dest.right--;
		thisGlider->whole.left = thisGlider->dest.left;
		
		thisGlider->wholeShadow.right = thisGlider->destShadow.right;
		thisGlider->destShadow.left--;
		thisGlider->destShadow.right--;
		thisGlider->wholeShadow.left = thisGlider->destShadow.left;
	}
	
	thisGlider->whole.bottom = thisGlider->dest.bottom;
	thisGlider->dest.top += kVRiseDuctSpeed;
	thisGlider->dest.bottom += kVRiseDuctSpeed;
	thisGlider->whole.top = thisGlider->dest.top;
	
	vNotClipped = thisGlider->dest.bottom - (kCeilingTransTop + 1);
	if (vNotClipped < kGliderHigh)
	{
		if (vNotClipped <= 0)
		{
			tempRect = thisGlider->whole;
			QOffsetRect(&tempRect, playOriginH, playOriginV);
			CopyRectWorkToMain(&tempRect);
			tempRect = thisGlider->wholeShadow;
			QOffsetRect(&tempRect, playOriginH, playOriginV);
			CopyRectWorkToMain(&tempRect);
			thisGlider->dontDraw = true;
			if (twoPlayerGame)
			{
				if (onePlayerLeft)
				{
					if (playerDead == kPlayer1)
						MoveDuctToDuct(&theGlider2);
					else
						MoveDuctToDuct(&theGlider);
				}
				else
				{
					if (otherPlayerEscaped == kPlayerDuckedOut)
					{
						otherPlayerEscaped = kNoOneEscaped;
						MoveDuctToDuct(thisGlider);
					}
					else
					{
						otherPlayerEscaped = kPlayerDuckedOut;
						RefreshScoreboard(kEscapedTitleMode);
						FlagGliderInLimbo(thisGlider, true);
					}
				}
			}
			else
				MoveDuctToDuct(thisGlider);
		}
		else
		{
			thisGlider->dest.top = thisGlider->dest.bottom - vNotClipped;
			thisGlider->src.top = thisGlider->src.bottom - vNotClipped;
			thisGlider->mask.top = thisGlider->mask.bottom - vNotClipped;
		}
	}
}

//--------------------------------------------------------------  FinishGliderMailingLeft

void FinishGliderMailingLeft (gliderPtr thisGlider)
{
	#define		kHPushMailSpeed		-4
	short		hNotClipped;
	
	if (thisGlider->dest.left == thisGlider->dest.right)
		PlayPrioritySound(kTransInSound, kTransInPriority);
	
	thisGlider->whole.right = thisGlider->dest.right;
	thisGlider->dest.left += kHPushMailSpeed;
	thisGlider->dest.right += kHPushMailSpeed;
	thisGlider->whole.left = thisGlider->dest.left;
	
	thisGlider->wholeShadow.right = thisGlider->destShadow.right;
	thisGlider->destShadow.left += kHPushMailSpeed;
	thisGlider->destShadow.right += kHPushMailSpeed;
	thisGlider->wholeShadow.left = thisGlider->destShadow.left;
	
	hNotClipped = thisGlider->clip.right - thisGlider->dest.left;
	if (hNotClipped < kGliderWide)
	{
		thisGlider->dest.right = thisGlider->dest.left + hNotClipped;
		thisGlider->src.right = thisGlider->src.left + hNotClipped;
		thisGlider->mask.right = thisGlider->mask.left + hNotClipped;
		thisGlider->destShadow.right = thisGlider->dest.right;
	}
	else
	{
		if (thisGlider->frame == kWasBurning)
			FlagGliderBurning(thisGlider);
		else
			FlagGliderNormal(thisGlider);
		thisGlider->enteredRect = thisGlider->dest;
	}
}

//--------------------------------------------------------------  FinishGliderMailingRight

void FinishGliderMailingRight (gliderPtr thisGlider)
{
	#define		kHPushMailRtSpeed	4
	short		hNotClipped;
	
	if (thisGlider->dest.left == thisGlider->dest.right)
		PlayPrioritySound(kTransInSound, kTransInPriority);
	
	thisGlider->whole.left = thisGlider->dest.left;
	thisGlider->dest.left += kHPushMailRtSpeed;
	thisGlider->dest.right += kHPushMailRtSpeed;
	thisGlider->whole.right = thisGlider->dest.right;
	
	thisGlider->wholeShadow.left = thisGlider->destShadow.left;
	thisGlider->destShadow.left += kHPushMailRtSpeed;
	thisGlider->destShadow.right += kHPushMailRtSpeed;
	thisGlider->wholeShadow.right = thisGlider->destShadow.right;
	
	hNotClipped = thisGlider->dest.right - thisGlider->clip.left;
	if (hNotClipped < kGliderWide)
	{
		thisGlider->dest.left = thisGlider->dest.right - hNotClipped;
		thisGlider->src.left = thisGlider->src.right - hNotClipped;
		thisGlider->mask.left = thisGlider->mask.right - hNotClipped;
		thisGlider->destShadow.left = thisGlider->dest.left;
	}
	else
	{
		if (thisGlider->frame == kWasBurning)
			FlagGliderBurning(thisGlider);
		else
			FlagGliderNormal(thisGlider);
		thisGlider->enteredRect = thisGlider->dest;
	}
}

//--------------------------------------------------------------  FinishGliderDuctingIn

void FinishGliderDuctingIn (gliderPtr thisGlider)
{
	#define		kVDropStairsSpeed	4
	short		vNotClipped;
	
	if (thisGlider->dest.top == thisGlider->dest.bottom)
		PlayPrioritySound(kTransInSound, kTransInPriority);
	
	thisGlider->whole.top = thisGlider->dest.top;
	thisGlider->dest.top += kVDropDuctSpeed;
	thisGlider->dest.bottom += kVDropDuctSpeed;
	thisGlider->whole.bottom = thisGlider->dest.bottom;
	
	vNotClipped = thisGlider->dest.bottom - (kCeilingTransTop + 1);
	if (vNotClipped < kGliderHigh)
	{
		thisGlider->dest.top = thisGlider->dest.bottom - vNotClipped;
		thisGlider->src.top = thisGlider->src.bottom - vNotClipped;
		thisGlider->mask.top = thisGlider->mask.bottom - vNotClipped;
	}
	else
	{
		if (thisGlider->frame == kWasBurning)
			FlagGliderBurning(thisGlider);
		else
			FlagGliderNormal(thisGlider);
		thisGlider->enteredRect = thisGlider->dest;
		FlagStillOvers(thisGlider);
	}
}

//--------------------------------------------------------------  MoveGliderInMailLeft

void MoveGliderInMailLeft (gliderPtr thisGlider)
{
	#define		kHMailPullSpeed		4
	#define		kVMailDropSpeed		2
	Rect		tempRect;
	short		fromIdealTop, hNotClipped;
	
	if (thisGlider->facing == kFaceLeft)
	{
		thisGlider->src = gliderSrc[2];
		thisGlider->mask = gliderSrc[2];
	}
	else
	{
		thisGlider->src = gliderSrc[0];
		thisGlider->mask = gliderSrc[0];
	}
	
	if (thisGlider->dest.top < thisGlider->clip.top)
	{
		fromIdealTop = thisGlider->clip.top - thisGlider->dest.top;
		if (fromIdealTop > kVMailDropSpeed)
			fromIdealTop = kVMailDropSpeed;
		
		thisGlider->whole.top = thisGlider->dest.top;
		thisGlider->dest.top += fromIdealTop;
		thisGlider->dest.bottom += fromIdealTop;
		thisGlider->whole.bottom = thisGlider->dest.bottom;
	}
	
	thisGlider->whole.left = thisGlider->dest.left;
	thisGlider->dest.left += kHMailPullSpeed;
	thisGlider->dest.right += kHMailPullSpeed;
	thisGlider->whole.right = thisGlider->dest.right;
	
	thisGlider->wholeShadow.left = thisGlider->destShadow.left;
	thisGlider->destShadow.left += kHMailPullSpeed;
	thisGlider->destShadow.right += kHMailPullSpeed;
	thisGlider->wholeShadow.right = thisGlider->destShadow.right;
	
	hNotClipped = thisGlider->clip.right - thisGlider->dest.left;
	if (hNotClipped < kGliderWide)
	{
		if (hNotClipped <= 0)
		{
			tempRect = thisGlider->whole;
			QOffsetRect(&tempRect, playOriginH, playOriginV);
			CopyRectWorkToMain(&tempRect);
			tempRect = thisGlider->wholeShadow;
			QOffsetRect(&tempRect, playOriginH, playOriginV);
			CopyRectWorkToMain(&tempRect);
			thisGlider->dontDraw = true;
			if (twoPlayerGame)
			{
				if (onePlayerLeft)
				{
					if (playerDead == kPlayer1)
						MoveMailToMail(&theGlider2);
					else
						MoveMailToMail(&theGlider);
				}
				else
				{
					if (otherPlayerEscaped == kPlayerMailedOut)
					{
						otherPlayerEscaped = kNoOneEscaped;
						MoveMailToMail(thisGlider);
					}
					else
					{
						otherPlayerEscaped = kPlayerMailedOut;
						RefreshScoreboard(kEscapedTitleMode);
						FlagGliderInLimbo(thisGlider, true);
					}
				}
			}
			else
				MoveMailToMail(thisGlider);
		}
		else
		{
			thisGlider->dest.right = thisGlider->dest.left + hNotClipped;
			thisGlider->src.right = thisGlider->src.left + hNotClipped;
			thisGlider->mask.right = thisGlider->mask.left + hNotClipped;
			thisGlider->destShadow.right = thisGlider->dest.right;
		}
	}
}

//--------------------------------------------------------------  MoveGliderInMailRight

void MoveGliderInMailRight (gliderPtr thisGlider)
{
	#define		kHMailPullRtSpeed	-4
	#define		kVMailDropSpeed		2
	Rect		tempRect;
	short		fromIdealTop, hNotClipped;
	
	if (thisGlider->facing == kFaceLeft)
	{
		thisGlider->src = gliderSrc[2];
		thisGlider->mask = gliderSrc[2];
	}
	else
	{
		thisGlider->src = gliderSrc[0];
		thisGlider->mask = gliderSrc[0];
	}
	
	if (thisGlider->dest.top < thisGlider->clip.top)
	{
		fromIdealTop = thisGlider->clip.top - thisGlider->dest.top;
		if (fromIdealTop > kVMailDropSpeed)
			fromIdealTop = kVMailDropSpeed;
		
		thisGlider->whole.top = thisGlider->dest.top;
		thisGlider->dest.top += fromIdealTop;
		thisGlider->dest.bottom += fromIdealTop;
		thisGlider->whole.bottom = thisGlider->dest.bottom;
	}
	
	thisGlider->whole.right = thisGlider->dest.right;
	thisGlider->dest.left += kHMailPullRtSpeed;
	thisGlider->dest.right += kHMailPullRtSpeed;
	thisGlider->whole.left = thisGlider->dest.left;
	
	thisGlider->wholeShadow.right = thisGlider->destShadow.right;
	thisGlider->destShadow.left += kHMailPullRtSpeed;
	thisGlider->destShadow.right += kHMailPullRtSpeed;
	thisGlider->wholeShadow.left = thisGlider->destShadow.left;
	
	hNotClipped = thisGlider->dest.right - thisGlider->clip.left;
	if (hNotClipped < kGliderWide)
	{
		if (hNotClipped <= 0)
		{
			tempRect = thisGlider->whole;
			QOffsetRect(&tempRect, playOriginH, playOriginV);
			CopyRectWorkToMain(&tempRect);
			tempRect = thisGlider->wholeShadow;
			QOffsetRect(&tempRect, playOriginH, playOriginV);
			CopyRectWorkToMain(&tempRect);
			thisGlider->dontDraw = true;
			if (twoPlayerGame)
			{
				if (onePlayerLeft)
				{
					if (playerDead == kPlayer1)
						MoveMailToMail(&theGlider2);
					else
						MoveMailToMail(&theGlider);
				}
				else
				{
					if (otherPlayerEscaped == kPlayerMailedOut)
					{
						otherPlayerEscaped = kNoOneEscaped;
						MoveMailToMail(thisGlider);
					}
					else
					{
						otherPlayerEscaped = kPlayerMailedOut;
						RefreshScoreboard(kEscapedTitleMode);
						FlagGliderInLimbo(thisGlider, true);
					}
				}
			}
			else
				MoveMailToMail(thisGlider);
		}
		else
		{
			thisGlider->dest.left = thisGlider->dest.right - hNotClipped;
			thisGlider->src.left = thisGlider->src.right - hNotClipped;
			thisGlider->mask.left = thisGlider->mask.right - hNotClipped;
			thisGlider->destShadow.left = thisGlider->dest.left;
		}
	}
}

//--------------------------------------------------------------  DeckGliderInFoil

void DeckGliderInFoil (gliderPtr thisGlider)
{
	showFoil = true;
	
	if (twoPlayerGame)
	{
		SetPort((GrafPtr)glidSrcMap);
		LoadGraphic(kGliderFoilPictID);
		SetPort((GrafPtr)glid2SrcMap);
		LoadGraphic(kGliderFoil2PictID);
	}
	
	if (thisGlider->facing == kFaceLeft)
	{
		thisGlider->src = gliderSrc[(thisGlider->frame + 2) + 
				kLeftFadeOffset];
		thisGlider->mask = gliderSrc[(thisGlider->frame + 2) + 
				kLeftFadeOffset];
	}
	else
	{
		thisGlider->src = gliderSrc[thisGlider->frame + 2];
		thisGlider->mask = gliderSrc[thisGlider->frame + 2];
	}
}

//--------------------------------------------------------------  MoveGliderFoilGoing

void MoveGliderFoilGoing (gliderPtr thisGlider)
{
	thisGlider->frame++;
	if (thisGlider->frame > 8)
	{
		FlagGliderNormal(thisGlider);
	}
	else
	{
		if (thisGlider->frame < 5)
		{
			if (thisGlider->facing == kFaceLeft)
			{
				thisGlider->src = gliderSrc[(10 - thisGlider->frame) + 
						kLeftFadeOffset];
				thisGlider->mask = gliderSrc[(10 - thisGlider->frame) + 
						kLeftFadeOffset];
			}
			else
			{
				thisGlider->src = gliderSrc[10 - thisGlider->frame];
				thisGlider->mask = gliderSrc[10 - thisGlider->frame];
			}
		}
		else
			DeckGliderInFoil(thisGlider);
	}
	MoveGlider(thisGlider);
}

//--------------------------------------------------------------  xxxx

void RemoveFoilFromGlider (gliderPtr thisGlider)
{
	showFoil = false;
	
	if (twoPlayerGame)
	{
		SetPort((GrafPtr)glidSrcMap);
		LoadGraphic(kGliderPictID);
		SetPort((GrafPtr)glid2SrcMap);
		LoadGraphic(kGlider2PictID);
	}
	
	if (thisGlider->facing == kFaceLeft)
	{
		thisGlider->src = gliderSrc[(thisGlider->frame + 2) + 
				kLeftFadeOffset];
		thisGlider->mask = gliderSrc[(thisGlider->frame + 2) + 
				kLeftFadeOffset];
	}
	else
	{
		thisGlider->src = gliderSrc[thisGlider->frame + 2];
		thisGlider->mask = gliderSrc[thisGlider->frame + 2];
	}
}

//--------------------------------------------------------------  MoveGliderFoilLosing

void MoveGliderFoilLosing (gliderPtr thisGlider)
{
	thisGlider->frame++;
	if (thisGlider->frame > 8)
		FlagGliderNormal(thisGlider);
	else
	{
		if (thisGlider->frame < 5)
		{
			if (thisGlider->facing == kFaceLeft)
			{
				thisGlider->src = gliderSrc[(10 - thisGlider->frame) + 
						kLeftFadeOffset];
				thisGlider->mask = gliderSrc[(10 - thisGlider->frame) + 
						kLeftFadeOffset];
			}
			else
			{
				thisGlider->src = gliderSrc[10 - thisGlider->frame];
				thisGlider->mask = gliderSrc[10 - thisGlider->frame];
			}
		}
		else
			RemoveFoilFromGlider(thisGlider);
	}
	MoveGlider(thisGlider);
}

//--------------------------------------------------------------  MoveGliderShredding

void MoveGliderShredding (gliderPtr thisGlider)
{
	#define		kDropShredSlow		1
	#define		kDropShredFast		4
	short		vNotClipped;
	
	if (thisGlider->frame > 0)
	{
		if (thisGlider->facing == kFaceLeft)
		{
			thisGlider->src = gliderSrc[2];
			thisGlider->mask = gliderSrc[2];
		}
		else
		{
			thisGlider->src = gliderSrc[0];
			thisGlider->mask = gliderSrc[0];
		}
		
		vNotClipped = thisGlider->frame - thisGlider->dest.top;
		if (vNotClipped < kGliderHigh)
		{
			thisGlider->whole.top = thisGlider->dest.top;
			thisGlider->dest.top += kDropShredSlow;
			thisGlider->dest.bottom += kDropShredSlow;
			thisGlider->whole.bottom = thisGlider->dest.bottom;
			shadowVisible = false;
			PlayPrioritySound(kShredSound, kShredPriority);
		}
		else
		{
			thisGlider->whole.top = thisGlider->dest.top;
			thisGlider->dest.top += kDropShredFast;
			thisGlider->dest.bottom += kDropShredFast;
			thisGlider->whole.bottom = thisGlider->dest.bottom;
		}
		
		vNotClipped = thisGlider->frame - thisGlider->dest.top;
		if (vNotClipped < kGliderHigh)
		{
			if (vNotClipped <= 0)
			{
				AddAShreddedGlider(&thisGlider->dest);
				thisGlider->frame = kShredderCountdown;
			}
			else
			{
				thisGlider->dest.bottom = thisGlider->dest.top + vNotClipped;
				thisGlider->src.bottom = thisGlider->src.top + vNotClipped;
				thisGlider->mask.bottom = thisGlider->mask.top + vNotClipped;
			}
		}
	}
	else
	{
		thisGlider->frame++;
		if (thisGlider->frame >= 0)
			OffAMortal(thisGlider);
	}
}

//--------------------------------------------------------------  HandleIdleGlider

void HandleIdleGlider (gliderPtr thisGlider)
{
	thisGlider->hVel--;
	if (thisGlider->hVel <= 0)
	{
		thisGlider->mode = thisGlider->wasMode;
		thisGlider->dontDraw = false;
	}
}

//--------------------------------------------------------------  HandleGlider

void HandleGlider (gliderPtr thisGlider)
{
	switch (thisGlider->mode)
	{
		case kGliderNormal:
		MoveGliderNormal(thisGlider);
		break;
		
		case kGliderFadingIn:
		FadeGliderIn(thisGlider);
		break;
		
		case kGliderFadingOut:
		FadeGliderOut(thisGlider);
		break;
		
		case kGliderGoingUp:
		MoveGliderUpStairs(thisGlider);
		break;
		
		case kGliderComingUp:
		FinishGliderUpStairs(thisGlider);
		break;
		
		case kGliderGoingDown:
		MoveGliderDownStairs(thisGlider);
		break;
		
		case kGliderComingDown:
		FinishGliderDownStairs(thisGlider);
		break;
		
		case kGliderFaceLeft:
		MoveGliderFaceLeft(thisGlider);
		break;
		
		case kGliderFaceRight:
		MoveGliderFaceRight(thisGlider);
		break;
		
		case kGliderBurning:
		MoveGliderBurning(thisGlider);
		break;
		
		case kGliderTransporting:
		TransportGliderOut(thisGlider);
		break;
		
		case kGliderDuctingDown:
		MoveGliderDownDuct(thisGlider);
		break;
		
		case kGliderDuctingUp:
		MoveGliderUpDuct(thisGlider);
		break;
		
		case kGliderDuctingIn:
		FinishGliderDuctingIn(thisGlider);
		break;
		
		case kGliderMailInLeft:
		MoveGliderInMailLeft(thisGlider);
		break;
		
		case kGliderMailOutLeft:				// <-- G _[]
		FinishGliderMailingLeft(thisGlider);
		break;
		
		case kGliderMailInRight:
		MoveGliderInMailRight(thisGlider);
		break;
		
		case kGliderMailOutRight:				// []_ G -->
		FinishGliderMailingRight(thisGlider);
		break;
		
		case kGliderGoingFoil:
		MoveGliderFoilGoing(thisGlider);
		break;
		
		case kGliderLosingFoil:
		MoveGliderFoilLosing(thisGlider);
		break;
		
		case kGliderShredding:
		MoveGliderShredding(thisGlider);
		break;
		
		case kGliderInLimbo:
		break;
		
		case kGliderIdle:
		HandleIdleGlider(thisGlider);
		break;
		
		case kGliderTransportingIn:
		TransportGliderIn(thisGlider);
		break;
		
	}
	
	thisGlider->ignoreLeft = false;
	thisGlider->ignoreRight = false;
	thisGlider->ignoreGround = false;
}

//--------------------------------------------------------------  OffsetGlider

void OffsetGlider (gliderPtr thisGlider, short where)
{
	if ((twoPlayerGame) && (onePlayerLeft) && (thisGlider->which == playerDead))
		return;
	
	switch (where)
	{
		case kToRight:
		thisGlider->dest.left += kRoomWide;
		thisGlider->dest.right += kRoomWide;
		thisGlider->destShadow.left += kRoomWide;
		thisGlider->destShadow.right += kRoomWide;
		thisGlider->whole = thisGlider->dest;
		thisGlider->wholeShadow = thisGlider->destShadow;
		break;
		
		case kToLeft:
		thisGlider->dest.left -= kRoomWide;
		thisGlider->dest.right -= kRoomWide;
		thisGlider->destShadow.left -= kRoomWide;
		thisGlider->destShadow.right -= kRoomWide;
		thisGlider->whole = thisGlider->dest;
		thisGlider->wholeShadow = thisGlider->destShadow;
		break;
		
		case kAbove:
		thisGlider->dest.top -= kTileHigh;
		thisGlider->dest.bottom -= kTileHigh;
		thisGlider->whole = thisGlider->dest;
		break;
		
		case kBelow:
		thisGlider->dest.top += kTileHigh;
		thisGlider->dest.bottom += kTileHigh;
		thisGlider->whole = thisGlider->dest;
		break;
	}
}

//--------------------------------------------------------------  OffAMortal

void OffAMortal (gliderPtr thisGlider)
{	
	if (gameOver)
		return;
	
	if (numShredded > 0)
		RemoveShreds();
	
	mortals--;
	if (mortals < 0)
	{
		HideGlider(thisGlider);
		if (twoPlayerGame)
		{
			if (mortals < -1)		// both players are now dead
			{
				FlagGameOver();
				thisGlider->dontDraw = true;
			}
			else
			{
				FlagGliderInLimbo(thisGlider, false);
				thisGlider->dontDraw = true;
				onePlayerLeft = true;
				playerDead = thisGlider->which;
			}
		}
		else
		{
			FlagGameOver();
			thisGlider->dontDraw = true;
		}
	}
	else
	{
		QuickGlidersRefresh();
		HideGlider(thisGlider);
	}
	
	if (mortals >= 0)
	{
		if (thisGlider->mode == kGliderGoingFoil)
			DeckGliderInFoil(thisGlider);
		
		FlagGliderNormal(thisGlider);
		if (playerSuicide)
			FollowTheLeader();
		else
		{
			StartGliderFadingIn(thisGlider);
			thisGlider->dest = thisGlider->enteredRect;
			thisGlider->whole = thisGlider->dest;
			thisGlider->destShadow.left = thisGlider->dest.left;
			thisGlider->destShadow.right = thisGlider->dest.right;
			thisGlider->wholeShadow = thisGlider->destShadow;
		}
	}
	else if ((mortals == -1) && (onePlayerLeft) && (!gameOver))
	{
		switch (otherPlayerEscaped)
		{
			case kPlayerEscapedUp:
			case kPlayerEscapingUpStairs:
			case kPlayerEscapedUpStairs:
			if (playerDead == kPlayer1)
				MoveRoomToRoom(&theGlider2, kAbove);
			else
				MoveRoomToRoom(&theGlider, kAbove);
			break;
			
			case kPlayerEscapedDown:
			case kPlayerEscapingDownStairs:
			case kPlayerEscapedDownStairs:
			if (playerDead == kPlayer1)
				MoveRoomToRoom(&theGlider2, kBelow);
			else
				MoveRoomToRoom(&theGlider, kBelow);
			break;
			
			case kPlayerEscapedLeft:
			if (playerDead == kPlayer1)
				MoveRoomToRoom(&theGlider2, kToLeft);
			else
				MoveRoomToRoom(&theGlider, kToLeft);
			break;
			
			case kPlayerEscapedRight:
			if (playerDead == kPlayer1)
				MoveRoomToRoom(&theGlider2, kToRight);
			else
				MoveRoomToRoom(&theGlider, kToRight);
			break;
			
			case kPlayerTransportedOut:
			if (playerDead == kPlayer1)
				TransportRoomToRoom(&theGlider2);
			else
				TransportRoomToRoom(&theGlider);
			break;
			
			case kPlayerMailedOut:
			if (playerDead == kPlayer1)
				MoveMailToMail(&theGlider2);
			else
				MoveMailToMail(&theGlider);
			break;
			
			case kPlayerDuckedOut:
			if (playerDead == kPlayer1)
				MoveDuctToDuct(&theGlider2);
			else
				MoveDuctToDuct(&theGlider);
			break;
			
			default:
			break;
		}
		
		otherPlayerEscaped = kPlayerIsDeadForever;
	}
}

