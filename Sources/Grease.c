
//============================================================================
//----------------------------------------------------------------------------
//								   Grease.c
//----------------------------------------------------------------------------
//============================================================================


#include "Externs.h"
#include "Environ.h"
#include "MainWindow.h"
#include "Objects.h"
#include "RectUtils.h"
#include "Room.h"


#define kGreaseIdle			0
#define kGreaseFalling		1
#define kGreaseSpreading	2
#define kGreaseSpiltIdle	3


void BackupGrease (Rect *, short, Boolean);


greasePtr	grease;
short		numGrease;

extern	hotPtr		hotSpots;
extern	savedType	savedMaps[];
extern	Point		shieldPt;
extern	Rect		greaseSrcRt[], greaseSrcLf[], shieldRect;


//==============================================================  Functions
//--------------------------------------------------------------  HandleGrease

// Goes through all grease objects currently on screen and handles…
// them.  If they're upright, nothing happens, but if they're in…
// the course of falling or spilling, this function will handle…
// advancing the spill, etc.

void HandleGrease (void)
{
	Rect		src;
	short		i;
	
	if (numGrease == 0)
		return;
	
	for (i = 0; i < numGrease; i++)
	{
		if (grease[i].mode == kGreaseFalling)
		{
			grease[i].frame++;
			if (grease[i].frame >= 3)		// grease completely tipped
			{
				grease[i].frame = 3;
				grease[i].mode = kGreaseSpreading;
				hotSpots[grease[i].hotNum].action = kSlideIt;
				hotSpots[grease[i].hotNum].isOn = true;
				if (grease[i].isRight)
					QSetRect(&src, 0, -2, 2, 0);
				else
					QSetRect(&src, -2, -2, 0, 0);
				QOffsetRect(&src, -playOriginH, -playOriginV);
				QOffsetRect(&src, grease[i].start, grease[i].dest.bottom);
				hotSpots[grease[i].hotNum].bounds = src;
			}
			
			QSetRect(&src, 0, 0, 32, 27);
			QOffsetRect(&src, 0, grease[i].frame * 27);
			CopyBits((BitMap *)*GetGWorldPixMap(savedMaps[grease[i].mapNum].map), 
					(BitMap *)*GetGWorldPixMap(workSrcMap), 
					&src, &grease[i].dest, 
					srcCopy, nil);
			CopyBits((BitMap *)*GetGWorldPixMap(savedMaps[grease[i].mapNum].map), 
					(BitMap *)*GetGWorldPixMap(backSrcMap), 
					&src, &grease[i].dest, 
					srcCopy, nil);
			
			AddRectToWorkRects(&grease[i].dest);
			if (grease[i].isRight)
				QOffsetRect(&grease[i].dest, 2, 0);
			else
				QOffsetRect(&grease[i].dest, -2, 0);
		}
		else if (grease[i].mode == kGreaseSpreading)
		{
			if (grease[i].isRight)
			{
				QSetRect(&src, 0, -2, 2, 0);
				QOffsetRect(&src, grease[i].start, grease[i].dest.bottom);
				grease[i].start += 2;
				hotSpots[grease[i].hotNum].bounds.right += 2;
			}
			else
			{
				QSetRect(&src, -2, -2, 0, 0);
				QOffsetRect(&src, grease[i].start, grease[i].dest.bottom);
				grease[i].start -= 2;
				hotSpots[grease[i].hotNum].bounds.left -= 2;
			}
			
			{
				CGrafPtr	wasCPort;
				GDHandle	wasWorld;
				
				GetGWorld(&wasCPort, &wasWorld);
				
				SetGWorld(backSrcMap, nil);
				PaintRect(&src);
				
				SetGWorld(workSrcMap, nil);
				PaintRect(&src);
				AddRectToWorkRects(&src);
				
				SetGWorld(wasCPort, wasWorld);
			}
			
			if (grease[i].isRight)
			{
				if (grease[i].start >= grease[i].stop)
					grease[i].mode = kGreaseSpiltIdle;
			}
			else
			{
				if (grease[i].start <= grease[i].stop)
					grease[i].mode = kGreaseSpiltIdle;
			}
		}
	}
}

//--------------------------------------------------------------  BackupGrease
// This makes copies of the region of the screen the grease is about…
// to be drawn to.  It is called in the "set up" when a player has…
// just entered a new room.  The "grease jar falling over" animation…
// is set up here.

