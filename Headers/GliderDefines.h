
//============================================================================
//----------------------------------------------------------------------------
//								GliderDefines.h
//----------------------------------------------------------------------------
//============================================================================


//==============================================================  Defines

//#define CREATEDEMODATA
//#define COMPILEDEMO
//#define CAREFULDEBUG
#define COMPILENOCP
#define COMPILEQT
#define BUILD_ARCADE_VERSION		1

#define kYellowUnaccounted			1
#define kYellowFailedResOpen		2
#define kYellowFailedResAdd			3
#define kYellowFailedResCreate		4
#define kYellowNoHouses				5
#define kYellowNewerVersion			6
#define kYellowNoBackground			7
#define kYellowIllegalRoomNum		8
#define kYellowNoBoundsRes			9
#define kYellowScrapError			10
#define kYellowNoMemory				11
#define kYellowFailedWrite			12
#define kYellowNoMusic				13
#define kYellowFailedSound			14
#define kYellowAppleEventErr		15
#define kYellowOpenedOldHouse		16
#define kYellowLostAllHouses		17
#define kYellowFailedSaveGame		18
#define kYellowSavedTimeWrong		19
#define kYellowSavedVersWrong		20
#define kYellowSavedRoomsWrong		21
#define kYellowQTMovieNotLoaded		22
#define kYellowNoRooms				23
#define kYellowCantOrderLinks		24

#define kSwitchIfNeeded				0
#define kSwitchTo256Colors			1
#define kSwitchTo16Grays			2

#define kProdGameScoreMode			-4
#define kKickGameScoreMode			-3
#define kPlayGameScoreMode			-2
#define kPlayWholeScoreMode			-1
#define kPlayChorus					4
#define kPlayRefrainSparse1			5
#define kPlayRefrainSparse2			6

#define kHitWallSound				0			//   ееееее
#define kFadeInSound				1			//  ее
#define kFadeOutSound				2			//   ееееее
#define kBeepsSound					3			//        ее
#define kBuzzerSound				4			//   ееееее
#define kDingSound					5			//
#define kEnergizeSound				6			//   ееееее
#define kFollowSound				7			//  ее    ее
#define kMicrowavedSound			8			//  ее    ее
#define kSwitchSound				9			//  ее    ее
#define kBirdSound					10			//   ееееее
#define kCuckooSound				11			//
#define kTikSound					12			//  ее    ее
#define kTokSound					13			//  ее    ее
#define kBlowerOn					14			//  ее    ее
#define kBlowerOff					15			//  ее    ее
#define kCaughtFireSound			16			//   ееееее
#define kScoreTikSound				17			//
#define kThrustSound				18			//  еее   ее
#define kFizzleSound				19			//  ееее  ее
#define kFireBandSound				20			//  ее ее ее
#define kBandReboundSound			21			//  ее  ееее
#define kGreaseSpillSound			22			//  ее   еее
#define kChordSound					23			//
#define kVCRSound					24			//  еееееее
#define kFoilHitSound				25			//  ее    ее
#define kShredSound					26			//  ее    ее
#define kToastLaunchSound			27			//  ее    ее
#define kToastLandSound				28			//  еееееее
#define kMacOnSound					29			//
#define kMacBeepSound				30			//
#define kMacOffSound				31			//
#define kTVOnSound					32			//
#define kTVOffSound					33			//   ееееее
#define kCoffeeSound				34			//  ее
#define kMysticSound				35			//   ееееее
#define kZapSound					36			//        ее
#define kPopSound					37			//   ееееее
#define kEnemyInSound				38			//
#define kEnemyOutSound				39			//   ееееее
#define kPaperCrunchSound			40			//  ее    ее
#define kBounceSound				41			//  ее    ее
#define kDripSound					42			//  ее    ее
#define kDropSound					43			//   ееееее
#define kFishOutSound				44			//
#define kFishInSound				45			//  ее    ее
#define kDontExitSound				46			//  ее    ее
#define kSizzleSound				47			//  ее    ее
#define kPaper1Sound				48			//  ее    ее
#define kPaper2Sound				49			//   ееееее
#define kPaper3Sound				50			//
#define kPaper4Sound				51			//  еее   ее
#define kTypingSound				52			//  ееее  ее
#define kCarriageSound				53			//  ее ее ее
#define kChord2Sound				54			//  ее  ееее
#define kPhoneRingSound				55			//  ее   еее
#define kChime1Sound				56			//
#define kChime2Sound				57			//  еееееее
#define kWebTwangSound				58			//  ее    ее
#define kTransOutSound				59			//  ее    ее
#define kTransInSound				60			//  ее    ее
#define kBonusSound					61			//  еееееее
#define kHissSound					62			//
#define kTriggerSound				63

