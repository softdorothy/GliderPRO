
//============================================================================
//----------------------------------------------------------------------------
//									Modes.c
//----------------------------------------------------------------------------
//============================================================================


#include "Externs.h"
#include "RectUtils.h"


short		saidFollow;

extern	Rect		gliderSrc[];
extern	Rect		transRect;
extern	short		fadeInSequence[], linkedToWhat;
extern	short		rightClip, leftClip, transRoom;
extern	Boolean		hasMirror, shadowVisible, firstPlayer, twoPlayerGame;
extern	Boolean		onePlayerLeft, playerDead;

//==============================================================  Functions
//--------------------------------------------------------------  StartGliderFadingIn

void StartGliderFadingIn (gliderPtr thisGlider)
{	
	if (foilTotal <= 0)
		showFoil = false;
	
	thisGlider->mode = kGliderFadingIn;
	thisGlider->whole = thisGlider->dest;
	thisGlider->frame = 0;
	thisGlider->dontDraw = false;
	if (thisGlider->facing == kFaceLeft)
	{
		thisGlider->src = 
				gliderSrc[fadeInSequence[thisGlider->frame] + kLeftFadeOffset];
		thisGlider->mask = 
				gliderSrc[fadeInSequence[thisGlider->frame] + kLeftFadeOffset];
	}
	else
	{
		thisGlider->src = gliderSrc[fadeInSequence[thisGlider->frame]];
		thisGlider->mask = gliderSrc[fadeInSequence[thisGlider->frame]];
	}
}

//--------------------------------------------------------------  StartGliderTransportingIn

void StartGliderTransportingIn (gliderPtr thisGlider)
{
	if (foilTotal <= 0)
		showFoil = false;
	
	thisGlider->mode = kGliderTransportingIn;
	thisGlider->whole = thisGlider->dest;
	thisGlider->frame = 0;
	thisGlider->dontDraw = false;
	if (thisGlider->facing == kFaceLeft)
	{
		thisGlider->src = 
				gliderSrc[fadeInSequence[thisGlider->frame] + kLeftFadeOffset];
		thisGlider->mask = 
				gliderSrc[fadeInSequence[thisGlider->frame] + kLeftFadeOffset];
	}
	else
	{
		thisGlider->src = gliderSrc[fadeInSequence[thisGlider->frame]];
		thisGlider->mask = gliderSrc[fadeInSequence[thisGlider->frame]];
	}
}

//--------------------------------------------------------------  StartGliderFadingOut

void StartGliderFadingOut (gliderPtr thisGlider)
{
	Rect		tempBounds;
	
	if (thisGlider->mode == kGliderFadingOut)
		return;
	
	if (thisGlider->mode == kGliderGoingFoil)
		DeckGliderInFoil(thisGlider);
	else if (thisGlider->mode == kGliderLosingFoil)
		RemoveFoilFromGlider(thisGlider);
	
	if (RectTall(&thisGlider->dest) > kGliderHigh)
	{
		tempBounds = thisGlider->dest;
		QOffsetRect(&tempBounds, playOriginH, playOriginV);
		AddRectToWorkRects(&tempBounds);
		if (hasMirror)
		{
			tempBounds = thisGlider->dest;
			QOffsetRect(&tempBounds, playOriginH - 20, playOriginV - 16);
			AddRectToWorkRects(&tempBounds);
		}
		thisGlider->dest.right = thisGlider->dest.left + kGliderWide;
		thisGlider->dest.top = thisGlider->dest.bottom - kGliderHigh;
	}
	thisGlider->mode = kGliderFadingOut;
	thisGlider->whole = thisGlider->dest;
	thisGlider->frame = kLastFadeSequence - 1;
	if (thisGlider->facing == kFaceLeft)
	{
		thisGlider->src = 
				gliderSrc[fadeInSequence[thisGlider->frame] + kLeftFadeOffset];
		thisGlider->mask = 
				gliderSrc[fadeInSequence[thisGlider->frame] + kLeftFadeOffset];
	}
	else
	{
		thisGlider->src = gliderSrc[fadeInSequence[thisGlider->frame]];
		thisGlider->mask = gliderSrc[fadeInSequence[thisGlider->frame]];
	}
}

