//============================================================================
//----------------------------------------------------------------------------
//								Environ.h
//----------------------------------------------------------------------------
//============================================================================


#include <Quickdraw.h>


typedef struct
{
	Rect		screen, gray;
	long		dirID;
	short		wasDepth, isDepth;
	short		thisResFile;
	short		numScreens;
	short		vRefNum;
	Boolean		can1Bit;
	Boolean		can4Bit;
	Boolean		can8Bit;
	Boolean		wasColorOrGray;
	Boolean		hasWNE;
	Boolean		hasSystem7;
	Boolean		hasColor;
	Boolean		hasGestalt;
	Boolean		canSwitch;
	Boolean		canColor;
	Boolean		hasSM3;
	Boolean		hasQT;
	Boolean		hasDrag;
} macEnviron;


extern macEnviron	thisMac;


