
//============================================================================
//----------------------------------------------------------------------------
//								 ObjectDraw.c
//----------------------------------------------------------------------------
//============================================================================


#include "Externs.h"
#include "Environ.h"
#include "Objects.h"
#include "RectUtils.h"
#include "Room.h"


#define k8WhiteColor			0
#define kYellowColor			5
#define kGoldColor				11
#define k8RedColor				35
#define kPaleVioletColor		42
#define k8LtTanColor			52
#define k8BambooColor			53
#define kDarkFleshColor			58
#define k8TanColor				94
#define k8PissYellowColor		95
#define k8OrangeColor			59
#define k8BrownColor			137
#define k8Red4Color				143
#define k8SkyColor				150
#define k8EarthBlueColor		170
#define k8DkRedColor			222
#define k8DkRed2Color			223
#define kIntenseGreenColor		225
#define kIntenseBlueColor		235
#define k8PumpkinColor			101
#define k8LtstGrayColor			245
#define k8LtstGray2Color		246
#define k8LtstGray3Color		43
#define k8LtstGray4Color		247
#define k8LtstGray5Color		248
#define k8LtGrayColor			249
#define k8GrayColor				250
#define k8Gray2Color			251
#define k8DkGrayColor			252
#define k8DkGray2Color			253
#define k8DkGray3Color			172
#define k8DkstGrayColor			254
#define k8BlackColor			255


void DrawClockDigit (short, Rect *);
void DrawClockHands (Point, short, short);
void DrawLargeClockHands (Point, short, short);


//==============================================================  Functions
//--------------------------------------------------------------  DrawSimpleBlowers

void DrawSimpleBlowers (short what, Rect *theRect)
{
	CopyMask((BitMap *)*GetGWorldPixMap(blowerSrcMap), 
			(BitMap *)*GetGWorldPixMap(blowerMaskMap), 
			(BitMap *)*GetGWorldPixMap(backSrcMap), 
			&srcRects[what], &srcRects[what], theRect);
}

//--------------------------------------------------------------  DrawTiki

void DrawTiki (Rect *theRect, short down)
{
#define kTikiPoleBase	300
	long		darkGrayC, lightWoodC, darkWoodC;
	CGrafPtr	wasCPort;
	GDHandle	wasWorld;
	
	GetGWorld(&wasCPort, &wasWorld);
	SetGWorld(backSrcMap, nil);
	
	if (thisMac.isDepth == 4)
	{
		darkGrayC = 14;
		lightWoodC = 6;
		darkWoodC = 9;
	}
	else
	{
		darkGrayC = k8DkstGrayColor;
		lightWoodC = k8BambooColor;
		darkWoodC = k8PissYellowColor;
	}
	
	if (theRect->bottom < kTikiPoleBase + down)
	{
		ColorLine(theRect->left + 11, theRect->bottom - 1, 
				theRect->left + 11, kTikiPoleBase + down - 1, darkGrayC);
		ColorLine(theRect->left + 12, theRect->bottom - 1, 
				theRect->left + 12, kTikiPoleBase + down, lightWoodC);
		ColorLine(theRect->left + 13, theRect->bottom - 1, 
				theRect->left + 13, kTikiPoleBase + down, darkWoodC);
		ColorLine(theRect->left + 14, theRect->bottom - 1, 
				theRect->left + 14, kTikiPoleBase + down, darkWoodC);
		ColorLine(theRect->left + 15, theRect->bottom - 1, 
				theRect->left + 15, kTikiPoleBase + down - 1, darkGrayC);
	}
	
	SetGWorld(wasCPort, wasWorld);
	
	CopyMask((BitMap *)*GetGWorldPixMap(blowerSrcMap), 
			(BitMap *)*GetGWorldPixMap(blowerMaskMap), 
			(BitMap *)*GetGWorldPixMap(backSrcMap), 
			&srcRects[kTiki], &srcRects[kTiki], theRect);
}

//--------------------------------------------------------------  DrawInvisibleBlower

void DrawInvisibleBlower (Rect *theRect)
{
	Rect		tempRect;
	CGrafPtr	wasCPort;
	GDHandle	wasWorld;

	GetGWorld(&wasCPort, &wasWorld);
	SetGWorld(backSrcMap, nil);
	
	QSetRect(&tempRect, 0, 0, 24, 24);
	QOffsetRect(&tempRect, theRect->left, theRect->top);
	
	ColorFrameRect(&tempRect, 192);
	SetGWorld(wasCPort, wasWorld);
}

//--------------------------------------------------------------  DrawLiftArea

void DrawLiftArea (Rect *theRect)
{	
	CGrafPtr	wasCPort;
	GDHandle	wasWorld;
	
	GetGWorld(&wasCPort, &wasWorld);
	SetGWorld(backSrcMap, nil);
	ColorFrameRect(theRect, 192);
	SetGWorld(wasCPort, wasWorld);
}

//--------------------------------------------------------------  DrawTable

