
//============================================================================
//----------------------------------------------------------------------------
//								Externs.h
//----------------------------------------------------------------------------
//============================================================================


#pragma once


#include <Menus.h>


#define kPreferredDepth				8


#define	kNilPointer					0L
#define	kPutInFront					(WindowPtr)-1L
#define	kNormalUpdates				TRUE
#define kOneKilobyte				1024
#define kOkayButton					1
#define kCancelButton				2
#define kControlActive				0
#define kControlInactive			255
#define kAsynch						TRUE
#define kSynch						FALSE

#define kHomeKeyASCII				0x01
#define kEnterKeyASCII				0x03
#define kEndKeyASCII				0x04
#define kHelpKeyASCII				0x05
#define	kDeleteKeyASCII				0x08
#define kTabKeyASCII				0x09
#define kPageUpKeyASCII				0x0B
#define kPageDownKeyASCII			0x0C
#define	kReturnKeyASCII				0x0D
#define	kFunctionKeyASCII			0x10
#define	kClearKeyASCII				0x1A
#define	kEscapeKeyASCII				0x1B
#define	kLeftArrowKeyASCII			0x1C
#define kRightArrowKeyASCII			0x1D
#define	kUpArrowKeyASCII			0x1E
#define kDownArrowKeyASCII			0x1F
#define kSpaceBarASCII				0x20
#define kExclamationASCII			0x21
#define kPlusKeyASCII				0x2B
#define kMinusKeyASCII				0x2D
#define k0KeyASCII					0x30
#define k1KeyASCII					0x31
#define k2KeyASCII					0x32
#define k3KeyASCII					0x33
#define k4KeyASCII					0x34
#define k5KeyASCII					0x35
#define k6KeyASCII					0x36
#define k7KeyASCII					0x37
#define k8KeyASCII					0x38
#define k9KeyASCII					0x39

#define kCapAKeyASCII				0x41
#define kCapBKeyASCII				0x42
#define kCapCKeyASCII				0x43
#define kCapDKeyASCII				0x44
#define kCapEKeyASCII				0x45
#define kCapFKeyASCII				0x46
#define kCapGKeyASCII				0x47
#define kCapHKeyASCII				0x48
#define kCapIKeyASCII				0x49
#define kCapJKeyASCII				0x4A
#define kCapKKeyASCII				0x4B
#define kCapLKeyASCII				0x4C
#define kCapMKeyASCII				0x4D
#define kCapNKeyASCII				0x4E
#define kCapOKeyASCII				0x4F
#define kCapPKeyASCII				0x50
#define kCapQKeyASCII				0x51
#define kCapRKeyASCII				0x52
#define kCapSKeyASCII				0x53
#define kCapTKeyASCII				0x54
#define kCapUKeyASCII				0x55
#define kCapVKeyASCII				0x56
#define kCapWKeyASCII				0x57
#define kCapXKeyASCII				0x58
#define kCapYKeyASCII				0x59
#define kCapZKeyASCII				0x5A

#define kAKeyASCII					0x61
#define kBKeyASCII					0x62
#define kCKeyASCII					0x63
#define kDKeyASCII					0x64
#define kEKeyASCII					0x65
#define kFKeyASCII					0x66
#define kGKeyASCII					0x67
#define kHKeyASCII					0x68
#define kIKeyASCII					0x69
#define kJKeyASCII					0x6A
#define kKKeyASCII					0x6B
#define kLKeyASCII					0x6C
#define kMKeyASCII					0x6D
#define kNKeyASCII					0x6E
#define kOKeyASCII					0x6F
#define kPKeyASCII					0x70
#define kQKeyASCII					0x71
#define kRKeyASCII					0x72
#define kSKeyASCII					0x73
#define kTKeyASCII					0x74
#define kUKeyASCII					0x75
#define kVKeyASCII					0x76
#define kWKeyASCII					0x77
#define kXKeyASCII					0x78
#define kYKeyASCII					0x79
#define kZKeyASCII					0x7A
#define kForwardDeleteASCII			0x7F

#define	kPlusKeypadMap				66		// key map offset for + on keypad
#define	kMinusKeypadMap				73		// key map offset for - on keypad
#define	kTimesKeypadMap				68		// key map offset for * on keypad
#define	k0KeypadMap					85		// key map offset for 0 on keypad
#define	k1KeypadMap					84		// key map offset for 1 on keypad
#define	k2KeypadMap					83		// key map offset for 2 on keypad
#define	k3KeypadMap					82		// key map offset for 3 on keypad
#define k4KeypadMap					81		// key map offset for 4 on keypad
#define k5KeypadMap					80		// key map offset for 5 on keypad
#define k6KeypadMap					95		// key map offset for 6 on keypad
#define k7KeypadMap					94		// key map offset for 7 on keypad
#define k8KeypadMap					92		// key map offset for 8 on keypad
#define k9KeypadMap					91		// key map offset for 9 on keypad