//--------------------------------------------------------------  StartGliderGoingUpStairs

void StartGliderGoingUpStairs (gliderPtr thisGlider)
{
	if (thisGlider->mode == kGliderGoingFoil)
		DeckGliderInFoil(thisGlider);
	else if (thisGlider->mode == kGliderLosingFoil)
		RemoveFoilFromGlider(thisGlider);
	
	if (thisGlider->mode == kGliderBurning)
		thisGlider->frame = kWasBurning;
	else
		thisGlider->frame = 0;
	
	thisGlider->mode = kGliderGoingUp;
}

//--------------------------------------------------------------  StartGliderGoingDownStairs

void StartGliderGoingDownStairs (gliderPtr thisGlider)
{
	if (thisGlider->mode == kGliderGoingFoil)
		DeckGliderInFoil(thisGlider);
	else if (thisGlider->mode == kGliderLosingFoil)
		RemoveFoilFromGlider(thisGlider);
	
	if (thisGlider->mode == kGliderBurning)
		thisGlider->frame = kWasBurning;
	else
		thisGlider->frame = 0;
	
	thisGlider->mode = kGliderGoingDown;
	rightClip = GetUpStairsRightEdge();
}

//--------------------------------------------------------------  StartGliderMailingIn

void StartGliderMailingIn (gliderPtr thisGlider, Rect *bounds, hotPtr who)
{
	short		topSought, whoLinked;
	Byte		objLinked;
	char		wasState;
	
	PlayPrioritySound(kTransOutSound, kTransOutPriority);
	
	whoLinked = who->who;
	transRoom = masterObjects[whoLinked].roomLink;
	objLinked = masterObjects[whoLinked].objectLink;
	linkedToWhat = WhatAreWeLinkedTo(transRoom, objLinked);
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	GetObjectRect(&(*thisHouse)->rooms[transRoom].objects[objLinked], &transRect);
	HSetState((Handle)thisHouse, wasState);
	
	thisGlider->frame = 0;
	thisGlider->clip = *bounds;
	topSought = bounds->bottom - RectTall(&thisGlider->dest);
	thisGlider->clip.top = topSought;
}

//--------------------------------------------------------------  StartGliderMailingOut

void StartGliderMailingOut (gliderPtr thisGlider)
{
	if (thisGlider->mode == kGliderGoingFoil)
		DeckGliderInFoil(thisGlider);
	else if (thisGlider->mode == kGliderLosingFoil)
		RemoveFoilFromGlider(thisGlider);
	
	if (linkedToWhat == kLinkedToLeftMailbox)
	{
		thisGlider->facing = kFaceLeft;
		thisGlider->mode = kGliderMailOutLeft;
		thisGlider->src = gliderSrc[2];
		thisGlider->mask = gliderSrc[2];
	}
	else
	{
		thisGlider->facing = kFaceRight;
		thisGlider->mode = kGliderMailOutRight;
		thisGlider->src = gliderSrc[0];
		thisGlider->mask = gliderSrc[0];
	}
	
	thisGlider->hVel = 0;
	thisGlider->vVel = 0;
	thisGlider->hDesiredVel = 0;
	thisGlider->vDesiredVel = 0;
	thisGlider->tipped = false;
	thisGlider->dontDraw = false;
}

//--------------------------------------------------------------  StartGliderDuctingDown