void DrawTable (Rect *tableTop, short down)
{
	#define		kTableBaseTop		296
	#define		kTableShadowTop		312
	#define		kTableShadowOffset	12
	Rect		tempRect;
	long		brownC, tanC, dkRedC, blackC;
	short		hCenter, vShadow;
	CGrafPtr	wasCPort;
	GDHandle	wasWorld;
	Pattern		dummyPattern;
	
	if (thisMac.isDepth == 4)
	{
		brownC = 11;
		tanC = 9;
		dkRedC = 14;
		blackC = 15;
	}
	else
	{
		brownC = k8BrownColor;
		tanC = k8TanColor;
		dkRedC = k8DkRed2Color;
		blackC = k8BlackColor;
	}
	
	GetGWorld(&wasCPort, &wasWorld);
	SetGWorld(backSrcMap, nil);
	
	QSetRect(&tempRect, tableTop->left, 0, tableTop->right, 
			RectWide(tableTop) / 10);
	QOffsetRect(&tempRect, 0, 
			-HalfRectTall(&tempRect) + kTableShadowTop + down);
	QOffsetRect(&tempRect, kTableShadowOffset, -kTableShadowOffset);
	PenPat(GetQDGlobalsGray(&dummyPattern));
	PenMode(patOr);
	if (thisMac.isDepth == 4)
		ColorOval(&tempRect, 15);
	else
		ColorOval(&tempRect, k8DkstGrayColor);
	PenNormal();
	
	InsetRect(tableTop, 0, 1);
	ColorRect(tableTop, brownC);
	InsetRect(tableTop, 0, -1);
	
	ColorLine(tableTop->left, tableTop->top + 1, 
			tableTop->left, tableTop->top + 1, k8WhiteColor);
	ColorLine(tableTop->left + 1, tableTop->top, 
			tableTop->right - 2, tableTop->top, k8WhiteColor);
	ColorLine(tableTop->right - 1, tableTop->top + 1, 
			tableTop->right - 1, tableTop->top + 1, k8WhiteColor);
	
	ColorLine(tableTop->left + 1, tableTop->top + 1, 
			tableTop->right - 2, tableTop->top + 1, tanC);
	ColorLine(tableTop->left, tableTop->top + 2, 
			tableTop->left, tableTop->bottom - 2, tanC);
	
	ColorLine(tableTop->left + 1, tableTop->bottom - 1, 
			tableTop->right - 2, tableTop->bottom - 1, blackC);
	ColorLine(tableTop->right - 1, tableTop->top + 2, 
			tableTop->right - 1, tableTop->bottom - 2, blackC);
	
	ColorLine(tableTop->left + 1, tableTop->bottom - 2, 
			tableTop->right - 2, tableTop->bottom - 2, dkRedC);
	
	if (tableTop->bottom < kTableBaseTop + down)
	{
		hCenter = (tableTop->left + tableTop->right) / 2;
		
		ColorLine(hCenter - 3, tableTop->bottom, 
				hCenter - 3, kTableBaseTop + down, blackC);
		ColorLine(hCenter - 2, tableTop->bottom, 
				hCenter - 2, kTableBaseTop + down, k8LtGrayColor);
		ColorLine(hCenter - 1, tableTop->bottom, 
				hCenter - 1, kTableBaseTop + down, k8GrayColor);
		ColorLine(hCenter, tableTop->bottom, 
				hCenter, kTableBaseTop + down, k8DkGrayColor);
		ColorLine(hCenter + 1, tableTop->bottom, 
				hCenter + 1, kTableBaseTop + down, blackC);
	
		vShadow = tableTop->bottom + RectWide(tableTop) / 4 - 2;
		if (vShadow > kTableBaseTop + down)
		{
			ColorLine(hCenter - 2, tableTop->bottom, 
					hCenter - 2, kTableBaseTop + down, k8DkGrayColor);
			ColorLine(hCenter - 1, tableTop->bottom, 
					hCenter - 1, kTableBaseTop + down, k8DkGrayColor);
			ColorLine(hCenter, tableTop->bottom, 
					hCenter, kTableBaseTop + down, blackC);
		}
		else
		{
			ColorLine(hCenter - 2, tableTop->bottom, 
					hCenter - 2, vShadow, k8DkGrayColor);
			ColorLine(hCenter - 1, tableTop->bottom, 
					hCenter - 1, vShadow + 1, k8DkGrayColor);
			ColorLine(hCenter, tableTop->bottom, 
					hCenter, vShadow + 2, blackC);
		}
	}
	
	SetGWorld(wasCPort, wasWorld);
	
	tempRect = tableSrc;
	QOffsetRect(&tempRect, -HalfRectWide(&tableSrc) + tableTop->left + 
			HalfRectWide(tableTop), kTableBaseTop + down);
	CopyMask((BitMap *)*GetGWorldPixMap(furnitureSrcMap), 
			(BitMap *)*GetGWorldPixMap(furnitureMaskMap), 
			(BitMap *)*GetGWorldPixMap(backSrcMap), 
			&tableSrc, &tableSrc, &tempRect);
}

//--------------------------------------------------------------  DrawShelf

void DrawShelf (Rect *shelfTop)
{
	#define		kBracketInset		18
	#define		kShelfDeep			4
	#define		kBracketThick		5
	#define		kShelfShadowOff		12
	Rect		tempRect;
	long		brownC, ltTanC, tanC, dkRedC, blackC;
	RgnHandle	shadowRgn;
	CGrafPtr	wasCPort;
	GDHandle	wasWorld;
	Pattern		dummyPattern;
	
	if (thisMac.isDepth == 4)
	{
		brownC = 11;
		ltTanC = 7;
		tanC = 9;
		dkRedC = 14;
		blackC = 15;
	}
	else
	{
		brownC = k8BrownColor;
		ltTanC = k8LtTanColor;
		tanC = k8TanColor;
		dkRedC = k8DkRed2Color;
		blackC = k8BlackColor;
	}
	
	GetGWorld(&wasCPort, &wasWorld);
	SetGWorld(backSrcMap, nil);
	
	MoveTo(shelfTop->left, shelfTop->bottom);
	shadowRgn = NewRgn();
	if (shadowRgn == nil)
		RedAlert(kErrUnnaccounted);
	OpenRgn();
	Line(kShelfShadowOff, kShelfShadowOff);
	Line(RectWide(shelfTop) - kShelfDeep, 0);
	Line(0, -kShelfThick + 1);
	Line(-kShelfShadowOff, -kShelfShadowOff);
	LineTo(shelfTop->left, shelfTop->bottom);
	CloseRgn(shadowRgn);
	PenPat(GetQDGlobalsGray(&dummyPattern));
	PenMode(patOr);
	if (thisMac.isDepth == 4)
		ColorRegion(shadowRgn, 15);
	else
		ColorRegion(shadowRgn, k8DkstGrayColor);
	PenNormal();
	DisposeRgn(shadowRgn);
	
	InsetRect(shelfTop, 0, 1);
	ColorRect(shelfTop, brownC);
	InsetRect(shelfTop, 0, -1);
	
	ColorLine(shelfTop->left + 1, shelfTop->top, 
			shelfTop->left + 1 + kShelfDeep, shelfTop->top, ltTanC);
	ColorLine(shelfTop->left, shelfTop->top + 1, 
			shelfTop->left + kShelfDeep, shelfTop->top + 1, tanC);
	ColorLine(shelfTop->left, shelfTop->top + 2, 
			shelfTop->left + kShelfDeep, shelfTop->top + 2, tanC);
	ColorLine(shelfTop->left, shelfTop->top + 3, 
			shelfTop->left + kShelfDeep, shelfTop->top + 3, tanC);
	ColorLine(shelfTop->left + 1, shelfTop->bottom - 1, 
			shelfTop->left + 1 + kShelfDeep, shelfTop->bottom - 1, dkRedC);
	ColorLine(shelfTop->left + 2 + kShelfDeep, shelfTop->bottom - 1, 
			shelfTop->right - 2, shelfTop->bottom - 1, blackC);
	ColorLine(shelfTop->left + 2 + kShelfDeep, shelfTop->top, 
			shelfTop->right - 2, shelfTop->top, tanC);
	ColorLine(shelfTop->right - 1, shelfTop->top + 1, 
			shelfTop->right - 1, shelfTop->bottom - 2, blackC);
	
	SetGWorld(wasCPort, wasWorld);
	
	tempRect = shelfSrc;
	ZeroRectCorner(&tempRect);
	QOffsetRect(&tempRect, shelfTop->left + kBracketInset, shelfTop->bottom);
	CopyMask((BitMap *)*GetGWorldPixMap(furnitureSrcMap), 
			(BitMap *)*GetGWorldPixMap(furnitureMaskMap), 
			(BitMap *)*GetGWorldPixMap(backSrcMap), 
			&shelfSrc, &shelfSrc, &tempRect);
	
	ZeroRectCorner(&tempRect);
	QOffsetRect(&tempRect, shelfTop->right - kBracketInset - kShelfDeep - 
			kBracketThick, shelfTop->bottom);
	CopyMask((BitMap *)*GetGWorldPixMap(furnitureSrcMap), 
			(BitMap *)*GetGWorldPixMap(furnitureMaskMap), 
			(BitMap *)*GetGWorldPixMap(backSrcMap), 
			&shelfSrc, &shelfSrc, &tempRect);
}

