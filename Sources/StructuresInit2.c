//============================================================================
//----------------------------------------------------------------------------
//							   StructuresInit2.c
//----------------------------------------------------------------------------
//============================================================================


#include <Resources.h>
#include "Externs.h"
#include "Environ.h"
#include "GameOver.h"
#include "MainWindow.h"
#include "Objects.h"
#include "RectUtils.h"
#include "Room.h"
#include "RoomGraphics.h"
#include "Utilities.h"


#define kAngelPictID			1019
#define kSupportPictID			1999
#define kClutterPictID			4018


void InitClutter (void);
void InitSupport (void);
void InitAngel (void);


extern	Rect		suppSrcRect, justRoomsRect;
extern	Rect		tileSrcRect, angelSrcRect;
extern	GDHandle	thisGDevice;
extern	CGrafPtr	tileSrcMap;
extern	FSSpecPtr	theHousesSpecs;
extern	hotPtr		hotSpots;
extern	sparklePtr	sparkles;
extern	flyingPtPtr	flyingPoints;
extern	flamePtr	flames, tikiFlames, bbqCoals;
extern	pendulumPtr	pendulums;
extern	savedPtr	savedMaps;
extern	bandPtr		bands;
extern	greasePtr	grease;
extern	starPtr		theStars;
extern	shredPtr	shreds;
extern	dynaPtr		dinahs;
extern	demoPtr		demoData;
extern	short		maxFiles;


//==============================================================  Functions
//--------------------------------------------------------------  InitClutter
// Structures and graphics relating to clutter are loaded up.
// Clutter includes mirrors, teddy bears, fireplaces, calendars, etc.

void InitClutter (void)
{
	CGrafPtr	wasCPort;
	GDHandle	wasWorld;
	OSErr		theErr;
	
	GetGWorld(&wasCPort, &wasWorld);
	
	QSetRect(&clutterSrcRect, 0, 0, 128, 69);
	theErr = CreateOffScreenGWorld(&clutterSrcMap, &clutterSrcRect, kPreferredDepth);
	SetGWorld(clutterSrcMap, nil);
	LoadGraphic(kClutterPictID);
	
	theErr = CreateOffScreenGWorld(&clutterMaskMap, &clutterSrcRect, 1);	
	SetGWorld(clutterMaskMap, nil);
	LoadGraphic(kClutterPictID + 1000);
	
	QSetRect(&flowerSrc[0], 0, 0, 10, 28);
	QOffsetRect(&flowerSrc[0], 0, 23);
	
	QSetRect(&flowerSrc[1], 0, 0, 24, 35);
	QOffsetRect(&flowerSrc[1], 10, 16);
	
	QSetRect(&flowerSrc[2], 0, 0, 34, 35);
	QOffsetRect(&flowerSrc[2], 34, 16);
	
	QSetRect(&flowerSrc[3], 0, 0, 27, 23);
	QOffsetRect(&flowerSrc[3], 68, 14);
	
	QSetRect(&flowerSrc[4], 0, 0, 27, 14);
	QOffsetRect(&flowerSrc[4], 68, 37);
	
	QSetRect(&flowerSrc[5], 0, 0, 32, 51);
	QOffsetRect(&flowerSrc[5], 95, 0);
	
	SetGWorld(wasCPort, wasWorld);
}

//--------------------------------------------------------------  InitSupport
// The floor support grphic is loaded up.  It is only visible when…
// playing in 9-room mode.  It is the horizontal wooden beam that…
// seperates floors from one another.

void InitSupport (void)
{
	CGrafPtr	wasCPort;
	GDHandle	wasWorld;
	OSErr		theErr;
	
	GetGWorld(&wasCPort, &wasWorld);
	
	QSetRect(&suppSrcRect, 0, 0, kRoomWide, kFloorSupportTall);		// 44
	theErr = CreateOffScreenGWorld(&suppSrcMap, &suppSrcRect, kPreferredDepth);
	SetGWorld(suppSrcMap, nil);
	LoadGraphic(kSupportPictID);
	
	SetGWorld(wasCPort, wasWorld);
}

