//============================================================================
//----------------------------------------------------------------------------
//								GliderVars.h
//----------------------------------------------------------------------------
//============================================================================


#include <Movies.h>


extern	Rect		blowerSrcRect;
extern	Rect		flame[], tikiFlame[];
extern	Rect		coals[];
extern	Rect		furnitureSrcRect;
extern	Rect		tableSrc, shelfSrc, hingeSrc, handleSrc, knobSrc;
extern	Rect		leftFootSrc, rightFootSrc, deckSrc;
extern	Rect		bonusSrcRect;
extern	Rect		pointsSrcRect;
extern	Rect		starSrc[], sparkleSrc[];
extern	Rect		digits[], pendulumSrc[], greaseSrcRt[], greaseSrcLf[];
extern	Rect		transSrcRect;
extern	Rect		switchSrcRect;
extern	Rect		lightSwitchSrc[], machineSwitchSrc[], thermostatSrc[];
extern	Rect		powerSrc[], knifeSwitchSrc[];
extern	Rect		lightSrcRect;
extern	Rect		flourescentSrc1, flourescentSrc2;
extern	Rect		trackLightSrc[];
extern	Rect		applianceSrcRect, toastSrcRect, shredSrcRect;	// Appliances
extern	Rect		plusScreen1, plusScreen2, tvScreen1, tvScreen2;
extern	Rect		coffeeLight1, coffeeLight2, vcrTime1, vcrTime2;
extern	Rect		stereoLight1, stereoLight2, microOn, microOff;
extern	Rect		outletSrc[];
extern	Rect		balloonSrcRect, copterSrcRect, dartSrcRect;		// Enemies
extern	Rect		ballSrcRect, dripSrcRect, enemySrcRect;
extern	Rect		fishSrcRect;
extern	Rect		balloonSrc[], copterSrc[], dartSrc[];
extern	Rect		ballSrc[], dripSrc[], fishSrc[];
extern	Rect		clutterSrcRect;
extern	Rect		flowerSrc[];
extern	Rect		*srcRects;

extern	Movie		theMovie;
extern	Rect		movieRect;
extern	Boolean		hasMovie, tvInRoom;

extern	gliderType	theGlider, theGlider2;
extern	objDataPtr	masterObjects;
extern	Rect		workSrcRect;
extern	Rect		backSrcRect;
extern	Rect		mainWindowRect, houseRect;
extern	houseHand	thisHouse;
extern	roomPtr		thisRoom;
extern	WindowPtr	mainWindow, coordWindow;
extern	long		theScore;
extern	short		playOriginH, playOriginV;
extern	short		thisRoomNumber, theMode, batteryTotal, bandsTotal;
extern	short		foilTotal, mortals, numMasterObjects, previousRoom;
extern	Boolean		fileDirty, gameDirty, showFoil, doZooms, isPlayMusicGame;