//--------------------------------------------------------------  DrawCabinet

void DrawCabinet (Rect *cabinet)
{
	#define		kCabinetDeep		4
	#define		kCabinetShadowOff	6
	Rect		tempRect;
	long		brownC, dkGrayC, ltTanC, tanC, dkRedC, blackC;
	RgnHandle	shadowRgn;
	CGrafPtr	wasCPort;
	GDHandle	wasWorld;
	Pattern		dummyPattern;
	
	if (thisMac.isDepth == 4)
	{
		brownC = 11;
		dkGrayC = 14;
		ltTanC = 7;
		tanC = 9;
		dkRedC = 14;
		blackC = 15;
	}
	else
	{
		brownC = k8BrownColor;
		dkGrayC = k8DkstGrayColor;
		ltTanC = k8LtTanColor;
		tanC = k8TanColor;
		dkRedC = k8DkRed2Color;
		blackC = k8BlackColor;
	}
	
	GetGWorld(&wasCPort, &wasWorld);
	SetGWorld(backSrcMap, nil);
	
	MoveTo(cabinet->left, cabinet->bottom);
	shadowRgn = NewRgn();
	if (shadowRgn == nil)
		RedAlert(kErrUnnaccounted);
	OpenRgn();
	Line(kCabinetShadowOff, kCabinetShadowOff);
	Line(RectWide(cabinet), 0);
	Line(0, -RectTall(cabinet) + kCabinetDeep);
	Line(-kCabinetShadowOff, -kCabinetShadowOff);
	LineTo(cabinet->left, cabinet->bottom);
	CloseRgn(shadowRgn);
	PenPat(GetQDGlobalsGray(&dummyPattern));
	PenMode(patOr);
	if (thisMac.isDepth == 4)
		ColorRegion(shadowRgn, 15);
	else
		ColorRegion(shadowRgn, dkGrayC);
	PenNormal();
	DisposeRgn(shadowRgn);
	
	InsetRect(cabinet, 1, 1);		// fill bulk of cabinet brown
	ColorRect(cabinet, brownC);
	InsetRect(cabinet, -1, -1);
	
	tempRect = *cabinet;			// add lighter left side
	tempRect.right = tempRect.left + kCabinetDeep;
	ColorRect(&tempRect, tanC);
									// hilight top edge
	ColorLine(cabinet->left + 1, cabinet->top + 1, 
			cabinet->left + kCabinetDeep, cabinet->top + 1, ltTanC);
	ColorLine(cabinet->left + kCabinetDeep, cabinet->top + 1, 
			cabinet->right - 3, cabinet->top + 1, tanC);
									// shadow bottom edge
	
	ColorLine(cabinet->left + kCabinetDeep + 3, cabinet->top + 5, 
			cabinet->left + kCabinetDeep + 3, cabinet->bottom - 6, tanC);
	ColorLine(cabinet->left + kCabinetDeep + 4, cabinet->top + 5, 
			cabinet->left + kCabinetDeep + 4, cabinet->bottom - 6, tanC);
	ColorLine(cabinet->left + kCabinetDeep + 9, cabinet->top + 10, 
			cabinet->left + kCabinetDeep + 9, cabinet->bottom - 11, dkGrayC);
	
	ColorLine(cabinet->right - 4, cabinet->top + 6, 
			cabinet->right - 4, cabinet->bottom - 5, dkRedC);
	ColorLine(cabinet->right - 5, cabinet->top + 5, 
			cabinet->right - 5, cabinet->bottom - 6, dkGrayC);
	ColorLine(cabinet->right - 10, cabinet->top + 10, 
			cabinet->right - 10, cabinet->bottom - 11, tanC);
	
	ColorLine(cabinet->left + kCabinetDeep + 4, cabinet->top + 4, 
			cabinet->left + kCabinetDeep + 4, cabinet->top + 4, ltTanC);
	ColorLine(cabinet->left + kCabinetDeep + 5, cabinet->top + 4, 
			cabinet->right - 6, cabinet->top + 4, tanC);
	ColorLine(cabinet->left + kCabinetDeep + 10, cabinet->top + 9, 
			cabinet->right - 11, cabinet->top + 9, dkGrayC);
	
	ColorLine(cabinet->right - 5, cabinet->bottom - 5, 
			cabinet->right - 5, cabinet->bottom - 5, dkRedC);
	ColorLine(cabinet->left + kCabinetDeep + 6, cabinet->bottom - 4, 
			cabinet->right - 5, cabinet->bottom - 4, dkRedC);
	ColorLine(cabinet->left + kCabinetDeep + 5, cabinet->bottom - 5, 
			cabinet->right - 6, cabinet->bottom - 5, dkGrayC);
	
	ColorLine(cabinet->left + kCabinetDeep + 10, cabinet->bottom - 10, 
			cabinet->right - 11, cabinet->bottom - 10, tanC);
	
	SetGWorld(wasCPort, wasWorld);
	
	tempRect = hingeSrc;
	ZeroRectCorner(&tempRect);
	QOffsetRect(&tempRect, cabinet->left + kCabinetDeep + 2, cabinet->top + 10);
	CopyMask((BitMap *)*GetGWorldPixMap(furnitureSrcMap), 
			(BitMap *)*GetGWorldPixMap(furnitureMaskMap), 
			(BitMap *)*GetGWorldPixMap(backSrcMap), 
			&hingeSrc, &hingeSrc, &tempRect);
	
	tempRect = hingeSrc;
	ZeroRectCorner(&tempRect);
	QOffsetRect(&tempRect, cabinet->left + kCabinetDeep + 2, cabinet->bottom - 26);
	CopyMask((BitMap *)*GetGWorldPixMap(furnitureSrcMap), 
			(BitMap *)*GetGWorldPixMap(furnitureMaskMap), 
			(BitMap *)*GetGWorldPixMap(backSrcMap), 
			&hingeSrc, &hingeSrc, &tempRect);
	
	tempRect = handleSrc;
	ZeroRectCorner(&tempRect);
	QOffsetRect(&tempRect, cabinet->right - 8, cabinet->top + 
			HalfRectTall(cabinet) - HalfRectTall(&handleSrc));
	CopyMask((BitMap *)*GetGWorldPixMap(furnitureSrcMap), 
			(BitMap *)*GetGWorldPixMap(furnitureMaskMap), 
			(BitMap *)*GetGWorldPixMap(backSrcMap), 
			&handleSrc, &handleSrc, &tempRect);
	
	FrameRect(cabinet);
}