#define kUpArrowKeyMap				121		// key map offset for up arrow
#define kDownArrowKeyMap			122		// key map offset for down arrow
#define kRightArrowKeyMap			123		// key map offset for right arrow
#define kLeftArrowKeyMap			124		// key map offset for left arrow

#define kAKeyMap					7
#define	kBKeyMap					12
#define	kCKeyMap					15
#define	kDKeyMap					5
#define	kEKeyMap					9
#define	kFKeyMap					4
#define	kGKeyMap					2
#define	kHKeyMap					3
#define	kMKeyMap					41
#define	kNKeyMap					42
#define	kOKeyMap					24
#define	kPKeyMap					36
#define	kQKeyMap					11
#define	kRKeyMap					8
#define kSKeyMap					6
#define kTKeyMap					22
#define kVKeyMap					14
#define kWKeyMap					10
#define kXKeyMap					0
#define kZKeyMap					1
#define kPeriodKeyMap				40
#define	kCommandKeyMap				48
#define	kEscKeyMap					50
#define	kDeleteKeyMap				52
#define kSpaceBarMap				54
#define	kTabKeyMap					55
#define	kControlKeyMap				60
#define	kOptionKeyMap				61
#define	kCapsLockKeyMap				62
#define	kShiftKeyMap				63

#define kTabRawKey					0x30	// key map offset for Tab key
#define kClearRawKey				0x47	// key map offset for Clear key
#define kF5RawKey					0x60	// key map offset for F5
#define kF6RawKey					0x61	// key map offset for F6
#define kF7RawKey					0x62	// key map offset for F7
#define kF3RawKey					0x63	// key map offset for F3
#define kF8RawKey					0x64	// key map offset for F8
#define kF9RawKey					0x65	// key map offset for F9
#define kF11RawKey					0x67	// key map offset for F11
#define kF13RawKey					0x69	// key map offset for F13
#define kF14RawKey					0x6B	// key map offset for F14
#define kF10RawKey					0x6D	// key map offset for F10
#define kF12RawKey					0x6F	// key map offset for F12
#define kF15RawKey					0x71	// key map offset for F15
#define kF4RawKey					0x76	// key map offset for F4
#define kF2RawKey					0x78	// key map offset for F2
#define kF1RawKey					0x7A	// key map offset for F1

#define	kErrUnnaccounted			1
#define	kErrNoMemory				2
#define kErrDialogDidntLoad			3
#define kErrFailedResourceLoad		4
#define kErrFailedGraphicLoad		5
#define kErrFailedOurDirect			6
#define kErrFailedValidation		7
#define kErrNeedSystem7				8
#define kErrFailedGetDevice			9
#define kErrFailedMemoryOperation	10
#define kErrFailedCatSearch			11
#define kErrNeedColorQD				12
#define kErrNeed16Or256Colors		13

#define iAbout					1
#define iNewGame				1
#define iTwoPlayer				2
#define iOpenSavedGame			3
#define iLoadHouse				5
#define iQuit					7
#define iEditor					1
#define iHighScores				3
#define iPrefs					4
#define iHelp					5
#define iNewHouse				1
#define iSave					2
#define iHouse					4
#define iRoom					5
#define iObject					6
#define iCut					8
#define iCopy					9
#define iPaste					10
#define iClear					11
#define iDuplicate				12
#define iBringForward			14
#define iSendBack				15
#define iGoToRoom				17
#define iMapWindow				19
#define iObjectWindow			20
#define iCoordinateWindow		21

//--------------------------------------------------------------  Structs
/*
typedef	short		SICN[16];
typedef	SICN		*SICNList;
typedef	SICNList	*SICNHand;
*/

#pragma options align=mac68k

typedef struct
{
	Str32		wasDefaultName;
	Str15		wasLeftName, wasRightName;
	Str15		wasBattName, wasBandName;
	Str15		wasHighName;
	Str31		wasHighBanner;
//	long		encrypted, fakeLong;
	long		wasLeftMap, wasRightMap;
	long		wasBattMap, wasBandMap;
	short		wasVolume;
	short		prefVersion;
	short		wasMaxFiles;
	short		wasEditH, wasEditV;
	short		wasMapH, wasMapV;
	short		wasMapWide, wasMapHigh;
	short		wasToolsH, wasToolsV;
	short		wasLinkH, wasLinkV;
	short		wasCoordH, wasCoordV;
	short		isMapLeft, isMapTop;
	short		wasNumNeighbors;
	short		wasDepthPref;
	short		wasToolGroup;
	short		smWarnings;
	short		wasFloor, wasSuite;
	Boolean		wasZooms, wasMusicOn;
	Boolean		wasAutoEdit, wasDoColorFade;
	Boolean		wasMapOpen, wasToolsOpen;
	Boolean		wasCoordOpen, wasQuickTrans;
	Boolean		wasIdleMusic, wasGameMusic;
	Boolean		wasEscPauseKey;
	Boolean		wasDoAutoDemo, wasScreen2;
	Boolean		wasDoBackground, wasHouseChecks;
	Boolean		wasPrettyMap, wasBitchDialogs;
} prefsInfo;