void StartGliderDuctingDown (gliderPtr thisGlider, Rect *bounds, hotPtr who)
{
	short		leftSought, whoLinked;
	Byte		objLinked;
	char		wasState;
	
	PlayPrioritySound(kTransOutSound, kTransOutPriority);
	
	if (thisGlider->mode == kGliderGoingFoil)
		DeckGliderInFoil(thisGlider);
	else if (thisGlider->mode == kGliderLosingFoil)
		RemoveFoilFromGlider(thisGlider);
	
	whoLinked = who->who;
	transRoom = masterObjects[whoLinked].roomLink;
	objLinked = masterObjects[whoLinked].objectLink;
	linkedToWhat = WhatAreWeLinkedTo(transRoom, objLinked);
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	GetObjectRect(&(*thisHouse)->rooms[transRoom].objects[objLinked], &transRect);
	HSetState((Handle)thisHouse, wasState);
	
	thisGlider->frame = 0;
	thisGlider->clip = *bounds;
	leftSought = bounds->left + ((RectWide(bounds) - kGliderWide) / 2);
	thisGlider->clip.left = leftSought;
	
	thisGlider->mode = kGliderDuctingDown;
}

//--------------------------------------------------------------  StartGliderDuctingUp

void StartGliderDuctingUp (gliderPtr thisGlider, Rect *bounds, hotPtr who)
{
	short		leftSought, whoLinked;
	Byte		objLinked;
	char		wasState;
	
	PlayPrioritySound(kTransOutSound, kTransOutPriority);
	
	if (thisGlider->mode == kGliderGoingFoil)
		DeckGliderInFoil(thisGlider);
	else if (thisGlider->mode == kGliderLosingFoil)
		RemoveFoilFromGlider(thisGlider);
	
	whoLinked = who->who;
	transRoom = masterObjects[whoLinked].roomLink;
	objLinked = masterObjects[whoLinked].objectLink;
	linkedToWhat = WhatAreWeLinkedTo(transRoom, objLinked);
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	GetObjectRect(&(*thisHouse)->rooms[transRoom].objects[objLinked], &transRect);
	HSetState((Handle)thisHouse, wasState);
	
	thisGlider->frame = 0;
	thisGlider->clip = *bounds;
	leftSought = bounds->left + ((RectWide(bounds) - kGliderWide) / 2);
	thisGlider->clip.left = leftSought;
	
	thisGlider->mode = kGliderDuctingUp;
}

//--------------------------------------------------------------  StartGliderDuctingIn

void StartGliderDuctingIn (gliderPtr thisGlider)
{
	thisGlider->mode = kGliderDuctingIn;
	thisGlider->whole = thisGlider->dest;
	thisGlider->dontDraw = false;
}

//--------------------------------------------------------------  StartGliderTransporting

void StartGliderTransporting (gliderPtr thisGlider, hotPtr who)
{
	short		whoLinked;
	Byte		objLinked;
	char		wasState;
	
	PlayPrioritySound(kTransOutSound, kTransOutPriority);
	
	if (thisGlider->mode == kGliderGoingFoil)
		DeckGliderInFoil(thisGlider);
	else if (thisGlider->mode == kGliderLosingFoil)
		RemoveFoilFromGlider(thisGlider);
	
	whoLinked = who->who;
	transRoom = masterObjects[whoLinked].roomLink;
	objLinked = masterObjects[whoLinked].objectLink;
	linkedToWhat = WhatAreWeLinkedTo(transRoom, objLinked);
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	GetObjectRect(&(*thisHouse)->rooms[transRoom].objects[objLinked], &transRect);
	HSetState((Handle)thisHouse, wasState);
	
	thisGlider->dest.right = thisGlider->dest.left + kGliderWide;
	thisGlider->dest.bottom = thisGlider->dest.top + kGliderHigh;
	thisGlider->destShadow.right = thisGlider->destShadow.left + kGliderWide;
	thisGlider->destShadow.bottom = thisGlider->destShadow.top + kShadowHigh;
	thisGlider->mode = kGliderTransporting;
	thisGlider->whole = thisGlider->dest;
	thisGlider->frame = kLastFadeSequence - 1;
	if (thisGlider->facing == kFaceLeft)
	{
		thisGlider->src = 
				gliderSrc[fadeInSequence[thisGlider->frame] + kLeftFadeOffset];
		thisGlider->mask = 
				gliderSrc[fadeInSequence[thisGlider->frame] + kLeftFadeOffset];
	}
	else
	{
		thisGlider->src = gliderSrc[fadeInSequence[thisGlider->frame]];
		thisGlider->mask = gliderSrc[fadeInSequence[thisGlider->frame]];
	}
}

