
//============================================================================
//----------------------------------------------------------------------------
//									Transit.c
//----------------------------------------------------------------------------
//============================================================================


#include "Externs.h"
#include "Environ.h"
#include "RectUtils.h"


void HandleRoomVisitation (void);


short		linkedToWhat;
Boolean		takingTheStairs, firstPlayer;


extern	Rect		justRoomsRect, transRect;
extern	short		transRoom, otherPlayerEscaped;
extern	short		localNumbers[9];
extern	Boolean		topOpen, twoPlayerGame, onePlayerLeft;
extern	Boolean		playerDead, playerSuicide, tvOn;


//==============================================================  Functions
//--------------------------------------------------------------  WhatAreWeLinkedTo

short WhatAreWeLinkedTo (short where, Byte who)
{
	short		what, whatType;
	char		wasState;
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	what = (*thisHouse)->rooms[where].objects[who].what;
	HSetState((Handle)thisHouse, wasState);
	
	switch (what)
	{
		case kMailboxLf:
		whatType = kLinkedToLeftMailbox;
		break;
		
		case kMailboxRt:
		whatType = kLinkedToRightMailbox;
		break;
		
		case kCeilingTrans:
		whatType = kLinkedToCeilingDuct;
		break;
		
		default:
		whatType = kLinkedToOther;
		break;
	}
	
	return (whatType);
}

//--------------------------------------------------------------  ReadyGliderFromTransit

void ReadyGliderFromTransit (gliderPtr thisGlider, short toWhat)
{
	Rect		tempRect;
	
	if ((twoPlayerGame) && (onePlayerLeft) && (thisGlider->which == playerDead))
		return;
	
	FlagGliderNormal(thisGlider);
	
	switch (toWhat)
	{
		case kLinkedToOther:
		StartGliderTransportingIn(thisGlider);	// set glider's mode
		tempRect = thisGlider->dest;			// position glider
		CenterRectInRect(&tempRect, &transRect);
		thisGlider->dest.left = tempRect.left;
		thisGlider->dest.right = tempRect.right;
		thisGlider->dest.top = tempRect.top;
		thisGlider->dest.bottom = tempRect.bottom;
		thisGlider->destShadow.left = tempRect.left;
		thisGlider->destShadow.right = tempRect.right;
		thisGlider->whole = thisGlider->dest;
		thisGlider->wholeShadow = thisGlider->destShadow;
		thisGlider->enteredRect = thisGlider->dest;
		break;
		
		case kLinkedToLeftMailbox:
		StartGliderMailingOut(thisGlider);
		thisGlider->clip = transRect;		// fix clip
		thisGlider->clip.right -= 64;
		thisGlider->clip.bottom -= 25;
		tempRect = thisGlider->dest;
		thisGlider->dest.left = thisGlider->clip.right;
		thisGlider->dest.right = thisGlider->dest.left;
		thisGlider->dest.bottom = thisGlider->clip.bottom - 4;
		thisGlider->dest.top = thisGlider->dest.bottom - RectTall(&tempRect);
		thisGlider->destShadow.left = thisGlider->dest.left;
		thisGlider->destShadow.right = thisGlider->dest.right;
		thisGlider->whole = thisGlider->dest;
		thisGlider->wholeShadow = thisGlider->destShadow;
		break;
		
		case kLinkedToRightMailbox:
		StartGliderMailingOut(thisGlider);
		thisGlider->clip = transRect;		// fix clip
		thisGlider->clip.left += 79;
		thisGlider->clip.bottom -= 25;
		tempRect = thisGlider->dest;
		thisGlider->dest.right = thisGlider->clip.left;
		thisGlider->dest.left = thisGlider->dest.right;
		thisGlider->dest.bottom = thisGlider->clip.bottom - 4;
		thisGlider->dest.top = thisGlider->dest.bottom - RectTall(&tempRect);
		thisGlider->destShadow.left = thisGlider->dest.left;
		thisGlider->destShadow.right = thisGlider->dest.right;
		thisGlider->whole = thisGlider->dest;
		thisGlider->wholeShadow = thisGlider->destShadow;
		break;
		
		case kLinkedToCeilingDuct:
		StartGliderDuctingIn(thisGlider);
		tempRect = thisGlider->dest;		// set glider's position
		CenterRectInRect(&tempRect, &transRect);
		thisGlider->dest.left = tempRect.left;
		thisGlider->dest.right = tempRect.right;
		thisGlider->dest.top = tempRect.top;
		thisGlider->dest.bottom = thisGlider->dest.top;
		QOffsetRect(&thisGlider->dest, 0, -RectTall(&tempRect));
		thisGlider->destShadow.left = tempRect.left;
		thisGlider->destShadow.right = tempRect.right;
		thisGlider->whole = thisGlider->dest;
		thisGlider->wholeShadow = thisGlider->destShadow;
		break;
		
		case kLinkedToFloorDuct:
		break;
		
		default:
		break;
	}
	
	if ((twoPlayerGame) && (thisGlider->which != firstPlayer))
		TagGliderIdle(thisGlider);
}