//--------------------------------------------------------------  InitAngel
// This loads the graphic of the girl riding the glider.  It is seen…
// only upon completing a house.  She flies across the screen dropping…
// stars below.

void InitAngel (void)
{
	CGrafPtr	wasCPort;
	GDHandle	wasWorld;
	OSErr		theErr;
	
	GetGWorld(&wasCPort, &wasWorld);
	
	QSetRect(&angelSrcRect, 0, 0, 96, 44);
	theErr = CreateOffScreenGWorld(&angelSrcMap, &angelSrcRect, kPreferredDepth);
	SetGWorld(angelSrcMap, nil);
	LoadGraphic(kAngelPictID);
	
	theErr = CreateOffScreenGWorld(&angelMaskMap, &angelSrcRect, 1);	
	SetGWorld(angelMaskMap, nil);
	LoadGraphic(kAngelPictID + 1);
	
	SetGWorld(wasCPort, wasWorld);
}

//--------------------------------------------------------------  CreateOffscreens
// All "utility" or "work" offscreen pix/bit maps are created here.
// These would be offscreens that are reused throughout a game - they…
// are not static (mere repositories for graphics).  Most are used…
// to facilitate the animation when a game is in progress.

void CreateOffscreens (void)
{
	CGrafPtr	wasCPort;
	GDHandle	wasWorld;
	OSErr		theErr;
	
	GetGWorld(&wasCPort, &wasWorld);
	
	justRoomsRect = houseRect;
	ZeroRectCorner(&justRoomsRect);
	
	workSrcRect = houseRect;			// Set up work map
	ZeroRectCorner(&workSrcRect);
	theErr = CreateOffScreenGWorld(&workSrcMap, &workSrcRect, kPreferredDepth);
	
	backSrcRect = houseRect;			// Set up background map
	ZeroRectCorner(&backSrcRect);
	theErr = CreateOffScreenGWorld(&backSrcMap, &backSrcRect, kPreferredDepth);
	
	InitScoreboardMap();	SpinCursor(1);
	InitGliderMap();		SpinCursor(1);
	InitBlowers();			SpinCursor(1);
	InitFurniture();		SpinCursor(1);
	InitPrizes();			SpinCursor(1);
	InitTransports();		SpinCursor(1);
	InitSwitches();			SpinCursor(1);
	InitLights();			SpinCursor(1);
	InitAppliances();		SpinCursor(1);
	InitEnemies();			SpinCursor(1);
	InitClutter();			SpinCursor(1);
	InitSupport();			SpinCursor(1);
	InitAngel();			SpinCursor(1);
	
	QSetRect(&tileSrcRect, 0, 0, 128, 80);
	tileSrcMap = nil;
//	????
}

//--------------------------------------------------------------  CreatePointers
// This function allocates other large structures.  Pointers to hold…
// large arrays, etc.