//--------------------------------------------------------------  FlagGliderNormal

void FlagGliderNormal (gliderPtr thisGlider)
{
	thisGlider->dest.right = thisGlider->dest.left + kGliderWide;
	thisGlider->dest.bottom = thisGlider->dest.top + kGliderHigh;
	thisGlider->destShadow.right = thisGlider->destShadow.left + kGliderWide;
	thisGlider->destShadow.bottom = thisGlider->destShadow.top + kShadowHigh;
	thisGlider->mode = kGliderNormal;
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
	thisGlider->hVel = 0;
	thisGlider->vVel = 0;
	thisGlider->hDesiredVel = 0;
	thisGlider->vDesiredVel = 0;
	thisGlider->tipped = false;
	thisGlider->ignoreLeft = false;
	thisGlider->ignoreRight = false;
	thisGlider->ignoreGround = false;
	thisGlider->dontDraw = false;
	thisGlider->frame = 0;
	shadowVisible = IsShadowVisible();
}

//--------------------------------------------------------------  FlagGliderShredding

void FlagGliderShredding (gliderPtr thisGlider, Rect *bounds)
{
	PlayPrioritySound(kCaughtFireSound, kCaughtFirePriority);
	thisGlider->dest.left = bounds->left + 36;
	thisGlider->dest.right = thisGlider->dest.left + kGliderWide;
	thisGlider->dest.bottom = thisGlider->dest.top + kGliderHigh;
	if (thisGlider->dest.left > thisGlider->whole.left)
	{
		thisGlider->whole.right = thisGlider->dest.right;
		thisGlider->wholeShadow.right = thisGlider->dest.right;
	}
	else
	{
		thisGlider->whole.left = thisGlider->dest.left;
		thisGlider->wholeShadow.left = thisGlider->dest.left;
	}
	thisGlider->destShadow.left = thisGlider->dest.left;
	thisGlider->destShadow.right = thisGlider->destShadow.left + kGliderWide;
	thisGlider->destShadow.bottom = thisGlider->destShadow.top + kShadowHigh;
	thisGlider->mode = kGliderShredding;
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
	thisGlider->hVel = 0;
	thisGlider->vVel = 0;
	thisGlider->hDesiredVel = 0;
	thisGlider->vDesiredVel = 0;
	thisGlider->frame = bounds->bottom - 3;
	thisGlider->tipped = false;
}

//--------------------------------------------------------------  FlagGliderBurning

void FlagGliderBurning (gliderPtr thisGlider)
{
	#define		kFramesToBurn	60
	
	PlayPrioritySound(kCaughtFireSound, kCaughtFirePriority);
	
	thisGlider->dest.right = thisGlider->dest.left + kGliderWide;
	thisGlider->dest.top = thisGlider->dest.bottom - kGliderBurningHigh;
	thisGlider->destShadow.right = thisGlider->destShadow.left + kGliderWide;
	thisGlider->destShadow.bottom = thisGlider->destShadow.top + kShadowHigh;
	thisGlider->mode = kGliderBurning;
	if (thisGlider->facing == kFaceLeft)
	{
		thisGlider->src = gliderSrc[25];
		thisGlider->mask = gliderSrc[25];
	}
	else
	{
		thisGlider->src = gliderSrc[21];
		thisGlider->mask = gliderSrc[21];
	}
	thisGlider->hVel = 0;
	thisGlider->vVel = 0;
	thisGlider->hDesiredVel = 0;
	thisGlider->vDesiredVel = 0;
	thisGlider->frame = 0;
	thisGlider->wasMode = kFramesToBurn;		// used to count down burning
	thisGlider->tipped = false;
}