#define kHitWallPriority			100			//   ееееее
#define kScoreTikPriority			101			//  ее
#define kBandReboundPriority		102			//   ееееее
#define kDontExitPriority			103			//        ее
#define kTikPriority				200			//   ееееее
#define kTokPriority				201			//
#define kMysticPriority				202			//   ееееее
#define kChime1Priority				203			//  ее    ее
#define kChime2Priority				204			//  ее    ее
#define kThrustPriority				300			//  ее    ее
#define kFireBandPriority			301			//   ееееее
#define kChordPriority				302			//
#define kVCRPriority				303			//  ее    ее
#define kToastLaunchPriority		304			//  ее    ее
#define kToastLandPriority			305			//  ее    ее
#define kCoffeePriority				306			//  ее    ее
#define kBouncePriority				307			//   ееееее
#define kDripPriority				308			//
#define kDropPriority				309			//  еее   ее
#define kWebTwangPriority			310			//  ееее  ее
#define kHissPriority				311			//  ее ее ее
#define kFoilHitPriority			400			//  ее  ееее
#define kMacOnPriority				401			//  ее   еее
#define kMacOffPriority				402			//
#define kMacBeepPriority			403			//  еееееее
#define kTVOnPriority				404			//  ее    ее
#define kTVOffPriority				405			//  ее    ее
#define kZapPriority				406			//  ее    ее
#define kPopPriority				407			//  еееееее
#define kEnemyInPriority			408			//
#define kEnemyOutPriority			409			//
#define kPaperCrunchPriority		410			//
#define kFishOutPriority			411			//
#define kFishInPriority				412			//
#define kSizzlePriority				413
#define kPhoneRingPriority			500
#define kSwitchPriority				700
#define kBlowerOnPriority			701
#define kBlowerOffPriority			702
#define kFizzlePriority				703
#define kBeepsPriority				800
#define kBuzzerPriority				801
#define kDingPriority				802
#define kEnergizePriority			803
#define kBirdPriority				804
#define kCuckooPriority				805
#define kGreaseSpillPriority		806
#define kPapersPriority				807
#define kTypingPriority				808
#define kCarriagePriority			809
#define kChord2Priority				810
#define kMicrowavedPriority			811
#define kBonusPriority				812
#define kFadeInPriority				900
#define kFadeOutPriority			901
#define kCaughtFirePriority			902
#define kShredPriority				903
#define kFollowPriority				904
#define kTransInPriority			905
#define kTransOutPriority			906
#define kTriggerPriority			999

#define kArrowCursor				0
#define kBeamCursor					1
#define kHandCursor					2

#define kAppleMenuID				128
#define kGameMenuID					129
#define kOptionsMenuID				130
#define kHouseMenuID				131

#define kSplashMode					0
#define kEditMode					1
#define kPlayMode					2

#define kIdleSplashMode				0
#define kIdleDemoMode				1
#define kIdleSplashTicks			7200L		// 2 minutes
#define kIdleLastMode				1

#define kRoomAbove					1
#define kRoomBelow					2
#define kRoomToRight				3
#define kRoomToLeft					4

#define kBumpUp						1
#define kBumpDown					2
#define kBumpRight					3
#define kBumpLeft					4

#define kAbove						1
#define kToRight					2
#define kBelow						3
#define kToLeft						4
#define kBottomCorner				5
#define kTopCorner					6

#define kCentralRoom				0
#define kNorthRoom					1
#define kNorthEastRoom				2
#define kEastRoom					3
#define kSouthEastRoom				4
#define kSouthRoom					5
#define kSouthWestRoom				6
#define kWestRoom					7
#define kNorthWestRoom				8

#define kSimpleRoom					2000
#define kPaneledRoom				2001
#define kBasement					2002
#define kChildsRoom					2003
#define kAsianRoom					2004
#define kUnfinishedRoom				2005
#define kSwingersRoom				2006
#define kBathroom					2007
#define kLibrary					2008
#define kGarden						2009
#define kSkywalk					2010
#define kDirt						2011
#define kMeadow						2012
#define kField						2013
#define kRoof						2014
#define kSky						2015
#define kStratosphere				2016
#define kStars						2017