void CreatePointers (void)
{
	Handle		tempHandle;
	short		i;
	
	thisRoom = nil;
	thisRoom = (roomPtr)NewPtr(sizeof(roomType));
	if (thisRoom == nil)
		RedAlert(kErrNoMemory);
	
	hotSpots = nil;
	hotSpots = (hotPtr)NewPtr(sizeof(hotObject) * kMaxHotSpots);
	if (hotSpots == nil)
		RedAlert(kErrNoMemory);
	
	sparkles = nil;
	sparkles = (sparklePtr)NewPtr(sizeof(sparkleType) * kMaxSparkles);
	if (sparkles == nil)
		RedAlert(kErrNoMemory);
	
	flyingPoints = nil;
	flyingPoints = (flyingPtPtr)NewPtr(sizeof(flyingPtType) * kMaxFlyingPts);
	if (flyingPoints == nil)
		RedAlert(kErrNoMemory);
	
	flames = nil;
	flames = (flamePtr)NewPtr(sizeof(flameType) * kMaxCandles);
	if (flames == nil)
		RedAlert(kErrNoMemory);
	
	tikiFlames = nil;
	tikiFlames = (flamePtr)NewPtr(sizeof(flameType) * kMaxTikis);
	if (tikiFlames == nil)
		RedAlert(kErrNoMemory);
	
	bbqCoals = nil;
	bbqCoals = (flamePtr)NewPtr(sizeof(flameType) * kMaxCoals);
	if (bbqCoals == nil)
		RedAlert(kErrNoMemory);
	
	pendulums = nil;
	pendulums = (pendulumPtr)NewPtr(sizeof(pendulumType) * kMaxPendulums);
	if (pendulums == nil)
		RedAlert(kErrNoMemory);
	
	savedMaps = nil;
	savedMaps = (savedPtr)NewPtr(sizeof(savedType) * kMaxSavedMaps);
	if (savedMaps == nil)
		RedAlert(kErrNoMemory);
	
	for (i = 0; i < kMaxSavedMaps; i++)
		savedMaps[i].map = nil;
	
	bands = nil;
	bands = (bandPtr)NewPtr(sizeof(bandType) * kMaxRubberBands);
	if (bands == nil)
		RedAlert(kErrNoMemory);
	
	grease = nil;
	grease = (greasePtr)NewPtr(sizeof(greaseType) * kMaxGrease);
	if (grease == nil)
		RedAlert(kErrNoMemory);
	
	theStars = nil;
	theStars = (starPtr)NewPtr(sizeof(starType) * kMaxStars);
	if (theStars == nil)
		RedAlert(kErrNoMemory);
	
	shreds = nil;
	shreds = (shredPtr)NewPtr(sizeof(shredType) * kMaxShredded);
	if (shreds == nil)
		RedAlert(kErrNoMemory);
	
	dinahs = nil;
	dinahs = (dynaPtr)NewPtr(sizeof(dynaType) * kMaxDynamicObs);
	if (dinahs == nil)
		RedAlert(kErrNoMemory);
	
	masterObjects = nil;
	masterObjects = (objDataPtr)NewPtr(sizeof(objDataType) * kMaxMasterObjects);
	if (masterObjects == nil)
		RedAlert(kErrNoMemory);
	
	srcRects = nil;
	srcRects = (Rect *)NewPtr(sizeof(Rect) * kNumSrcRects);
	if (srcRects == nil)
		RedAlert(kErrNoMemory);
	
	theHousesSpecs = nil;
	theHousesSpecs = (FSSpecPtr)NewPtr(sizeof(FSSpec) * maxFiles);
	if (theHousesSpecs == nil)
		RedAlert(kErrNoMemory);
	
#ifdef CREATEDEMODATA
	demoData = nil;
	demoData = (demoPtr)NewPtr(sizeof(demoType) * 2000);
	if (demoData == nil)
		RedAlert(kErrNoMemory);
#else
	demoData = nil;
	demoData = (demoPtr)NewPtr(kDemoLength);
	if (demoData == nil)
		RedAlert(kErrNoMemory);
	tempHandle = GetResource('demo', 128);
	if (tempHandle == nil)
		RedAlert(kErrNoMemory);
	else
	{
		BlockMove(*tempHandle, demoData, kDemoLength);
		ReleaseResource(tempHandle);
	}
#endif
}

//--------------------------------------------------------------  InitSrcRects

// This is a nasty, ugly function that initializes all global rectangles…
// used in Glider PRO.