//--------------------------------------------------------------  FlagGliderFaceLeft

void FlagGliderFaceLeft (gliderPtr thisGlider)
{
	thisGlider->mode = kGliderFaceLeft;
	thisGlider->frame = kLastAboutFaceFrame;
	thisGlider->src = gliderSrc[kLastAboutFaceFrame];
	thisGlider->mask = gliderSrc[kLastAboutFaceFrame];
}

//--------------------------------------------------------------  FlagGliderFaceRight

void FlagGliderFaceRight (gliderPtr thisGlider)
{
	thisGlider->mode = kGliderFaceRight;
	thisGlider->frame = kFirstAboutFaceFrame;
	thisGlider->src = gliderSrc[kFirstAboutFaceFrame];
	thisGlider->mask = gliderSrc[kFirstAboutFaceFrame];
}

//--------------------------------------------------------------  FlagGliderInLimbo

void FlagGliderInLimbo (gliderPtr thisGlider, Boolean sayIt)
{
	thisGlider->wasMode = thisGlider->mode;
	thisGlider->mode = kGliderInLimbo;
	if ((sayIt) && (saidFollow < 3))
	{
		PlayPrioritySound(kFollowSound, kFollowPriority);
		saidFollow++;
	}
	firstPlayer = thisGlider->which;
}

//--------------------------------------------------------------  UndoGliderLimbo

void UndoGliderLimbo (gliderPtr thisGlider)
{
	if ((twoPlayerGame) && (onePlayerLeft) && (thisGlider->which == playerDead))
		return;
	
	if (thisGlider->mode == kGliderInLimbo)
		thisGlider->mode = thisGlider->wasMode;
	thisGlider->dontDraw = false;
}

//--------------------------------------------------------------  ToggleGliderFacing

void ToggleGliderFacing (gliderPtr thisGlider)
{
	if (thisGlider->mode != kGliderNormal)
		return;
	
	if (thisGlider->facing == kFaceLeft)
		FlagGliderFaceRight(thisGlider);
	else
		FlagGliderFaceLeft(thisGlider);
}

//--------------------------------------------------------------  InsureGliderFacingRight

void InsureGliderFacingRight (gliderPtr thisGlider)
{
	if ((twoPlayerGame) && (onePlayerLeft) && (thisGlider->which == playerDead))
		return;
	
	if ((thisGlider->facing == kFaceLeft) && (thisGlider->mode != kGliderBurning))
		FlagGliderFaceRight(thisGlider);
}

//--------------------------------------------------------------  InsureGliderFacingLeft

void InsureGliderFacingLeft (gliderPtr thisGlider)
{
	if ((twoPlayerGame) && (onePlayerLeft) && (thisGlider->which == playerDead))
		return;
	
	if ((thisGlider->facing == kFaceRight) && (thisGlider->mode != kGliderBurning))
		FlagGliderFaceLeft(thisGlider);
}

//--------------------------------------------------------------  ReadyGliderForTripUpStairs

void ReadyGliderForTripUpStairs (gliderPtr thisGlider)
{
	#define kVGliderAppearsComingUp		100
	
	if ((twoPlayerGame) && (thisGlider->which == playerDead) && (onePlayerLeft))
		return;
	
	thisGlider->facing = kFaceLeft;
	thisGlider->mode = kGliderComingUp;
	thisGlider->src = gliderSrc[2];
	thisGlider->mask = gliderSrc[2];
	thisGlider->hVel = 0;
	thisGlider->vVel = 0;
	thisGlider->hDesiredVel = 0;
	thisGlider->vDesiredVel = 0;
	thisGlider->tipped = false;
	
	rightClip = GetUpStairsRightEdge();
	thisGlider->dest = thisGlider->src;
	ZeroRectCorner(&thisGlider->dest);
	QOffsetRect(&thisGlider->dest, rightClip, kVGliderAppearsComingUp);
	thisGlider->whole = thisGlider->dest;
	thisGlider->destShadow.left = thisGlider->dest.left;
	thisGlider->destShadow.right = thisGlider->dest.right;
	thisGlider->wholeShadow = thisGlider->destShadow;
	
	FinishGliderUpStairs(thisGlider);
}