//--------------------------------------------------------------  DrawSimpleFurniture

void DrawSimpleFurniture (short what, Rect *theRect)
{
	CopyMask((BitMap *)*GetGWorldPixMap(furnitureSrcMap), 
			(BitMap *)*GetGWorldPixMap(furnitureMaskMap), 
			(BitMap *)*GetGWorldPixMap(backSrcMap), 
			&srcRects[what], &srcRects[what], theRect);
}

//--------------------------------------------------------------  DrawCounter

void DrawCounter (Rect *counter)
{
	#define		kCounterFooterHigh	12
	#define		kCounterStripWide	6
	#define		kCounterStripTall	29
	#define		kCounterPanelDrop	12
	Rect		tempRect;
	RgnHandle	shadowRgn;
	long		brownC, dkGrayC, tanC, blackC, dkstRedC;
	short		nRects, width, i;
	CGrafPtr	wasCPort;
	GDHandle	wasWorld;
	Pattern		dummyPattern;
	
	if (thisMac.isDepth == 4)
	{
		brownC = 11;
		dkGrayC = 14;
		tanC = 9;
		blackC = 15;
		dkstRedC = 15;
	}
	else
	{
		brownC = k8BrownColor;
		dkGrayC = k8DkstGrayColor;
		tanC = k8TanColor;
		blackC = k8BlackColor;
		dkstRedC = k8DkRed2Color;
	}
	
	GetGWorld(&wasCPort, &wasWorld);
	SetGWorld(backSrcMap, nil);
	
	MoveTo(counter->right - 2, counter->bottom);
	shadowRgn = NewRgn();
	if (shadowRgn == nil)
		RedAlert(kErrUnnaccounted);
	OpenRgn();
	Line(10, -10);
	Line(0, -RectTall(counter) + 29);
	Line(2, 0);
	Line(0, -7);
	Line(-12, -12);
	LineTo(counter->right - 2, counter->bottom);
	CloseRgn(shadowRgn);
	PenPat(GetQDGlobalsGray(&dummyPattern));
	PenMode(patOr);
	if (thisMac.isDepth == 4)
		ColorRegion(shadowRgn, 15);
	else
		ColorRegion(shadowRgn, dkGrayC);
	PenNormal();
	DisposeRgn(shadowRgn);
	
	InsetRect(counter, 2, 2);
	ColorRect(counter, brownC);
	InsetRect(counter, -2, -2);
	
	tempRect = *counter;
	tempRect.top = tempRect.bottom - kCounterFooterHigh;
	tempRect.left += 2;
	tempRect.right -= 2;
	ColorRect(&tempRect, dkGrayC);
	ColorLine(counter->left + 2, counter->bottom - kCounterFooterHigh, 
			counter->right - 3, counter->bottom - kCounterFooterHigh, blackC);
	ColorLine(counter->left + 2, counter->bottom - kCounterFooterHigh + 1, 
			counter->right - 3, counter->bottom - kCounterFooterHigh + 1, blackC);
	ColorLine(counter->right - 3, counter->bottom - kCounterFooterHigh, 
			counter->right - 3, counter->bottom - 1, blackC);
	ColorLine(counter->left + 2, counter->bottom - kCounterFooterHigh, 
			counter->left + 2, counter->bottom - 1, k8DkGrayColor);
	
	ColorLine(counter->right - 2, counter->top, 
			counter->right - 2, counter->bottom - kCounterFooterHigh - 1, dkstRedC);
	ColorLine(counter->left + 1, counter->top + 8, 
			counter->left + 1, counter->bottom - kCounterFooterHigh - 1, tanC);
	
	if (thisMac.isDepth == 4)
	{
		ColorLine(counter->left - 1, counter->top, 
				counter->right, counter->top, 1);
		ColorLine(counter->left - 1, counter->top + 1, 
				counter->right, counter->top + 1, 2);
		ColorLine(counter->left - 1, counter->top + 2, 
				counter->right, counter->top + 2, 3);
		ColorLine(counter->left - 1, counter->top + 3, 
				counter->right, counter->top + 3, 4);
		ColorLine(counter->left - 1, counter->top + 4, 
				counter->right, counter->top + 4, 5);
		ColorLine(counter->left - 1, counter->top + 5, 
				counter->right, counter->top + 5, 5);
		ColorLine(counter->left - 1, counter->top + 6, 
				counter->right, counter->top + 6, 5);
		ColorLine(counter->left - 1, counter->top, 
				counter->left - 1, counter->top + 6, 1);
	}
	else
	{
		ColorLine(counter->left - 1, counter->top, 
				counter->right, counter->top, k8LtstGrayColor);
		ColorLine(counter->left - 1, counter->top + 1, 
				counter->right, counter->top + 1, k8LtstGray2Color);
		ColorLine(counter->left - 1, counter->top + 2, 
				counter->right, counter->top + 2, k8LtstGray3Color);
		ColorLine(counter->left - 1, counter->top + 3, 
				counter->right, counter->top + 3, k8LtstGray4Color);
		ColorLine(counter->left - 1, counter->top + 4, 
				counter->right, counter->top + 4, k8LtstGray5Color);
		ColorLine(counter->left - 1, counter->top + 5, 
				counter->right, counter->top + 5, k8LtstGray5Color);
		ColorLine(counter->left - 1, counter->top + 6, 
				counter->right, counter->top + 6, k8LtstGray5Color);
		ColorLine(counter->left - 1, counter->top, 
				counter->left - 1, counter->top + 6, k8LtstGrayColor);
	}
	
	ColorLine(counter->right, counter->top, 
			counter->right, counter->top + 6, k8LtGrayColor);
	ColorLine(counter->left + 1, counter->top + 7, 
			counter->right - 2, counter->top + 7, dkstRedC);
	ColorLine(counter->left + 1, counter->top + 8, 
			counter->right - 2, counter->top + 8, dkstRedC);
	
	SetGWorld(wasCPort, wasWorld);
	
	nRects = RectWide(counter) / 40;
	if (nRects == 0)
		nRects = 1;
	width = ((RectWide(counter) - kCounterStripWide) / nRects) - kCounterStripWide;
	QSetRect(&tempRect, 0, 0, width, RectTall(counter) - kCounterStripTall);
	QOffsetRect(&tempRect, counter->left + kCounterStripWide, 
			counter->top + kCounterPanelDrop);
	for (i = 0; i < nRects; i++)
	{
		HiliteRect(&tempRect, tanC, dkstRedC);
		InsetRect(&tempRect, 4, 4);
		HiliteRect(&tempRect, dkstRedC, tanC);
		InsetRect(&tempRect, -4, -4);
		QOffsetRect(&tempRect, kCounterStripWide + width, 0);
	}
}