//--------------------------------------------------------------  MoveRoomToRoom

void MoveRoomToRoom (gliderPtr thisGlider, short where)
{
	Rect		enterRect;
	
	HandleRoomVisitation();
	switch (where)
	{
		case kToRight:
		SetMusicalMode(kProdGameScoreMode);
		if (twoPlayerGame)
		{
			UndoGliderLimbo(&theGlider);
			UndoGliderLimbo(&theGlider2);
			InsureGliderFacingRight(&theGlider);
			InsureGliderFacingRight(&theGlider2);
		}
		else
			InsureGliderFacingRight(thisGlider);
		ForceThisRoom(localNumbers[kEastRoom]);
		if (twoPlayerGame)
		{
			OffsetGlider(&theGlider, kToLeft);
			OffsetGlider(&theGlider2, kToLeft);
			QSetRect(&enterRect, 0, 0, 48, 20);
			QOffsetRect(&enterRect, 0, 
					kGliderStartsDown + (short)thisRoom->leftStart - 2);
			theGlider.enteredRect = enterRect;
			theGlider2.enteredRect = enterRect;
		}
		else
		{
			OffsetGlider(thisGlider, kToLeft);
			QSetRect(&enterRect, 0, 0, 48, 20);
			QOffsetRect(&enterRect, 0, 
					kGliderStartsDown + (short)thisRoom->leftStart - 2);
			thisGlider->enteredRect = enterRect;
		}
		break;
		
		case kToLeft:
		SetMusicalMode(kProdGameScoreMode);
		if (twoPlayerGame)
		{
			UndoGliderLimbo(&theGlider);
			UndoGliderLimbo(&theGlider2);
			InsureGliderFacingLeft(&theGlider);
			InsureGliderFacingLeft(&theGlider2);
		}
		else
			InsureGliderFacingLeft(thisGlider);
		ForceThisRoom(localNumbers[kWestRoom]);
		if (twoPlayerGame)
		{
			OffsetGlider(&theGlider, kToRight);
			OffsetGlider(&theGlider2, kToRight);
			QSetRect(&enterRect, 0, 0, 48, 20);
			QOffsetRect(&enterRect, kRoomWide - 48, 
					kGliderStartsDown + (short)thisRoom->rightStart - 2);
			theGlider.enteredRect = enterRect;
			theGlider2.enteredRect = enterRect;
		}
		else
		{
			OffsetGlider(thisGlider, kToRight);
			QSetRect(&enterRect, 0, 0, 48, 20);
			QOffsetRect(&enterRect, kRoomWide - 48, 
					kGliderStartsDown + (short)thisRoom->rightStart - 2);
			thisGlider->enteredRect = enterRect;
		}
		break;
		
		case kAbove:
		SetMusicalMode(kKickGameScoreMode);
		ForceThisRoom(localNumbers[kNorthRoom]);
		if (!takingTheStairs)
		{
			if (twoPlayerGame)
			{
				UndoGliderLimbo(&theGlider);
				UndoGliderLimbo(&theGlider2);
				OffsetGlider(&theGlider, kBelow);
				OffsetGlider(&theGlider2, kBelow);
				theGlider.enteredRect = theGlider.dest;
				theGlider2.enteredRect = theGlider2.dest;
			}
			else
			{
				OffsetGlider(thisGlider, kBelow);
				thisGlider->enteredRect = thisGlider->dest;
			}
		}
		else
		{
			if (twoPlayerGame)
			{
				ReadyGliderForTripUpStairs(&theGlider);
				ReadyGliderForTripUpStairs(&theGlider2);
			}
			else
				ReadyGliderForTripUpStairs(thisGlider);
		}
		break;
		
		case kBelow:
		SetMusicalMode(kKickGameScoreMode);
		ForceThisRoom(localNumbers[kSouthRoom]);
		if (!takingTheStairs)
		{
			if (twoPlayerGame)
			{
				UndoGliderLimbo(&theGlider);
				UndoGliderLimbo(&theGlider2);
				OffsetGlider(&theGlider, kAbove);
				OffsetGlider(&theGlider2, kAbove);
				theGlider.enteredRect = theGlider.dest;
				theGlider2.enteredRect = theGlider2.dest;
			}
			else
			{
				OffsetGlider(thisGlider, kAbove);
				thisGlider->enteredRect = thisGlider->dest;
			}
		}
		else
		{
			if (twoPlayerGame)
			{
				ReadyGliderForTripDownStairs(&theGlider);
				ReadyGliderForTripDownStairs(&theGlider2);
			}
			else
				ReadyGliderForTripDownStairs(thisGlider);
		}
		break;
		
		default:
		break;
	}
	
	if ((twoPlayerGame) && (!onePlayerLeft))
	{
		if (firstPlayer == kPlayer1)
			TagGliderIdle(&theGlider2);
		else
			TagGliderIdle(&theGlider);
	}
	
	ReadyLevel();
	RefreshScoreboard(kNormalTitleMode);
	WipeScreenOn(where, &justRoomsRect);
	
#ifdef COMPILEQT
	RenderFrame();
	if ((thisMac.hasQT) && (hasMovie) && (tvInRoom) && (tvOn))
	{
		GoToBeginningOfMovie(theMovie);
		StartMovie(theMovie);
	}
#endif
}