#define kMapRoomHeight				20
#define kMapRoomWidth				32

#define kMaxScores					10
#define kMaxRoomObs					24
#define kMaxSparkles				3
#define kNumSparkleModes			5
#define kMaxFlyingPts				3
#define kMaxFlyingPointsLoop		24
#define kMaxCandles					20
#define kMaxTikis					8
#define kMaxCoals					8
#define kMaxPendulums				8
#define kMaxHotSpots				56
#define kMaxSavedMaps				24
#define kMaxRubberBands				2
#define kMaxGrease					16
#define kMaxStars					4
#define kMaxShredded				4
#define kMaxDynamicObs				18
#define kMaxMasterObjects			216		// kMaxRoomObs * 9
#define kMaxViewWidth				1536
#define kMaxViewHeight				1026

#define kSelectTool					0

#define kBlowerMode					1
#define kFurnitureMode				2
#define kBonusMode					3
#define kTransportMode				4
#define kSwitchMode					5
#define kLightMode					6
#define kApplianceMode				7
#define kEnemyMode					8
#define kClutterMode				9

#define kIgnoreIt					0			//   ееееее
#define kLiftIt						1			//  ее    ее
#define kDropIt						2			//  ееееееее
#define kPushItLeft					3			//  ее    ее
#define kPushItRight				4			//  ее    ее
#define kDissolveIt					5			//
#define kRewardIt					6			//   ееееее
#define kMoveItUp					7			//  ее    ее
#define kMoveItDown					8			//  ее
#define kSwitchIt					9			//  ее    ее
#define kShredIt					10			//   ееееее
#define kStrumIt					11			//
#define kTriggerIt					12			//  ееееееее
#define kBurnIt						13			//     ее
#define kSlideIt					14			//     ее
#define kTransportIt				15			//     ее
#define kIgnoreLeftWall				16			//     ее
#define kIgnoreRightWall			17			//
#define kMailItLeft					18			//   ееееее
#define kMailItRight				19			//     ее
#define kDuctItDown					20			//     ее
#define kDuctItUp					21			//     ее
#define kMicrowaveIt				22			//   ееееее
#define kIgnoreGround				23			//
#define kBounceIt					24			//
#define kChimeIt					25			//     ее
#define kWebIt						26			//     ее
#define kSoundIt					27

#define kFloorVent					0x01		// Blowers
#define kCeilingVent				0x02
#define kFloorBlower				0x03
#define kCeilingBlower				0x04
#define kSewerGrate					0x05
#define kLeftFan					0x06
#define kRightFan					0x07
#define kTaper						0x08
#define kCandle						0x09
#define kStubby						0x0A
#define kTiki						0x0B
#define kBBQ						0x0C
#define kInvisBlower				0x0D
#define kGrecoVent					0x0E
#define kSewerBlower				0x0F
#define kLiftArea					0x10

#define kTable						0x11		// Furniture
#define kShelf						0x12
#define kCabinet					0x13
#define kFilingCabinet				0x14
#define kWasteBasket				0x15
#define kMilkCrate					0x16
#define kCounter					0x17
#define kDresser					0x18
#define kDeckTable					0x19
#define kStool						0x1A
#define kTrunk						0x1B
#define kInvisObstacle				0x1C
#define kManhole					0x1D
#define kBooks						0x1E
#define kInvisBounce				0x1F

#define kRedClock					0x21		// Prizes
#define kBlueClock					0x22
#define kYellowClock				0x23
#define kCuckoo						0x24
#define kPaper						0x25
#define kBattery					0x26
#define kBands						0x27
#define kGreaseRt					0x28
#define kGreaseLf					0x29
#define kFoil						0x2A
#define kInvisBonus					0x2B
#define kStar						0x2C
#define kSparkle					0x2D
#define kHelium						0x2E
#define kSlider						0x2F

#define kUpStairs					0x31		// Transport
#define kDownStairs					0x32
#define kMailboxLf					0x33
#define kMailboxRt					0x34
#define kFloorTrans					0x35
#define kCeilingTrans				0x36
#define kDoorInLf					0x37
#define kDoorInRt					0x38
#define kDoorExRt					0x39
#define kDoorExLf					0x3A
#define kWindowInLf					0x3B
#define kWindowInRt					0x3C
#define kWindowExRt					0x3D
#define kWindowExLf					0x3E
#define kInvisTrans					0x3F
#define kDeluxeTrans				0x40