//--------------------------------------------------------------  DrawDresser

void DrawDresser (Rect *dresser)
{
	#define		kDresserTopThick	4
	#define		kDresserCrease		9
	#define		kDresserDrawerDrop	12
	#define		kDresserSideSpare	14
	Rect		tempRect, dest;
	long		yellowC, brownC, dkGrayC, ltTanC, dkstRedC;
	RgnHandle	shadowRgn;
	short		nRects, height, i;
	CGrafPtr	wasCPort;
	GDHandle	wasWorld;
	Pattern		dummyPattern;
	
	if (thisMac.isDepth == 4)
	{
		yellowC = 9;
		brownC = 11;
		dkGrayC = 14;
		ltTanC = 7;
		dkstRedC = 15;
	}
	else
	{
		yellowC = k8PissYellowColor;
		brownC = k8BrownColor;
		dkGrayC = k8DkstGrayColor;
		ltTanC = k8LtTanColor;
		dkstRedC = k8DkRed2Color;
	}
	
	GetGWorld(&wasCPort, &wasWorld);
	SetGWorld(backSrcMap, nil);
	
	MoveTo(dresser->left + 10, dresser->bottom + 9);
	shadowRgn = NewRgn();
	if (shadowRgn == nil)
		RedAlert(kErrUnnaccounted);
	OpenRgn();
	Line(RectWide(dresser) - 11, 0);
	Line(9, -9);
	Line(0, -RectTall(dresser) + 12);
	Line(-9, -9);
	Line(-RectWide(dresser) + 11, 0);
	LineTo(dresser->left + 10, dresser->bottom + 9);
	CloseRgn(shadowRgn);
	PenPat(GetQDGlobalsGray(&dummyPattern));
	PenMode(patOr);
	if (thisMac.isDepth == 4)
		ColorRegion(shadowRgn, 15);
	else
		ColorRegion(shadowRgn, k8DkstGrayColor);
	PenNormal();
	DisposeRgn(shadowRgn);
	
	InsetRect(dresser, 2, 2);
	ColorRect(dresser, k8PumpkinColor);
	HiliteRect(dresser, k8OrangeColor, dkstRedC);
	InsetRect(dresser, -2, -2);
	
	tempRect = *dresser;
	tempRect.bottom = tempRect.top + kDresserTopThick;
	ColorRect(&tempRect, k8PissYellowColor);
	HiliteRect(&tempRect, ltTanC, dkstRedC);
	ColorLine(dresser->left + 2, dresser->top + kDresserTopThick, 
			dresser->right - 3, dresser->top + kDresserTopThick, k8Red4Color);
	
	ColorLine(dresser->left + kDresserCrease, dresser->top + kDresserTopThick + 1, 
			dresser->left + kDresserCrease, dresser->bottom - 4, k8Red4Color);
	ColorLine(dresser->right - kDresserCrease, dresser->top + kDresserTopThick + 1, 
			dresser->right - kDresserCrease, dresser->bottom - 4, k8OrangeColor);
	
	nRects = RectTall(dresser) / 30;
	if (nRects == 0)
		nRects = 1;
	height = (RectTall(dresser) - 14) / nRects - 4;
	QSetRect(&tempRect, 0, 0, RectWide(dresser) - kDresserSideSpare, height);
	QOffsetRect(&tempRect, dresser->left + 7, dresser->top + 10);
	for (i = 0; i < nRects; i++)
	{
		ColorLine(tempRect.left + 1, tempRect.bottom, 
				tempRect.right, tempRect.bottom, dkstRedC);
		ColorLine(tempRect.right, tempRect.top + 1, 
				tempRect.right, tempRect.bottom, dkstRedC);
		ColorRect(&tempRect, yellowC);
		HiliteRect(&tempRect, ltTanC, brownC);
		InsetRect(&tempRect, 1, 1);
		HiliteRect(&tempRect, ltTanC, brownC);
		InsetRect(&tempRect, -1, -1);
		
		QSetRect(&dest, -4, -4, 4, 4);
		QOffsetRect(&dest, HalfRectTall(&tempRect), HalfRectTall(&tempRect));
		QOffsetRect(&dest, tempRect.left, tempRect.top);
		CopyBits((BitMap *)*GetGWorldPixMap(furnitureSrcMap), 
				(BitMap *)*GetGWorldPixMap(backSrcMap), 
				&knobSrc, &dest, srcCopy, nil);
		
		QSetRect(&dest, -4, -4, 4, 4);
		QOffsetRect(&dest, -HalfRectTall(&tempRect), HalfRectTall(&tempRect));
		QOffsetRect(&dest, tempRect.right, tempRect.top);
		CopyBits((BitMap *)*GetGWorldPixMap(furnitureSrcMap), 
				(BitMap *)*GetGWorldPixMap(backSrcMap), 
				&knobSrc, &dest, srcCopy, nil);
		
		QOffsetRect(&tempRect, 0, kDresserTopThick + height);
	}
	
	SetGWorld(wasCPort, wasWorld);
	
	dest = leftFootSrc;
	ZeroRectCorner(&dest);
	QOffsetRect(&dest, dresser->left + 6, dresser->bottom - 2);
	
	CopyMask((BitMap *)*GetGWorldPixMap(furnitureSrcMap), 
			(BitMap *)*GetGWorldPixMap(furnitureMaskMap), 
			(BitMap *)*GetGWorldPixMap(backSrcMap), 
			&leftFootSrc, &leftFootSrc, &dest);
	
	dest = rightFootSrc;
	ZeroRectCorner(&dest);
	QOffsetRect(&dest, dresser->right - 19, dresser->bottom - 2);
	
	CopyMask((BitMap *)*GetGWorldPixMap(furnitureSrcMap), 
			(BitMap *)*GetGWorldPixMap(furnitureMaskMap), 
			(BitMap *)*GetGWorldPixMap(backSrcMap), 
			&rightFootSrc, &rightFootSrc, &dest);
}