//--------------------------------------------------------------  TransportRoomToRoom

void TransportRoomToRoom (gliderPtr thisGlider)
{
	Boolean		sameRoom;
	
	SetMusicalMode(kKickGameScoreMode);
	HandleRoomVisitation();
	
	sameRoom = (transRoom == thisRoomNumber);
	if (!sameRoom)
		ForceThisRoom(transRoom);
	if (twoPlayerGame)
	{
		UndoGliderLimbo(&theGlider);			// turn off limbo if needed
		UndoGliderLimbo(&theGlider2);			// turn off limbo if needed
		ReadyGliderFromTransit(&theGlider, linkedToWhat);
		ReadyGliderFromTransit(&theGlider2, linkedToWhat);
	}
	else
		ReadyGliderFromTransit(thisGlider, linkedToWhat);
	
	if (!sameRoom)
		ReadyLevel();
	RefreshScoreboard(kNormalTitleMode);
	if (!sameRoom)
		WipeScreenOn(kAbove, &justRoomsRect);
	
#ifdef COMPILEQT
	RenderFrame();
	if ((thisMac.hasQT) && (hasMovie) && (tvInRoom) && (tvOn))
	{
		GoToBeginningOfMovie(theMovie);
		StartMovie(theMovie);
	}
#endif
}

//--------------------------------------------------------------  MoveDuctToDuct

void MoveDuctToDuct (gliderPtr thisGlider)
{
	Boolean		sameRoom;
	
	SetMusicalMode(kKickGameScoreMode);
	HandleRoomVisitation();
	
	sameRoom = (transRoom == thisRoomNumber);
	if (!sameRoom)
		ForceThisRoom(transRoom);
	
	if (twoPlayerGame)
	{
		UndoGliderLimbo(&theGlider);			// turn off limbo if needed
		UndoGliderLimbo(&theGlider2);			// turn off limbo if needed
		ReadyGliderFromTransit(&theGlider, linkedToWhat);
		ReadyGliderFromTransit(&theGlider2, linkedToWhat);
	}
	else
		ReadyGliderFromTransit(thisGlider, linkedToWhat);
	
	if (!sameRoom)
		ReadyLevel();
	RefreshScoreboard(kNormalTitleMode);
	if (!sameRoom)
		WipeScreenOn(kAbove, &justRoomsRect);
	
#ifdef COMPILEQT
	RenderFrame();
	if ((thisMac.hasQT) && (hasMovie) && (tvInRoom) && (tvOn))
	{
		GoToBeginningOfMovie(theMovie);
		StartMovie(theMovie);
	}
#endif
}

//--------------------------------------------------------------  MoveMailToMail