void InitSrcRects (void)
{
	QSetRect(&srcRects[kFloorVent], 0, 0, 48, 11);		// Blowers
	QOffsetRect(&srcRects[kFloorVent], 0, 0);
	QSetRect(&srcRects[kCeilingVent], 0, 0, 48, 11);
	QOffsetRect(&srcRects[kCeilingVent], 0, 11);
	QSetRect(&srcRects[kFloorBlower], 0, 0, 48, 15);
	QOffsetRect(&srcRects[kFloorBlower], 0, 22);
	QSetRect(&srcRects[kCeilingBlower], 0, 0, 48, 15);
	QOffsetRect(&srcRects[kCeilingBlower], 0, 37);
	QSetRect(&srcRects[kSewerGrate], 0, 0, 48, 17);
	QOffsetRect(&srcRects[kSewerGrate], 0, 52);
	QSetRect(&srcRects[kLeftFan], 0, 0, 40, 55);
	QOffsetRect(&srcRects[kLeftFan], 0, 69);
	QSetRect(&srcRects[kRightFan], 0, 0, 40, 55);
	QOffsetRect(&srcRects[kRightFan], 0, 124);
	QSetRect(&srcRects[kTaper], 0, 0, 20, 59);
	QOffsetRect(&srcRects[kTaper], 0, 209);
	QSetRect(&srcRects[kCandle], 0, 0, 32, 30);
	QOffsetRect(&srcRects[kCandle], 0, 179);
	QSetRect(&srcRects[kStubby], 0, 0, 20, 36);
	QOffsetRect(&srcRects[kStubby], 0, 268);
	QSetRect(&srcRects[kTiki], 0, 0, 27, 28);
	QOffsetRect(&srcRects[kTiki], 21, 268);
	QSetRect(&srcRects[kBBQ], 0, 0, 64, 33);
	QSetRect(&srcRects[kInvisBlower], 0, 0, 24, 24);
	QSetRect(&srcRects[kGrecoVent], 0, 0, 48, 18);
	QOffsetRect(&srcRects[kGrecoVent], 0, 340);
	QSetRect(&srcRects[kSewerBlower], 0, 0, 32, 12);
	QOffsetRect(&srcRects[kSewerBlower], 0, 390);
	QSetRect(&srcRects[kLiftArea], 0, 0, 64, 32);
	
	QSetRect(&srcRects[kTable], 0, 0, 64, kTableThick);		// Furniture
	QSetRect(&srcRects[kShelf], 0, 0, 64, kShelfThick);
	QSetRect(&srcRects[kCabinet], 0, 0, 64, 64);
	QSetRect(&srcRects[kFilingCabinet], 0, 0, 74, 107);
	QSetRect(&srcRects[kWasteBasket], 0, 0, 64, 61);
	QOffsetRect(&srcRects[kWasteBasket], 0, 43);
	QSetRect(&srcRects[kMilkCrate], 0, 0, 64, 58);
	QOffsetRect(&srcRects[kMilkCrate], 0, 104);
	QSetRect(&srcRects[kCounter], 0, 0, 128, 64);
	QSetRect(&srcRects[kDresser], 0, 0, 128, 64);
	QSetRect(&srcRects[kDeckTable], 0, 0, 64, kTableThick);
	QSetRect(&srcRects[kStool], 0, 0, 48, 38);
	QOffsetRect(&srcRects[kStool], 0, 183);
	QSetRect(&srcRects[kTrunk], 0, 0, 144, 80);
	QSetRect(&srcRects[kInvisObstacle], 0, 0, 64, 64);
	QSetRect(&srcRects[kManhole], 0, 0, 123, 22);
	QSetRect(&srcRects[kBooks], 0, 0, 64, 51);
	QSetRect(&srcRects[kInvisBounce], 0, 0, 64, 64);
	
	QSetRect(&srcRects[kRedClock], 0, 0, 28, 17);			// Prizes
	QSetRect(&srcRects[kBlueClock], 0, 0, 28, 25);
	QOffsetRect(&srcRects[kBlueClock], 0, 17);
	QSetRect(&srcRects[kYellowClock], 0, 0, 28, 28);
	QOffsetRect(&srcRects[kYellowClock], 0, 42);
	QSetRect(&srcRects[kCuckoo], 0, 0, 40, 80);
	QOffsetRect(&srcRects[kCuckoo], 0, 148);
	QSetRect(&srcRects[kPaper], 0, 0, 48, 21);
	QOffsetRect(&srcRects[kPaper], 0, 127);
	QSetRect(&srcRects[kBattery], 0, 0, 16, 25);
	QOffsetRect(&srcRects[kBattery], 32, 0);
	QSetRect(&srcRects[kBands], 0, 0, 28, 23);
	QOffsetRect(&srcRects[kBands], 20, 70);
	QSetRect(&srcRects[kGreaseRt], 0, 0, 32, 27);
	QOffsetRect(&srcRects[kGreaseRt], 0, 243);
	QSetRect(&srcRects[kGreaseLf], 0, 0, 32, 27);
	QOffsetRect(&srcRects[kGreaseLf], 0, 324);
	QSetRect(&srcRects[kFoil], 0, 0, 55, 15);
	QOffsetRect(&srcRects[kFoil], 0, 228);
	QSetRect(&srcRects[kInvisBonus], 0, 0, 24, 24);
	QSetRect(&srcRects[kStar], 0, 0, 32, 31);
	QOffsetRect(&srcRects[kStar], 48, 0);
	QSetRect(&srcRects[kSparkle], 0, 0, 20, 19);
	QOffsetRect(&srcRects[kSparkle], 0, 70);
	QSetRect(&srcRects[kHelium], 0, 0, 56, 16);
	QOffsetRect(&srcRects[kHelium], 32, 270);
	QSetRect(&srcRects[kSlider], 0, 0, 64, 16);
	
	QSetRect(&srcRects[kUpStairs], 0, 0, 160, 267);			// Transport
	QSetRect(&srcRects[kDownStairs], 0, 0, 160, 267);
	QSetRect(&srcRects[kMailboxLf], 0, 0, 94, 80);
	QSetRect(&srcRects[kMailboxRt], 0, 0, 94, 80);
	QSetRect(&srcRects[kFloorTrans], 0, 0, 56, 15);
	QOffsetRect(&srcRects[kFloorTrans], 0, 1);
	QSetRect(&srcRects[kCeilingTrans], 0, 0, 56, 15);
	QOffsetRect(&srcRects[kCeilingTrans], 0, 16);
	QSetRect(&srcRects[kDoorInLf], 0, 0, 144, 322);
	QSetRect(&srcRects[kDoorInRt], 0, 0, 144, 322);
	QSetRect(&srcRects[kDoorExRt], 0, 0, 16, 322);
	QSetRect(&srcRects[kDoorExLf], 0, 0, 16, 322);
	QSetRect(&srcRects[kWindowInLf], 0, 0, 20, 170);
	QSetRect(&srcRects[kWindowInRt], 0, 0, 20, 170);
	QSetRect(&srcRects[kWindowExRt], 0, 0, 16, 170);
	QSetRect(&srcRects[kWindowExLf], 0, 0, 16, 170);
	QSetRect(&srcRects[kInvisTrans], 0, 0, 64, 32);
	QSetRect(&srcRects[kDeluxeTrans], 0, 0, 64, 64);
	
	QSetRect(&srcRects[kLightSwitch], 0, 0, 15, 24);		// Switch
	QSetRect(&srcRects[kMachineSwitch], 0, 0, 16, 24);
	QOffsetRect(&srcRects[kMachineSwitch], 0, 48);
	QSetRect(&srcRects[kThermostat], 0, 0, 15, 24);
	QOffsetRect(&srcRects[kThermostat], 0, 48);
	QSetRect(&srcRects[kPowerSwitch], 0, 0, 8, 8);
	QOffsetRect(&srcRects[kPowerSwitch], 0, 72);
	QSetRect(&srcRects[kKnifeSwitch], 0, 0, 16, 24);
	QOffsetRect(&srcRects[kKnifeSwitch], 0, 80);
	QSetRect(&srcRects[kInvisSwitch], 0, 0, 12, 12);
	QSetRect(&srcRects[kTrigger], 0, 0, 12, 12);
	QSetRect(&srcRects[kLgTrigger], 0, 0, 48, 48);
	QSetRect(&srcRects[kSoundTrigger], 0, 0, 32, 32);
	
	QSetRect(&srcRects[kCeilingLight], 0, 0, 64, 20);		// Lights
	QOffsetRect(&srcRects[kCeilingLight], 0, 0);
	QSetRect(&srcRects[kLightBulb], 0, 0, 16, 28);
	QOffsetRect(&srcRects[kLightBulb], 0, 20);
	QSetRect(&srcRects[kTableLamp], 0, 0, 48, 70);
	QOffsetRect(&srcRects[kTableLamp], 16, 20);
	QSetRect(&srcRects[kHipLamp], 0, 0, 72, 276);
	QSetRect(&srcRects[kDecoLamp], 0, 0, 64, 212);
	QSetRect(&srcRects[kFlourescent], 0, 0, 64, 12);
	QSetRect(&srcRects[kTrackLight], 0, 0, 64, 24);
	QSetRect(&srcRects[kInvisLight], 0, 0, 16, 16);
	
	QSetRect(&srcRects[kShredder], 0, 0, 73, 22);			// Appliances
	QSetRect(&srcRects[kToaster], 0, 0, 48, 27);
	QOffsetRect(&srcRects[kToaster], 0, 22);
	QSetRect(&srcRects[kMacPlus], 0, 0, 48, 58);
	QOffsetRect(&srcRects[kMacPlus], 0, 49);
	QSetRect(&srcRects[kGuitar], 0, 0, 64, 172);
	QSetRect(&srcRects[kTV], 0, 0, 92, 77);
	QSetRect(&srcRects[kCoffee], 0, 0, 43, 64);
	QOffsetRect(&srcRects[kCoffee], 0, 107);
	QSetRect(&srcRects[kOutlet], 0, 0, 16, 24);
	QOffsetRect(&srcRects[kOutlet], 64, 22);
	QSetRect(&srcRects[kVCR], 0, 0, 96, 22);
	QSetRect(&srcRects[kStereo], 0, 0, 128, 53);
	QSetRect(&srcRects[kMicrowave], 0, 0, 92, 59);
	QSetRect(&srcRects[kCinderBlock], 0, 0, 40, 62);
	QSetRect(&srcRects[kFlowerBox], 0, 0, 80, 32);
	QSetRect(&srcRects[kCDs], 0, 0, 16, 30);
	QOffsetRect(&srcRects[kCDs], 48, 22);
	QSetRect(&srcRects[kCustomPict], 0, 0, 72, 34);
	
	QSetRect(&srcRects[kBalloon], 0, 0, 24, 30);			// Enemies
	QSetRect(&srcRects[kCopterLf], 0, 0, 32, 30);
	QSetRect(&srcRects[kCopterRt], 0, 0, 32, 30);
	QSetRect(&srcRects[kDartLf], 0, 0, 64, 19);
	QSetRect(&srcRects[kDartRt], 0, 0, 64, 19);
	QSetRect(&srcRects[kBall], 0, 0, 32, 32);
	QSetRect(&srcRects[kDrip], 0, 0, 16, 12);
	QSetRect(&srcRects[kFish], 0, 0, 36, 33);
	QSetRect(&srcRects[kCobweb], 0, 0, 54, 45);
	
	QSetRect(&srcRects[kOzma], 0, 0, 102, 92);				// Clutter
	QSetRect(&srcRects[kMirror], 0, 0, 64, 64);
	QSetRect(&srcRects[kMousehole], 0, 0, 10, 11);
	QSetRect(&srcRects[kFireplace], 0, 0, 180, 142);
	QSetRect(&srcRects[kWallWindow], 0, 0, 64, 80);
	QSetRect(&srcRects[kBear], 0, 0, 56, 58);
	QSetRect(&srcRects[kCalendar], 0, 0, 63, 92);
	QSetRect(&srcRects[kVase1], 0, 0, 36, 45);
	QSetRect(&srcRects[kVase2], 0, 0, 35, 57);
	QSetRect(&srcRects[kBulletin], 0, 0, 80, 58);
	QSetRect(&srcRects[kCloud], 0, 0, 128, 30);
	QSetRect(&srcRects[kFaucet], 0, 0, 56, 18);
	QOffsetRect(&srcRects[kFaucet], 0, 51);
	QSetRect(&srcRects[kRug], 0, 0, 144, 18);
	QSetRect(&srcRects[kChimes], 0, 0, 28, 74);
}