//--------------------------------------------------------------  DrawDeckTable

void DrawDeckTable (Rect *tableTop, short down)
{
	#define		kTableBaseTop		296
	#define		kTableShadowTop		312
	#define		kTableShadowOffset	12
	
	Rect		tempRect;
	long		bambooC, brownC, dkGrayC;
	short		hCenter, vShadow;
	CGrafPtr	wasCPort;
	GDHandle	wasWorld;
	Pattern		dummyPattern;
	
	if (thisMac.isDepth == 4)
	{
		bambooC = 6;
		brownC = 11;
		dkGrayC = 14;
	}
	else
	{
		bambooC = k8BambooColor;
		brownC = k8BrownColor;
		dkGrayC = k8DkstGrayColor;
	}
	
	GetGWorld(&wasCPort, &wasWorld);
	SetGWorld(backSrcMap, nil);
	
	QSetRect(&tempRect, tableTop->left, 0, tableTop->right, 
			RectWide(tableTop) / 10);
	QOffsetRect(&tempRect, 0, 
			-HalfRectTall(&tempRect) + kTableShadowTop + down);
	QOffsetRect(&tempRect, kTableShadowOffset, -kTableShadowOffset);
	PenPat(GetQDGlobalsGray(&dummyPattern));
	PenMode(patOr);
	ColorOval(&tempRect, dkGrayC);
	PenNormal();
	
	InsetRect(tableTop, 0, 1);
	ColorRect(tableTop, kGoldColor);
	InsetRect(tableTop, 0, -1);
	
	ColorLine(tableTop->left, tableTop->top + 1, 
			tableTop->left, tableTop->top + 1, k8WhiteColor);
	ColorLine(tableTop->left + 1, tableTop->top, 
			tableTop->right - 2, tableTop->top, k8WhiteColor);
	ColorLine(tableTop->right - 1, tableTop->top + 1, 
			tableTop->right - 1, tableTop->top + 1, k8WhiteColor);
	
	ColorLine(tableTop->left + 1, tableTop->top + 1, 
			tableTop->right - 2, tableTop->top + 1, kYellowColor);
	ColorLine(tableTop->left, tableTop->top + 2, 
			tableTop->left, tableTop->bottom - 2, kYellowColor);
	
	ColorLine(tableTop->left + 1, tableTop->bottom - 1, 
			tableTop->right - 2, tableTop->bottom - 1, brownC);
	ColorLine(tableTop->right - 1, tableTop->top + 2, 
			tableTop->right - 1, tableTop->bottom - 2, brownC);
	
	ColorLine(tableTop->left + 1, tableTop->bottom - 2, 
			tableTop->right - 2, tableTop->bottom - 2, bambooC);
	
	if (tableTop->bottom < kTableBaseTop + down)
	{
		hCenter = (tableTop->left + tableTop->right) / 2;
		
		ColorLine(hCenter - 3, tableTop->bottom, 
				hCenter - 3, kTableBaseTop + down, dkGrayC);
		ColorLine(hCenter - 2, tableTop->bottom, 
				hCenter - 2, kTableBaseTop + down, k8WhiteColor);
		ColorLine(hCenter - 1, tableTop->bottom, 
				hCenter - 1, kTableBaseTop + down, k8WhiteColor);
		ColorLine(hCenter, tableTop->bottom, 
				hCenter, kTableBaseTop + down, k8LtGrayColor);
		ColorLine(hCenter + 1, tableTop->bottom, 
				hCenter + 1, kTableBaseTop + down, dkGrayC);
		
		vShadow = tableTop->bottom + RectWide(tableTop) / 4 - 2;
		if (vShadow > kTableBaseTop + down)
		{
			ColorLine(hCenter - 2, tableTop->bottom, 
					hCenter - 2, kTableBaseTop + down, k8LtGrayColor);
			ColorLine(hCenter - 1, tableTop->bottom, 
					hCenter - 1, kTableBaseTop + down, k8LtGrayColor);
			ColorLine(hCenter, tableTop->bottom, 
					hCenter, kTableBaseTop + down, dkGrayC);
		}
		else
		{
			ColorLine(hCenter - 2, tableTop->bottom, 
					hCenter - 2, vShadow, k8LtGrayColor);
			ColorLine(hCenter - 1, tableTop->bottom, 
					hCenter - 1, vShadow + 1, k8LtGrayColor);
			ColorLine(hCenter, tableTop->bottom, 
					hCenter, vShadow + 2, dkGrayC);
		}
	}
	
	SetGWorld(wasCPort, wasWorld);
	
	tempRect = deckSrc;
	ZeroRectCorner(&tempRect);
	QOffsetRect(&tempRect, -HalfRectWide(&deckSrc) + tableTop->left + 
			HalfRectWide(tableTop), kTableBaseTop + down);
	CopyMask((BitMap *)*GetGWorldPixMap(furnitureSrcMap), 
			(BitMap *)*GetGWorldPixMap(furnitureMaskMap), 
			(BitMap *)*GetGWorldPixMap(backSrcMap), 
			&deckSrc, &deckSrc, &tempRect);
}

//--------------------------------------------------------------  DrawStool

void DrawStool (Rect *theRect, short down)
{
	#define		kStoolBase	304
	long		grayC, dkGrayC;
	CGrafPtr	wasCPort;
	GDHandle	wasWorld;
	
	GetGWorld(&wasCPort, &wasWorld);
	SetGWorld(backSrcMap, nil);
	
	if (thisMac.isDepth == 4)
	{
		grayC = 13;
		dkGrayC = 14;
	}
	else
	{
		grayC = k8DkGray2Color;
		dkGrayC = k8DkstGrayColor;
	}
	
	if (theRect->bottom < kStoolBase + down)
	{
		ColorLine(theRect->left + 21, theRect->bottom - 1, 
				theRect->left + 21, kStoolBase + down - 1, k8DkGrayColor);
		ColorLine(theRect->left + 22, theRect->bottom - 1, 
				theRect->left + 22, kStoolBase + down, k8Gray2Color);
		ColorLine(theRect->left + 23, theRect->bottom - 1, 
				theRect->left + 23, kStoolBase + down, k8DkGrayColor);
		ColorLine(theRect->left + 24, theRect->bottom - 1, 
				theRect->left + 24, kStoolBase + down, k8DkGray3Color);
		ColorLine(theRect->left + 25, theRect->bottom - 1, 
				theRect->left + 25, kStoolBase + down, grayC);
		ColorLine(theRect->left + 26, theRect->bottom - 1, 
				theRect->left + 26, kStoolBase + down - 1, dkGrayC);
	}
	
	SetGWorld(wasCPort, wasWorld);
	
	CopyMask((BitMap *)*GetGWorldPixMap(furnitureSrcMap), 
			(BitMap *)*GetGWorldPixMap(furnitureMaskMap), 
			(BitMap *)*GetGWorldPixMap(backSrcMap), 
			&srcRects[kStool], &srcRects[kStool], theRect);
}