//--------------------------------------------------------------  ReadyGliderForTripDownStairs

void ReadyGliderForTripDownStairs (gliderPtr thisGlider)
{
	#define kVGliderAppearsComingDown		100
	
	if ((twoPlayerGame) && (thisGlider->which == playerDead) && (onePlayerLeft))
		return;
	
	thisGlider->facing = kFaceRight;
	thisGlider->mode = kGliderComingDown;
	thisGlider->src = gliderSrc[0];
	thisGlider->mask = gliderSrc[0];
	thisGlider->hVel = 0;
	thisGlider->vVel = 0;
	thisGlider->hDesiredVel = 0;
	thisGlider->vDesiredVel = 0;
	thisGlider->tipped = false;
	
	leftClip = GetDownStairsLeftEdge();
	thisGlider->dest = thisGlider->src;
	ZeroRectCorner(&thisGlider->dest);
	QOffsetRect(&thisGlider->dest, leftClip - kGliderWide, kVGliderAppearsComingDown);
	thisGlider->whole = thisGlider->dest;
	thisGlider->destShadow.left = thisGlider->dest.left;
	thisGlider->destShadow.right = thisGlider->dest.right;
	thisGlider->wholeShadow = thisGlider->destShadow;
	
	FinishGliderDownStairs(thisGlider);
}

//--------------------------------------------------------------  StartGliderFoilGoing

void StartGliderFoilGoing (gliderPtr thisGlider)
{
	if ((thisGlider->mode == kGliderGoingFoil) || (thisGlider->mode == kGliderInLimbo))
		return;
	
	QuickFoilRefresh(false);
	
	thisGlider->mode = kGliderGoingFoil;
	thisGlider->whole = thisGlider->dest;
	thisGlider->frame = 0;
	if (thisGlider->facing == kFaceLeft)
	{
		thisGlider->src = gliderSrc[(10 - thisGlider->frame) + kLeftFadeOffset];
		thisGlider->mask = gliderSrc[(10 - thisGlider->frame) + kLeftFadeOffset];
	}
	else
	{
		thisGlider->src = gliderSrc[10 - thisGlider->frame];
		thisGlider->mask = gliderSrc[10 - thisGlider->frame];
	}
}

//--------------------------------------------------------------  StartGliderFoilLosing

void StartGliderFoilLosing (gliderPtr thisGlider)
{
	if ((thisGlider->mode == kGliderLosingFoil) || 
			(thisGlider->mode == kGliderInLimbo))
		return;
	
	QuickFoilRefresh(false);
	PlayPrioritySound(kFizzleSound, kFizzlePriority);
	
	thisGlider->mode = kGliderLosingFoil;
	thisGlider->whole = thisGlider->dest;
	thisGlider->frame = 0;
	if (thisGlider->facing == kFaceLeft)
	{
		thisGlider->src = gliderSrc[(10 - thisGlider->frame) + kLeftFadeOffset];
		thisGlider->mask = gliderSrc[(10 - thisGlider->frame) + kLeftFadeOffset];
	}
	else
	{
		thisGlider->src = gliderSrc[10 - thisGlider->frame];
		thisGlider->mask = gliderSrc[10 - thisGlider->frame];
	}
}

//--------------------------------------------------------------  TagGliderIdle

void TagGliderIdle (gliderPtr thisGlider)
{
	if ((twoPlayerGame) && (onePlayerLeft) && (thisGlider->which == playerDead))
		return;
	
	thisGlider->wasMode = thisGlider->mode;
	thisGlider->mode = kGliderIdle;
	thisGlider->hVel = 30;			// used for 30 frame delay
}

