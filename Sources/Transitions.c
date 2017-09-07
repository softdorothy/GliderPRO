
//============================================================================
//----------------------------------------------------------------------------
//								 Transitions.c
//----------------------------------------------------------------------------
//============================================================================


#include "Externs.h"
#include "Environ.h"
#include "MainWindow.h"
#include "RectUtils.h"


//==============================================================  Functions
//--------------------------------------------------------------  PourScreenOn

void PourScreenOn (Rect *theRect)
{
	#define		kMaxColumnsWide	96
	#define		kChipHigh		20
	#define		kChipWide		16
	Rect		columnRects[kMaxColumnsWide];
	short		columnProgress[kMaxColumnsWide];
	short		i, colsComplete, colWide, rowTall;
	Boolean		working;
	
	colWide = theRect->right / kChipWide;			// determine # of cols
	rowTall = (theRect->bottom / kChipHigh) + 1;	// determine # of rows
	
	working = true;
	colsComplete = 0;
	for (i = 0; i < colWide; i++)
	{
		columnProgress[i] = 0;
		QSetRect(&columnRects[i], 0, 0, kChipWide, kChipHigh);
		QOffsetRect(&columnRects[i], (i * kChipWide) + theRect->left, theRect->top);
	}
	
	while (working)
	{
		do
		{
			i = RandomInt(colWide);
		}
		while (columnProgress[i] >= rowTall);
		
		if (columnRects[i].left < theRect->left)
			columnRects[i].left = theRect->left;
		if (columnRects[i].top < theRect->top)
			columnRects[i].top = theRect->top;
		if (columnRects[i].right > theRect->right)
			columnRects[i].right = theRect->right;
		if (columnRects[i].bottom > theRect->bottom)
			columnRects[i].bottom = theRect->bottom;
		
		CopyBits((BitMap *)*GetGWorldPixMap(workSrcMap), 
				GetPortBitMapForCopyBits(GetWindowPort(mainWindow)), 
				&columnRects[i], &columnRects[i], srcCopy, nil);
				
		QOffsetRect(&columnRects[i], 0, kChipHigh);
		columnProgress[i]++;
		if (columnProgress[i] >= rowTall)
		{
			colsComplete++;
			if (colsComplete >= colWide)
				working = false;
		}
	}
}

//--------------------------------------------------------------  WipeScreenOn

void WipeScreenOn (short direction, Rect *theRect)
{
	#define		kWipeRectThick	4
	Rect		wipeRect;
	RgnHandle	dummyRgn;
	short		hOffset, vOffset;
	short		i, count;
	
	wipeRect = *theRect;
	switch (direction)
	{
		case kAbove:
		wipeRect.bottom = wipeRect.top + kWipeRectThick;
		hOffset = 0;
		vOffset = kWipeRectThick;
		count = ((theRect->bottom - theRect->top) / kWipeRectThick) + 1;
		break;
		
		case kToRight:
		wipeRect.left = wipeRect.right - kWipeRectThick;
		hOffset = -kWipeRectThick;
		vOffset = 0;
		count = workSrcRect.right / kWipeRectThick;
		break;
		
		case kBelow:
		wipeRect.top = wipeRect.bottom - kWipeRectThick;
		hOffset = 0;
		vOffset = -kWipeRectThick;
		count = ((theRect->bottom - theRect->top) / kWipeRectThick) + 1;
		break;
		
		case kToLeft:
		wipeRect.right = wipeRect.left + kWipeRectThick;
		hOffset = kWipeRectThick;
		vOffset = 0;
		count = workSrcRect.right / kWipeRectThick;
		break;
	}
	
	dummyRgn = NewRgn();
	
	for (i = 0; i < count; i++)
	{
		CopyBits((BitMap *)*GetGWorldPixMap(workSrcMap), 
				GetPortBitMapForCopyBits(GetWindowPort(mainWindow)), 
				&wipeRect, &wipeRect, srcCopy, GetPortVisibleRegion(GetWindowPort(mainWindow), dummyRgn));
		
		QOffsetRect(&wipeRect, hOffset, vOffset);
		
		if (wipeRect.top < theRect->top)
			wipeRect.top = theRect->top;
		else if (wipeRect.top > theRect->bottom)
			wipeRect.top = theRect->bottom;
		if (wipeRect.bottom < theRect->top)
			wipeRect.bottom = theRect->top;
		else if (wipeRect.bottom > theRect->bottom)
			wipeRect.bottom = theRect->bottom;
	}
	
	DisposeRgn(dummyRgn);
}

//--------------------------------------------------------------  DumpScreenOn

void DumpScreenOn (Rect *theRect)
{
	CopyBits((BitMap *)*GetGWorldPixMap(workSrcMap), 
			GetPortBitMapForCopyBits(GetWindowPort(mainWindow)), 
			theRect, theRect, srcCopy, nil);
}