void BackupGrease (Rect *src, short index, Boolean isRight)
{
	Rect		dest;
	short		i;
	
	QSetRect(&dest, 0, 0, 32, 27);
	for (i = 0; i < 4; i++)
	{
		CopyBits((BitMap *)*GetGWorldPixMap(backSrcMap), 
				(BitMap *)*GetGWorldPixMap(savedMaps[index].map), 
				src, &dest, srcCopy, nil);
		
		if (isRight)
		{
			CopyMask((BitMap *)*GetGWorldPixMap(bonusSrcMap), 
					(BitMap *)*GetGWorldPixMap(bonusMaskMap), 
					(BitMap *)*GetGWorldPixMap(savedMaps[index].map), 
					&greaseSrcRt[i], &greaseSrcRt[i], &dest);
			QOffsetRect(src, 2, 0);
		}
		else
		{
			CopyMask((BitMap *)*GetGWorldPixMap(bonusSrcMap), 
					(BitMap *)*GetGWorldPixMap(bonusMaskMap), 
					(BitMap *)*GetGWorldPixMap(savedMaps[index].map), 
					&greaseSrcLf[i], &greaseSrcLf[i], &dest);
			QOffsetRect(src, -2, 0);
		}
		QOffsetRect(&dest, 0, 27);
	}

}

//--------------------------------------------------------------  ReBackUpGrease
// Just like th eabove function but it is called while the player is…
// active in a room and has changed the lighting situation (like turned…
// off or on the lights).  It assumes certain data strucutures are…
// already declared from an earlier call to the above funciton.

short ReBackUpGrease (short where, short who)
{
	Rect		src;
	short		i;
	
	for (i = 0; i < numGrease; i++)
	{
		if ((grease[i].where == where) && (grease[i].who == who))
		{
			if ((grease[i].mode == kGreaseIdle) || (grease[i].mode == kGreaseFalling))
			{
				src = grease[i].dest;
				BackupGrease(&src, grease[i].mapNum, grease[i].isRight);
			}
			return (i);
		}
	}
	
	return (-1);
}

//--------------------------------------------------------------  AddGrease

// Called when a new room is being set up during a game.  This adds…
// another jar of grease to the queue of jars to be handled.

short AddGrease (short where, short who, short h, short v, 
		short distance, Boolean isRight)
{
	Rect		src, bounds;
	short		savedNum;
	
	if (numGrease >= kMaxGrease)
		return (-1);
	
	QSetRect(&src, 0, 0, 32, 27);
	QOffsetRect(&src, h, v);
	
	QSetRect(&bounds, 0, 0, 32, 27 * 4);
	savedNum = BackUpToSavedMap(&bounds, where, who);
	if (savedNum != -1)
	{
		BackupGrease (&src, savedNum, isRight);
		if (isRight)
			QOffsetRect(&src, -8, 0);
		else
			QOffsetRect(&src, 8, 0);
		grease[numGrease].who = who;
		grease[numGrease].where = where;
		grease[numGrease].dest = src;
		grease[numGrease].mapNum = savedNum;
		grease[numGrease].mode = kGreaseIdle;
		grease[numGrease].frame = -1;
		if (isRight)
		{
			grease[numGrease].isRight = true;
			grease[numGrease].start = src.right + 4;
			grease[numGrease].stop = src.right + distance;
		}
		else
		{
			grease[numGrease].isRight = false;
			grease[numGrease].start = src.left - 4;
			grease[numGrease].stop = src.left - distance;
		}
		numGrease++;
		
		return (numGrease - 1);
	}
	else
		return (-1);
}

//--------------------------------------------------------------  SpillGrease

// A player has knocked a jar of grease over - this function flags that.

void SpillGrease (short who, short index)
{
	if (grease[who].mode == kGreaseIdle)
	{
		grease[who].mode = kGreaseFalling;
		grease[who].hotNum = index;
		PlayPrioritySound(kGreaseSpillSound, kGreaseSpillPriority);
	}
}

//--------------------------------------------------------------  RedrawAllGrease
// Called to redraw all the black lines of spilt grease.

void RedrawAllGrease (void)
{
	CGrafPtr	wasCPort;
	GDHandle	wasWorld;
	Rect		src;
	short		i;
	
	if (numGrease == 0)
		return;
	
	for (i = 0; i < numGrease; i++)
	{
		src = hotSpots[grease[i].hotNum].bounds;
		if ((grease[i].where == thisRoomNumber) && 
				((src.bottom - src.top) == 2) && 
				(grease[i].mode != kGreaseIdle))
		{
			QOffsetRect(&src, playOriginH, playOriginV);
			
			GetGWorld(&wasCPort, &wasWorld);
			
			SetGWorld(backSrcMap, nil);
			PaintRect(&src);
			
			SetGWorld(workSrcMap, nil);
			PaintRect(&src);
			AddRectToWorkRects(&src);
			
			SetGWorld(wasCPort, wasWorld);
		}
	}
}