//--------------------------------------------------------------  DrawInvisObstacle

void DrawInvisObstacle (Rect *theRect)
{	
	CGrafPtr	wasCPort;
	GDHandle	wasWorld;
	
	GetGWorld(&wasCPort, &wasWorld);
	SetGWorld(backSrcMap, nil);
	ColorFrameRect(theRect, k8BrownColor);
	SetGWorld(wasCPort, wasWorld);
}

//--------------------------------------------------------------  DrawInvisBounce

void DrawInvisBounce (Rect *theRect)
{
	CGrafPtr	wasCPort;
	GDHandle	wasWorld;
	
	GetGWorld(&wasCPort, &wasWorld);
	SetGWorld(backSrcMap, nil);
	ColorFrameRect(theRect, k8RedColor);
	SetGWorld(wasCPort, wasWorld);
}

//--------------------------------------------------------------  DrawRedClock

void DrawRedClock (Rect *theRect)
{
	DateTimeRec	timeRec;
	Rect		dest;
	short		hour, minutes;
	
	CopyMask((BitMap *)*GetGWorldPixMap(bonusSrcMap), 
			(BitMap *)*GetGWorldPixMap(bonusMaskMap), 
			(BitMap *)*GetGWorldPixMap(backSrcMap), 
			&srcRects[kRedClock], &srcRects[kRedClock], theRect);
	
	GetTime(&timeRec);
	hour = timeRec.hour % 12;
	if (hour == 0)
		hour = 12;
	minutes = timeRec.minute;
	
	QSetRect(&dest, 0, 0, 4, 6);
	QOffsetRect(&dest, theRect->left + 5, theRect->top + 7);
	if (hour > 9)
		DrawClockDigit(hour / 10, &dest);
	QOffsetRect(&dest, 4, 0);
	DrawClockDigit(hour % 10, &dest);
	QOffsetRect(&dest, 6, 0);
	DrawClockDigit(minutes / 10, &dest);
	QOffsetRect(&dest, 4, 0);
	DrawClockDigit(minutes % 10, &dest);
}

//--------------------------------------------------------------  DrawClockDigit

void DrawClockDigit (short number, Rect *dest)
{
	CopyBits((BitMap *)*GetGWorldPixMap(bonusSrcMap), 
			(BitMap *)*GetGWorldPixMap(backSrcMap), 
			&digits[number], dest, srcCopy, nil);
}

//--------------------------------------------------------------  DrawBlueClock

void DrawBlueClock (Rect *theRect)
{
	DateTimeRec	timeRec;
	Point		dest;
	short		hour, minutes;
	
	CopyMask((BitMap *)*GetGWorldPixMap(bonusSrcMap), 
			(BitMap *)*GetGWorldPixMap(bonusMaskMap), 
			(BitMap *)*GetGWorldPixMap(backSrcMap), 
			&srcRects[kBlueClock], &srcRects[kBlueClock], theRect);
	
	dest.h = theRect->left + 13;
	dest.v = theRect->top + 13;
	GetTime(&timeRec);
	hour = timeRec.hour % 12;
	minutes = ((timeRec.minute + 2) / 5) % 12;
	DrawClockHands(dest, minutes, hour);
}

//--------------------------------------------------------------  DrawYellowClock

void DrawYellowClock (Rect *theRect)
{
	DateTimeRec	timeRec;
	Point		dest;
	short		hour, minutes;
	
	CopyMask((BitMap *)*GetGWorldPixMap(bonusSrcMap), 
			(BitMap *)*GetGWorldPixMap(bonusMaskMap), 
			(BitMap *)*GetGWorldPixMap(backSrcMap), 
			&srcRects[kYellowClock], &srcRects[kYellowClock], theRect);
	
	dest.h = theRect->left + 13;
	dest.v = theRect->top + 15;
	GetTime(&timeRec);
	hour = timeRec.hour % 12;
	minutes = ((timeRec.minute + 2) / 5) % 12;
	DrawClockHands(dest, minutes, hour);
}

//--------------------------------------------------------------  DrawCuckoo

void DrawCuckoo (Rect *theRect)
{
	DateTimeRec	timeRec;
	Point		dest;
	short		hour, minutes;
	
	CopyMask((BitMap *)*GetGWorldPixMap(bonusSrcMap), 
			(BitMap *)*GetGWorldPixMap(bonusMaskMap), 
			(BitMap *)*GetGWorldPixMap(backSrcMap), 
			&srcRects[kCuckoo], &srcRects[kCuckoo], theRect);
	
	dest.h = theRect->left + 19;
	dest.v = theRect->top + 31;
	GetTime(&timeRec);
	hour = timeRec.hour % 12;
	minutes = ((timeRec.minute + 2) / 5) % 12;
	DrawLargeClockHands(dest, minutes, hour);
}

//--------------------------------------------------------------  DrawClockHands

void DrawClockHands (Point where, short bigHand, short littleHand)
{
	CGrafPtr	wasCPort;
	GDHandle	wasWorld;
	
	GetGWorld(&wasCPort, &wasWorld);
	SetGWorld(backSrcMap, nil);
	MoveTo(where.h, where.v);
	switch (bigHand)
	{
		case 0:
		Line(0, -6);
		break;
		
		case 1:
		Line(3, -5);
		break;
		
		case 2:
		Line(5, -3);
		break;
		
		case 3:
		Line(6, 0);
		break;
		
		case 4:
		Line(5, 3);
		break;
		
		case 5:
		Line(3, 5);
		break;
		
		case 6:
		Line(0, 6);
		break;
		
		case 7:
		Line(-3, 5);
		break;
		
		case 8:
		Line(-5, 3);
		break;
		
		case 9:
		Line(-6, 0);
		break;
		
		case 10:
		Line(-5, -3);
		break;
		
		case 11:
		Line(-3, -5);
		break;
	}
	
	MoveTo(where.h, where.v);
	switch (littleHand)
	{
		case 0:
		Line(0, -4);
		break;
		
		case 1:
		Line(2, -3);
		break;
		
		case 2:
		Line(3, -2);
		break;
		
		case 3:
		Line(4, 0);
		break;
		
		case 4:
		Line(3, 2);
		break;
		
		case 5:
		Line(2, 3);
		break;
		
		case 6:
		Line(0, 4);
		break;
		
		case 7:
		Line(-2, 3);
		break;
		
		case 8:
		Line(-3, 2);
		break;
		
		case 9:
		Line(-4, 0);
		break;
		
		case 10:
		Line(-3, -2);
		break;
		
		case 11:
		Line(-2, -3);
		break;
	}
	
	SetGWorld(wasCPort, wasWorld);
}