void MoveMailToMail (gliderPtr thisGlider)
{
	Boolean		sameRoom;
	
	SetMusicalMode(kKickGameScoreMode);
	HandleRoomVisitation();
	
	sameRoom = (transRoom == thisRoomNumber);
	if (!sameRoom)
		ForceThisRoom(transRoom);
	
	if (twoPlayerGame)
	{
		UndoGliderLimbo(&theGlider);			// turn off limbo if needed
		UndoGliderLimbo(&theGlider2);			// turn off limbo if needed
		ReadyGliderFromTransit(&theGlider, linkedToWhat);
		ReadyGliderFromTransit(&theGlider2, linkedToWhat);
	}
	else
		ReadyGliderFromTransit(thisGlider, linkedToWhat);
	
	if (!sameRoom)
		ReadyLevel();
	RefreshScoreboard(kNormalTitleMode);
	if (!sameRoom)
		WipeScreenOn(kAbove, &justRoomsRect);
	
#ifdef COMPILEQT
	RenderFrame();
	if ((thisMac.hasQT) && (hasMovie) && (tvInRoom) && (tvOn))
	{
		GoToBeginningOfMovie(theMovie);
		StartMovie(theMovie);
	}
#endif
}

//--------------------------------------------------------------  HandleRoomVisitation

void HandleRoomVisitation (void)
{
	houseType	*thisHousePtr;
	char		wasState;
	
	if (!thisRoom->visited)
	{
		wasState = HGetState((Handle)thisHouse);
		HLock((Handle)thisHouse);
		thisHousePtr = *thisHouse;
		thisHousePtr->rooms[localNumbers[kCentralRoom]].visited = true;
		HSetState((Handle)thisHouse, wasState);
		theScore += kRoomVisitScore;
		thisRoom->visited = true;
	}
}

//--------------------------------------------------------------  ForceKillGlider

void ForceKillGlider (void)
{
	if (theGlider.mode == kGliderInLimbo)
	{
		if (theGlider2.mode != kGliderFadingOut)
		{
			StartGliderFadingOut(&theGlider2);
			PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
			playerSuicide = true;
		}
	}
	else if (theGlider2.mode == kGliderInLimbo)
	{
		if (theGlider.mode != kGliderFadingOut)
		{
			StartGliderFadingOut(&theGlider);
			PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
			playerSuicide = true;
		}
	}
}

//--------------------------------------------------------------  FollowTheLeader

void FollowTheLeader (void)
{
	short		wasEscaped;
	Boolean		oneOrTwo;
	
	playerSuicide = false;
	wasEscaped = otherPlayerEscaped;
	otherPlayerEscaped = kNoOneEscaped;
	
	if (theGlider.mode == kGliderInLimbo)
	{
		oneOrTwo = true;
		theGlider2.dest = theGlider.dest;
		theGlider2.destShadow = theGlider.destShadow;
		theGlider2.whole = theGlider2.dest;
		theGlider2.wholeShadow = theGlider2.destShadow;
	}
	else if (theGlider2.mode == kGliderInLimbo)
	{
		oneOrTwo = false;
		theGlider.dest = theGlider2.dest;
		theGlider.destShadow = theGlider2.destShadow;
		theGlider.whole = theGlider.dest;
		theGlider.wholeShadow = theGlider.destShadow;
	}
	
	switch (wasEscaped)
	{
		case kPlayerEscapedUp:
		case kPlayerEscapingUpStairs:
		case kPlayerEscapedUpStairs:
		if (oneOrTwo)
			MoveRoomToRoom(&theGlider2, kAbove);
		else
			MoveRoomToRoom(&theGlider, kAbove);
		break;
		
		case kPlayerEscapedDown:
		case kPlayerEscapingDownStairs:
		case kPlayerEscapedDownStairs:
		if (oneOrTwo)
			MoveRoomToRoom(&theGlider2, kBelow);
		else
			MoveRoomToRoom(&theGlider, kBelow);
		break;
		
		case kPlayerEscapedLeft:
		if (oneOrTwo)
			MoveRoomToRoom(&theGlider2, kToLeft);
		else
			MoveRoomToRoom(&theGlider, kToLeft);
		break;
		
		case kPlayerEscapedRight:
		if (oneOrTwo)
			MoveRoomToRoom(&theGlider2, kToRight);
		else
			MoveRoomToRoom(&theGlider, kToRight);
		break;
		
		case kPlayerTransportedOut:
		if (oneOrTwo)
			TransportRoomToRoom(&theGlider2);
		else
			TransportRoomToRoom(&theGlider);
		break;
		
		case kPlayerMailedOut:
		if (oneOrTwo)
			MoveMailToMail(&theGlider2);
		else
			MoveMailToMail(&theGlider);
		break;
		
		case kPlayerDuckedOut:
		if (oneOrTwo)
			MoveDuctToDuct(&theGlider2);
		else
			MoveDuctToDuct(&theGlider);
		break;
		
		default:
		break;
	}
}