#define kLightSwitch				0x41		// Switches
#define kMachineSwitch				0x42
#define kThermostat					0x43
#define kPowerSwitch				0x44
#define kKnifeSwitch				0x45
#define kInvisSwitch				0x46
#define kTrigger					0x47
#define kLgTrigger					0x48
#define kSoundTrigger				0x49

#define kCeilingLight				0x51		// Lights
#define kLightBulb					0x52
#define kTableLamp					0x53
#define kHipLamp					0x54
#define kDecoLamp					0x55
#define kFlourescent				0x56
#define kTrackLight					0x57
#define kInvisLight					0x58

#define kShredder					0x61		// Appliances
#define kToaster					0x62
#define kMacPlus					0x63
#define kGuitar						0x64
#define kTV							0x65
#define kCoffee						0x66
#define kOutlet						0x67
#define kVCR						0x68
#define kStereo						0x69
#define kMicrowave					0x6A
#define kCinderBlock				0x6B
#define kFlowerBox					0x6C
#define kCDs						0x6D
#define kCustomPict					0x6E

#define kBalloon					0x71		// Enemies
#define kCopterLf					0x72
#define kCopterRt					0x73
#define kDartLf						0x74
#define kDartRt						0x75
#define kBall						0x76
#define kDrip						0x77
#define kFish						0x78
#define kCobweb						0x79

#define kOzma						0x81		// Clutter
#define kMirror						0x82
#define kMousehole					0x83
#define kFireplace					0x84
#define kFlower						0x85
#define kWallWindow					0x86
#define kBear						0x87
#define kCalendar					0x88
#define kVase1						0x89
#define kVase2						0x8A
#define kBulletin					0x8B
#define kCloud						0x8C
#define kFaucet						0x8D
#define kRug						0x8E
#define kChimes						0x8F

#define kNumSrcRects				0x90

#define kTableThick					8
#define kShelfThick					6
#define kToggle						0
#define kForceOn					1
#define kForceOff					2
#define kOneShot					3
#define kNumTrackLights				3
#define kNumOutletPicts				4
#define kNumCandleFlames			5
#define kNumTikiFlames				5
#define kNumBBQCoals				4
#define kNumPendulums				3
#define kNumBreadPicts				6
#define kNumBalloonFrames			8
#define kNumCopterFrames			10
#define kNumDartFrames				4
#define kNumBallFrames				2
#define kNumDripFrames				6
#define kNumFishFrames				8
#define kNumFlowers					6

#define	kNumMarqueePats				7
#define kObjectNameStrings			1007

#define kSwitchLinkOnly				3
#define kTriggerLinkOnly			4
#define kTransportLinkOnly			5

#define kFloorVentTop				305
#define kCeilingVentTop				8
#define kFloorBlowerTop				304
#define kCeilingBlowerTop			5
#define kSewerGrateTop				303
#define kCeilingTransTop			6
#define kFloorTransTop				302
#define kStairsTop					28
#define kCounterBottom				304
#define kDresserBottom				293
#define kCeilingLightTop			4
#define kHipLampTop					23
#define kDecoLampTop				91
#define kFlourescentTop				12
#define kTrackLightTop				5

#define kDoorInTop					0
#define kDoorInLfLeft				0
#define kDoorInRtLeft				368
#define kDoorExTop					0
#define kDoorExLfLeft				0
#define kDoorExRtLeft				496
#define kWindowInTop				64
#define kWindowInLfLeft				0
#define kWindowInRtLeft				492
#define kWindowExTop				64
#define kWindowExLfLeft				0
#define kWindowExRtLeft				496

#define kNumTiles					8
#define kTileWide					64
#define kTileHigh					322
#define kRoomWide					512			// kNumTiles * kTileWide
#define kFloorSupportTall			44
#define kVertLocalOffset			322			// kTileHigh - 39 (was 283, then 295)

#define kCeilingLimit				8
#define kFloorLimit					312
#define kRoofLimit					122
#define kLeftWallLimit				12
#define kNoLeftWallLimit			-24			// 0 - (kGliderWide / 2)
#define kRightWallLimit				500
#define kNoRightWallLimit			536			// kRoomWide + (kGliderWide / 2)
#define kNoCeilingLimit				-10
#define kNoFloorLimit				332