//--------------------------------------------------------------  DrawClockHands

void DrawLargeClockHands (Point where, short bigHand, short littleHand)
{
	CGrafPtr	wasCPort;
	GDHandle	wasWorld;
	
	GetGWorld(&wasCPort, &wasWorld);
	SetGWorld(backSrcMap, nil);
	ForeColor(whiteColor);
	
	MoveTo(where.h, where.v);
	switch (bigHand)
	{
		case 0:
		Line(0, -10);
		break;
		
		case 1:
		Line(5, -9);
		break;
		
		case 2:
		Line(9, -5);
		break;
		
		case 3:
		Line(10, 0);
		break;
		
		case 4:
		Line(9, 5);
		break;
		
		case 5:
		Line(5, 9);
		break;
		
		case 6:
		Line(0, 10);
		break;
		
		case 7:
		Line(-5, 9);
		break;
		
		case 8:
		Line(-9, 5);
		break;
		
		case 9:
		Line(-10, 0);
		break;
		
		case 10:
		Line(-9, -5);
		break;
		
		case 11:
		Line(-5, -9);
		break;
	}
	
	MoveTo(where.h, where.v);
	switch (littleHand)
	{
		case 0:
		Line(0, -6);
		break;
		
		case 1:
		Line(3, -5);
		break;
		
		case 2:
		Line(5, -3);
		break;
		
		case 3:
		Line(6, 0);
		break;
		
		case 4:
		Line(5, 3);
		break;
		
		case 5:
		Line(3, 5);
		break;
		
		case 6:
		Line(0, 6);
		break;
		
		case 7:
		Line(-3, 5);
		break;
		
		case 8:
		Line(-5, 3);
		break;
		
		case 9:
		Line(-6, 0);
		break;
		
		case 10:
		Line(-5, -3);
		break;
		
		case 11:
		Line(-3, -5);
		break;
	}
	
	ForeColor(blackColor);
	SetGWorld(wasCPort, wasWorld);
}

//--------------------------------------------------------------  DrawSimplePrizes

void DrawSimplePrizes (short what, Rect *theRect)
{
	CopyMask((BitMap *)*GetGWorldPixMap(bonusSrcMap), 
			(BitMap *)*GetGWorldPixMap(bonusMaskMap), 
			(BitMap *)*GetGWorldPixMap(backSrcMap), 
			&srcRects[what], &srcRects[what], theRect);
}

//--------------------------------------------------------------  DrawGreaseRt

void DrawGreaseRt (Rect *theRect, short distance, Boolean state)
{
	Rect		spill, dest;
	CGrafPtr	wasCPort;
	GDHandle	wasWorld;
	
	dest = *theRect;
	if (state)		// grease upright
	{
		CopyMask((BitMap *)*GetGWorldPixMap(bonusSrcMap), 
				(BitMap *)*GetGWorldPixMap(bonusMaskMap), 
				(BitMap *)*GetGWorldPixMap(backSrcMap), 
				&greaseSrcRt[0], &greaseSrcRt[0], &dest);
	}
	else			// grease spilled
	{
		QOffsetRect(&dest, 6, 0);
		CopyMask((BitMap *)*GetGWorldPixMap(bonusSrcMap), 
				(BitMap *)*GetGWorldPixMap(bonusMaskMap), 
				(BitMap *)*GetGWorldPixMap(backSrcMap), 
				&greaseSrcRt[3], &greaseSrcRt[3], &dest);
		
		GetGWorld(&wasCPort, &wasWorld);
		SetGWorld(backSrcMap, nil);
		QSetRect(&spill, 0, -2, distance - 5, 0);
		QOffsetRect(&spill, dest.right - 1, dest.bottom);
		PaintRect(&spill);
		SetGWorld(wasCPort, wasWorld);
	}
}

//--------------------------------------------------------------  DrawGreaseLf

void DrawGreaseLf (Rect *theRect, short distance, Boolean state)
{
	Rect		spill, dest;
	CGrafPtr	wasCPort;
	GDHandle	wasWorld;
	
	dest = *theRect;
	if (state)		// grease upright
	{
		CopyMask((BitMap *)*GetGWorldPixMap(bonusSrcMap), 
				(BitMap *)*GetGWorldPixMap(bonusMaskMap), 
				(BitMap *)*GetGWorldPixMap(backSrcMap), 
				&greaseSrcLf[0], &greaseSrcLf[0], &dest);
	}
	else			// grease spilled
	{
		QOffsetRect(&dest, -6, 0);
		CopyMask((BitMap *)*GetGWorldPixMap(bonusSrcMap), 
				(BitMap *)*GetGWorldPixMap(bonusMaskMap), 
				(BitMap *)*GetGWorldPixMap(backSrcMap), 
				&greaseSrcLf[3], &greaseSrcLf[3], &dest);
		
		GetGWorld(&wasCPort, &wasWorld);
		SetGWorld(backSrcMap, nil);
		QSetRect(&spill, -distance + 5, -2, 0, 0);
		QOffsetRect(&spill, dest.left + 1, dest.bottom);
		PaintRect(&spill);
		SetGWorld(wasCPort, wasWorld);
	}
}

//--------------------------------------------------------------  DrawBands

void DrawFoil (Rect *theRect)
{
	CopyMask((BitMap *)*GetGWorldPixMap(bonusSrcMap), 
			(BitMap *)*GetGWorldPixMap(bonusMaskMap), 
			(BitMap *)*GetGWorldPixMap(backSrcMap), 
			&srcRects[kFoil], &srcRects[kFoil], theRect);
}

//--------------------------------------------------------------  DrawInvisBonus

void DrawInvisBonus (Rect *theRect)
{
	CGrafPtr	wasCPort;
	GDHandle	wasWorld;
	
	GetGWorld(&wasCPort, &wasWorld);
	SetGWorld(backSrcMap, nil);
	ColorFrameOval(theRect, 227);
	SetGWorld(wasCPort, wasWorld);
}

//--------------------------------------------------------------  DrawSlider

void DrawSlider (Rect *theRect)
{
	CGrafPtr	wasCPort;
	GDHandle	wasWorld;
	
	GetGWorld(&wasCPort, &wasWorld);
	SetGWorld(backSrcMap, nil);
	FrameRect(theRect);
	SetGWorld(wasCPort, wasWorld);
}