#pragma options align=reset

//--------------------------------------------------------------  Prototypes

void DoAbout (void);									// --- About.c

void LoadCursors (void);								// --- AnimCursor.c
void DisposCursors (void);
void IncrementCursor (void);
void DecrementCursor (void);
void SpinCursor (short);
void BackSpinCursor (short);

void ColorText (StringPtr, long);						// --- ColorUtils.c
void ColorRect (Rect *, long);
void ColorOval (Rect *, long);
void ColorRegion (RgnHandle, long);
void ColorLine (short, short, short, short, long);
void HiliteRect (Rect *, short, short);
void ColorFrameRect (Rect *, long);
void ColorFrameWHRect (short, short, short, short, long);
void ColorFrameOval (Rect *, long);
void LtGrayForeColor (void);
void GrayForeColor (void);
void DkGrayForeColor (void);
void RestoreColorsSlam (void);

void MonitorWait (void);								// --- DebugUtils.c
void DisplayRect (Rect *);
void FlashRect (Rect *);
void CheckLegitRect(Rect *, Rect *);
void DisplayLong (long);
void DisplayShort (short);
void FlashLong (long);
void FlashShort (short);
void DoBarGraph (short, short, short, short);
short BetaOkay (void);
void DebugNum (long);
void DisplayCTSeed (CGrafPtr);
void FillScreenRed (void);
void DumpToResEditFile (Ptr, long);

void HandleEvent (void);								// --- Event.c
void HiliteAllWindows (void);
void IgnoreThisClick (void);

short WhatsOurDepth (void);								// --- Environs.c
void SwitchToDepth (short, Boolean);
void CheckOurEnvirons (void);
//void ReflectSecondMonitorEnvirons (Boolean, Boolean, Boolean);
void HandleDepthSwitching (void);
void RestoreColorDepth (void);
void CheckMemorySize (void);
void SetAppMemorySize (long);

Boolean CheckFileError (short, StringPtr);				// --- File Error.c

Boolean SavePrefs (prefsInfo *, short);					// --- Prefs.c
Boolean LoadPrefs (prefsInfo *, short);

void PasStringCopy (StringPtr, StringPtr);				// --- StringUtils.c
short WhichStringFirst (StringPtr, StringPtr);
void PasStringCopyNum (StringPtr, StringPtr, short);
void PasStringConcat (StringPtr, StringPtr);
void GetLineOfText (StringPtr, short, StringPtr);
void WrapText (StringPtr, short);
void GetFirstWordOfString (StringPtr, StringPtr);
void CollapseStringToWidth (StringPtr, short);
void GetChooserName (StringPtr);
StringPtr GetLocalizedString (short, StringPtr);

Point MyGetGlobalMouse (void);							// --- Utilities.c
void ToolBoxInit (void);
void FindOurDevice (void);
short RandomInt (short);
long RandomLong (long);
void InitRandomLongQUS (void);
unsigned long RandomLongQUS (void);
//void CenterAlert (short);
void RedAlert (short);
//void CreateOffScreenBitMap (Rect *, GrafPtr *);
//void CreateOffScreenPixMap (Rect *, CGrafPtr *);
//void KillOffScreenPixMap (CGrafPtr);
//void KillOffScreenBitMap (GrafPtr);
void LoadGraphic (short);
void LoadScaledGraphic (short, Rect *);
//void PlotSICN (Rect *, SICNHand, long);
void LargeIconPlot (Rect *, short);
void DrawCIcon (short, short, short);
char KeyMapOffsetFromRawKey (char);
long LongSquareRoot (long);
//void HideMenuBarOld (void);
//void ShowMenuBarOld (void);
Boolean WaitForInputEvent (short);
void WaitCommandQReleased (void);
char GetKeyMapFromMessage (long);
void GetKeyName (long, StringPtr);
Boolean OptionKeyDown (void);
long ExtractCTSeed (CGrafPtr);
//void ForceCTSeed (CGrafPtr, long);
void DelayTicks (long);
void UnivGetSoundVolume (short *, Boolean);
void  UnivSetSoundVolume (short, Boolean);

Boolean ValidInstallation (Boolean);					// --- Validate.c

void GetWindowLeftTop (WindowPtr, short *, short *);	// --- WindowUtils.c
void GetWindowRect (WindowPtr, Rect *);
void GetLocalWindowRect (WindowPtr, Rect *);
//void FlagWindowFloating (WindowPtr);
//Boolean	IsWindowFloating (WindowPtr);
void OpenMessageWindow (StringPtr);
void SetMessageWindowMessage (StringPtr);
void CloseMessageWindow (void);
void CloseThisWindow (WindowPtr *);

#ifdef powerc
//	extern pascal void SetSoundVol(short level);		// for old Sound Manager
//	extern pascal void GetSoundVol(short *level)
//	THREEWORDINLINE(0x4218, 0x10B8, 0x0260);
#endif

#include "GliderDefines.h"
#include "GliderStructs.h"
#include "GliderVars.h"
#include "GliderProtos.h"