#define kScoreboardHigh				0
#define kScoreboardLow				1
#define kScoreboardTall				20

#define kHouseVersion				0x0200
#define kNewHouseVersion			0x0300
#define kBaseBackgroundID			2000
#define kFirstOutdoorBack			2009
#define kNumBackgrounds				18
#define kUserBackground				3000
#define kUserStructureRange			3300
#define kSplash8BitPICT				1000
#define kRoomIsEmpty				-1
#define kObjectIsEmpty				-1
#define kNoObjectSelected			-1
#define kInitialGliderSelected		-2
#define kLeftGliderSelected			-3
#define kRightGliderSelected		-4
#define kWindoidWDEF				2048
#define kWindoidGrowWDEF			2064
#define kTicksPerFrame				2
#define kStarPictID					1995
#define kNumUndergroundFloors		8
#define kRoomVisitScore				100
#define kRedClockPoints				100
#define kBlueClockPoints			300
#define kYellowClockPoints			500
#define kCuckooClockPoints			1000
#define kStarPoints					5000
#define kRedOrangeColor8			23		// actually, 18
#define kMaxNumRoomsH				128
#define kMaxNumRoomsV				64
#define kStartSparkle				4
#define kLengthOfZap				30

#define kGliderWide					48
#define kGliderHigh					20
#define kHalfGliderWide				24
#define kGliderBurningHigh			26
#define kShadowHigh					9
#define kShadowTop					306
#define kFaceRight					TRUE
#define kFaceLeft					FALSE
#define kPlayer1					TRUE
#define kPlayer2					FALSE
#define kNumGliderSrcRects			31
#define kNumShadowSrcRects			2
#define kFirstAboutFaceFrame		18
#define kLastAboutFaceFrame			20
#define kWasBurning					2
#define kLeftFadeOffset				7
#define kLastFadeSequence			16
#define kGliderFoil2PictID			3963
#define kGlider2PictID				3974
#define kGliderFoilPictID			3976
#define kGliderPictID				3999
#define kGliderStartsDown			32

#define kGliderNormal				0			//  ее    ее
#define kGliderFadingIn				1			//  еее  еее
#define kGliderFadingOut			2			//  ее ее ее
#define kGliderGoingUp				3			//  ее    ее
#define kGliderComingUp				4			//  ее    ее
#define kGliderGoingDown			5			// 
#define kGliderComingDown			6			//   ееееее
#define kGliderFaceLeft				7			//  ее    ее
#define kGliderFaceRight			8			//  ее    ее
#define kGliderBurning				9			//  ее    ее
#define kGliderTransporting			10			//   ееееее
#define kGliderDuctingDown			11			//
#define kGliderDuctingUp			12			//  еееееее
#define kGliderDuctingIn			13			//  ее    ее
#define kGliderMailInLeft			14			//  ее    ее
#define kGliderMailOutLeft			15			//  ее    ее
#define kGliderMailInRight			16			//  еееееее
#define kGliderMailOutRight			17			//
#define kGliderGoingFoil			18			//  ееееееее
#define kGliderLosingFoil			19			//  ее
#define kGliderShredding			20			//  ееее
#define kGliderInLimbo				21			//  ее
#define kGliderIdle					22			//  ееееееее
#define kGliderTransportingIn		23

#define kPlayerIsDeadForever		-69
#define kPlayerMailedOut			-12
#define kPlayerDuckedOut			-11
#define kPlayerTransportedOut		-10
#define kPlayerEscapingDownStairs	-9
#define kPlayerEscapingUpStairs		-8
#define kPlayerEscapedDownStairs	-7
#define kPlayerEscapedUpStairs		-6
#define kPlayerEscapedDown			-5
#define kPlayerEscapedUp			-4
#define kPlayerEscapedLeft			-3
#define kPlayerEscapedRight			-2
#define kNoOneEscaped				-1

#define kLinkedToOther				0
#define kLinkedToLeftMailbox		1
#define kLinkedToRightMailbox		2
#define kLinkedToCeilingDuct		3
#define kLinkedToFloorDuct			4

#define kResumeGameMode				0
#define kNewGameMode				1

#define kNormalTitleMode			0
#define kEscapedTitleMode			1
#define kSavingTitleMode			2

#define kScoreboardPictID			1997

#define kDemoLength					6702
