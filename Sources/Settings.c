
//============================================================================
//----------------------------------------------------------------------------
//									Settings.c
//----------------------------------------------------------------------------
//============================================================================


#include <Sound.h>
#include <TextUtils.h>
#include "DialogUtils.h"
#include "Externs.h"
#include "Environ.h"
#include "House.h"


#define kMainPrefsDialID		1012
#define kDisplayPrefsDialID		1017
#define kSoundPrefsDialID		1018
#define kControlPrefsDialID		1023
#define kBrainsPrefsDialID		1024
#define kDisplayButton			3
#define kSoundButton			4
#define kControlsButton			5
#define kBrainsButton			6
#define kDisplay1Item			3
#define kDisplay3Item			4
#define kDisplay9Item			5
#define kDoColorFadeItem		9
#define kCurrentDepth			10
#define k256Depth				11
#define k16Depth				12
#define kDispDefault			15
#define kUseQDItem				16
#define kUseScreen2Item			17
#define kSofterItem				4
#define kLouderItem				5
#define kVolNumberItem			7
#define kIdleMusicItem			8
#define kPlayMusicItem			9
#define kSoundDefault			13
#define kRightControl			5
#define kLeftControl			6
#define kBattControl			7
#define kBandControl			8
#define kControlDefaults		13
#define kESCPausesRadio			14
#define kTABPausesRadio			15
#define kMaxFilesItem			5
#define kQuickTransitCheck		7
#define kDoZoomsCheck			8
#define kBrainsDefault			9
#define kDoDemoCheck			10
#define kDoBackgroundCheck		11
#define kDoErrorCheck			12
#define kDoPrettyMapCheck		13
#define kDoBitchDlgsCheck		14


void SetBrainsToDefaults (DialogPtr);
void UpdateSettingsBrains (DialogPtr);
pascal Boolean BrainsFilter (DialogPtr, EventRecord *, short *);
void DoBrainsPrefs (void);
void SetControlsToDefaults (DialogPtr);
void UpdateControlKeyName (DialogPtr);
void UpdateSettingsControl (DialogPtr);
pascal Boolean ControlFilter (DialogPtr, EventRecord *, short *);
void DoControlPrefs (void);
void SoundDefaults (DialogPtr);
void UpdateSettingsSound (DialogPtr);
void HandleSoundMusicChange (short, Boolean);
pascal Boolean SoundFilter (DialogPtr, EventRecord *, short *);
void DoSoundPrefs (void);
void DisplayDefaults (void);
void FrameDisplayIcon (DialogPtr);
void DisplayUpdate (DialogPtr);
pascal Boolean DisplayFilter (DialogPtr, EventRecord *, short *);
void DoDisplayPrefs (void);
void SetAllDefaults (void);
void FlashSettingsButton (short);
void UpdateSettingsMain (DialogPtr);
pascal Boolean PrefsFilter (DialogPtr, EventRecord *, short *);
void BitchAboutChanges (void);


Rect		prefButton[4], controlRects[4];
Str15		leftName, rightName, batteryName, bandName;
Str15		tempLeftStr, tempRightStr, tempBattStr, tempBandStr;
long		tempLeftMap, tempRightMap, tempBattMap, tempBandMap;
short		whichCtrl, wasDepthPref;
Boolean		wasFade, wasIdle, wasPlay, wasTransit, wasZooms, wasBackground;
Boolean		wasEscPauseKey, wasDemos, wasScreen2, nextRestartChange, wasErrorCheck;
Boolean		wasPrettyMap, wasBitchDialogs;

extern	short		numNeighbors, isDepthPref, maxFiles, willMaxFiles;
extern	Boolean		isDoColorFade, isPlayMusicIdle, isUseSecondScreen;
extern	Boolean		isHouseChecks, doBitchDialogs;
extern	Boolean		isEscPauseKey, failedMusic, isSoundOn, doBackground;
extern	Boolean		isMusicOn, quickerTransitions, doAutoDemo;
extern	Boolean		changeLockStateOfHouse, saveHouseLocked, doPrettyMap;


//==============================================================  Functions
//--------------------------------------------------------------  SetBrainsToDefaults

void SetBrainsToDefaults (DialogPtr theDialog)
{
	SetDialogNumToStr(theDialog, kMaxFilesItem, 24L);
#ifdef powerc
	wasTransit = false;
#else
	wasTransit = true;
#endif
	wasZooms = true;
	wasDemos = true;
	wasBackground = false;
	wasErrorCheck = true;
	wasPrettyMap = true;
	wasBitchDialogs = true;
	SetDialogItemValue(theDialog, kQuickTransitCheck, (short)wasTransit);
	SetDialogItemValue(theDialog, kDoZoomsCheck, (short)wasZooms);
	SetDialogItemValue(theDialog, kDoDemoCheck, (short)wasDemos);
	SetDialogItemValue(theDialog, kDoBackgroundCheck, (short)wasBackground);
	SetDialogItemValue(theDialog, kDoErrorCheck, (short)wasErrorCheck);
	SetDialogItemValue(theDialog, kDoPrettyMapCheck, (short)wasPrettyMap);
	SetDialogItemValue(theDialog, kDoBitchDlgsCheck, (short)wasBitchDialogs);
}

//--------------------------------------------------------------  UpdateSettingsBrains

void UpdateSettingsBrains (DialogPtr theDialog)
{
	DrawDialog(theDialog);
	DrawDefaultButton(theDialog);
	
	SetDialogNumToStr(theDialog, kMaxFilesItem, (long)willMaxFiles);
	SelectDialogItemText(theDialog, kMaxFilesItem, 0, 1024);
	
	FrameDialogItemC(theDialog, 3, kRedOrangeColor8);
}

//--------------------------------------------------------------  BrainsFilter

pascal Boolean BrainsFilter (DialogPtr dial, EventRecord *event, short *item)
{
	switch (event->what)
	{
		case keyDown:
		switch ((event->message) & charCodeMask)
		{
			case kReturnKeyASCII:
			case kEnterKeyASCII:
			FlashDialogButton(dial, kOkayButton);
			*item = kOkayButton;
			return(true);
			break;
			
			case kEscapeKeyASCII:
			FlashDialogButton(dial, kCancelButton);
			*item = kCancelButton;
			return(true);
			break;
			
			case kCapAKeyASCII:
			case kAKeyASCII:
			*item = kDoDemoCheck;
			return(true);
			break;
			
			case kCapBKeyASCII:
			case kBKeyASCII:
			*item = kDoBackgroundCheck;
			return(true);
			break;
			
			case kCapDKeyASCII:
			case kDKeyASCII:
			*item = kBrainsDefault;
			FlashDialogButton(dial, kBrainsDefault);
			return(true);
			break;
			
			case kCapEKeyASCII:
			case kEKeyASCII:
			*item = kDoErrorCheck;
			return(true);
			break;
			
			case kCapQKeyASCII:
			case kQKeyASCII:
			*item = kQuickTransitCheck;
			return(true);
			break;
			
			case kCapZKeyASCII:
			case kZKeyASCII:
			*item = kDoZoomsCheck;
			return(true);
			break;
			
			default:
			return(false);
		}
		break;
		
		case mouseDown:
		return(false);
		break;
		
		case updateEvt:
		SetPort((GrafPtr)dial);
		BeginUpdate(GetDialogWindow(dial));
		UpdateSettingsBrains(dial);
		EndUpdate(GetDialogWindow(dial));
		event->what = nullEvent;
		return(false);
		break;
		
		default:
		return(false);
		break;
	}
}

//--------------------------------------------------------------  DoBrainsPrefs

void DoBrainsPrefs (void)
{
	DialogPtr		prefDlg;
	long			tempLong;
	short			itemHit, wasMaxFiles;
	Boolean			leaving;
	ModalFilterUPP	brainsFilterUPP;
	
	brainsFilterUPP = NewModalFilterUPP(BrainsFilter);
	
	BringUpDialog(&prefDlg, kBrainsPrefsDialID);
	leaving = false;
	wasMaxFiles = willMaxFiles;
	
	wasTransit = quickerTransitions;
	wasZooms = doZooms;
	wasDemos = doAutoDemo;
	wasBackground = doBackground;
	wasErrorCheck = isHouseChecks;
	wasPrettyMap = doPrettyMap;
	wasBitchDialogs = doBitchDialogs;
	
	SetDialogItemValue(prefDlg, kQuickTransitCheck, (short)wasTransit);
	SetDialogItemValue(prefDlg, kDoZoomsCheck, (short)wasZooms);
	SetDialogItemValue(prefDlg, kDoDemoCheck, (short)wasDemos);
	SetDialogItemValue(prefDlg, kDoBackgroundCheck, (short)wasBackground);
	SetDialogItemValue(prefDlg, kDoErrorCheck, (short)wasErrorCheck);
	SetDialogItemValue(prefDlg, kDoPrettyMapCheck, (short)wasPrettyMap);
	SetDialogItemValue(prefDlg, kDoBitchDlgsCheck, (short)wasBitchDialogs);
	
	while (!leaving)
	{
		ModalDialog(brainsFilterUPP, &itemHit);
		switch (itemHit)
		{
			case kOkayButton:
			GetDialogNumFromStr(prefDlg, kMaxFilesItem, &tempLong);
			if (tempLong > 500)
				tempLong = 500;
			else if (tempLong < 12)
				tempLong = 12;
			willMaxFiles = tempLong;
			if (willMaxFiles != wasMaxFiles)
				nextRestartChange = true;
			quickerTransitions = wasTransit;
			doZooms = wasZooms;
			doAutoDemo = wasDemos;
			doBackground = wasBackground;
			isHouseChecks = wasErrorCheck;
			doPrettyMap = wasPrettyMap;
			doBitchDialogs = wasBitchDialogs;
			leaving = true;
			break;
			
			case kCancelButton:
			willMaxFiles = wasMaxFiles;
			leaving = true;
			break;
			
			case kQuickTransitCheck:
			wasTransit = !wasTransit;
			SetDialogItemValue(prefDlg, kQuickTransitCheck, (short)wasTransit);
			break;
			
			case kDoZoomsCheck:
			wasZooms = !wasZooms;
			SetDialogItemValue(prefDlg, kDoZoomsCheck, (short)wasZooms);
			break;
			
			case kDoDemoCheck:
			wasDemos = !wasDemos;
			SetDialogItemValue(prefDlg, kDoDemoCheck, (short)wasDemos);
			break;
			
			case kDoBackgroundCheck:
			wasBackground = !wasBackground;
			SetDialogItemValue(prefDlg, kDoBackgroundCheck, (short)wasBackground);
			break;
			
			case kBrainsDefault:
			SetBrainsToDefaults(prefDlg);
			break;
			
			case kDoErrorCheck:
			wasErrorCheck = !wasErrorCheck;
			SetDialogItemValue(prefDlg, kDoErrorCheck, (short)wasErrorCheck);
			break;
			
			case kDoPrettyMapCheck:
			wasPrettyMap = !wasPrettyMap;
			SetDialogItemValue(prefDlg, kDoPrettyMapCheck, (short)wasPrettyMap);
			break;
			
			case kDoBitchDlgsCheck:
			wasBitchDialogs = !wasBitchDialogs;
			SetDialogItemValue(prefDlg, kDoBitchDlgsCheck, (short)wasBitchDialogs);
			break;
		}
	}
	
	DisposeDialog(prefDlg);
	DisposeModalFilterUPP(brainsFilterUPP);
}

//--------------------------------------------------------------  SetControlsToDefaults

void SetControlsToDefaults (DialogPtr theDialog)
{
	PasStringCopy("\plf arrow", tempLeftStr);
	PasStringCopy("\prt arrow", tempRightStr);
	PasStringCopy("\pdn arrow", tempBattStr);
	PasStringCopy("\pup arrow", tempBandStr);
	tempLeftMap = kLeftArrowKeyMap;
	tempRightMap = kRightArrowKeyMap;
	tempBattMap = kDownArrowKeyMap;
	tempBandMap = kUpArrowKeyMap;
	wasEscPauseKey = false;
	SelectFromRadioGroup(theDialog, kTABPausesRadio, 
				kESCPausesRadio, kTABPausesRadio);
}

//--------------------------------------------------------------  UpdateControlKeyName

void UpdateControlKeyName (DialogPtr theDialog)
{
	DrawDialogUserText(theDialog, kRightControl + 4, tempRightStr, whichCtrl == 0);
	DrawDialogUserText(theDialog, kLeftControl + 4, tempLeftStr, whichCtrl == 1);
	DrawDialogUserText(theDialog, kBattControl + 4, tempBattStr, whichCtrl == 2);
	DrawDialogUserText(theDialog, kBandControl + 4, tempBandStr, whichCtrl == 3);
}

//--------------------------------------------------------------  UpdateSettingsControl

void UpdateSettingsControl (DialogPtr theDialog)
{
	short		i;
	
	DrawDialog(theDialog);
	
	PenSize(2, 2);
	ForeColor(whiteColor);
	for (i = 0; i < 4; i++)
		FrameRect(&controlRects[i]);
	ForeColor(redColor);
	FrameRect(&controlRects[whichCtrl]);
	ForeColor(blackColor);
	PenNormal();
	UpdateControlKeyName(theDialog);
	FrameDialogItemC(theDialog, 3, kRedOrangeColor8);
}

//--------------------------------------------------------------  ControlFilter

pascal Boolean ControlFilter (DialogPtr dial, EventRecord *event, short *item)
{
	long		wasKeyMap;
	
	switch (event->what)
	{
		case keyDown:
		switch (whichCtrl)
		{
			case 0:
			wasKeyMap = (long)GetKeyMapFromMessage(event->message);
			if ((wasKeyMap == tempLeftMap) || (wasKeyMap == tempBattMap) || 
					(wasKeyMap == tempBandMap) || (wasKeyMap == kTabKeyMap) || 
					(wasKeyMap == kEscKeyMap) || (wasKeyMap == kDeleteKeyMap))
			{
				if (wasKeyMap == kEscKeyMap)
				{
					FlashDialogButton(dial, kCancelButton);
					*item = kCancelButton;
					return(true);			
				}
				else
					SysBeep(1);
			}
			else
			{
				GetKeyName(event->message, tempRightStr);
				tempRightMap = wasKeyMap;
			}
			break;
			
			case 1:
			wasKeyMap = (long)GetKeyMapFromMessage(event->message);
			if ((wasKeyMap == tempRightMap) || (wasKeyMap == tempBattMap) || 
					(wasKeyMap == tempBandMap) || (wasKeyMap == kTabKeyMap) || 
					(wasKeyMap == kEscKeyMap) || (wasKeyMap == kDeleteKeyMap))
			{
				if (wasKeyMap == kEscKeyMap)
				{
					FlashDialogButton(dial, kCancelButton);
					*item = kCancelButton;
					return(true);			
				}
				else
					SysBeep(1);
			}
			else
			{
				GetKeyName(event->message, tempLeftStr);
				tempLeftMap = wasKeyMap;
			}
			break;
			
			case 2:
			wasKeyMap = (long)GetKeyMapFromMessage(event->message);
			if ((wasKeyMap == tempRightMap) || (wasKeyMap == tempLeftMap) || 
					(wasKeyMap == tempBandMap) || (wasKeyMap == kTabKeyMap) || 
					(wasKeyMap == kEscKeyMap) || (wasKeyMap == kDeleteKeyMap))
			{
				if (wasKeyMap == kEscKeyMap)
				{
					FlashDialogButton(dial, kCancelButton);
					*item = kCancelButton;
					return(true);			
				}
				else
					SysBeep(1);
			}
			else
			{
				GetKeyName(event->message, tempBattStr);
				tempBattMap = wasKeyMap;
			}
			break;
			
			case 3:
			wasKeyMap = (long)GetKeyMapFromMessage(event->message);
			if ((wasKeyMap == tempRightMap) || (wasKeyMap == tempLeftMap) || 
					(wasKeyMap == tempBattMap) || (wasKeyMap == kTabKeyMap) || 
					(wasKeyMap == kEscKeyMap) || (wasKeyMap == kDeleteKeyMap))
			{
				if (wasKeyMap == kEscKeyMap)
				{
					FlashDialogButton(dial, kCancelButton);
					*item = kCancelButton;
					return(true);			
				}
				else
					SysBeep(1);
			}
			else
			{
				GetKeyName(event->message, tempBandStr);
				tempBandMap = wasKeyMap;
			}
			break;
		}
		UpdateControlKeyName(dial);
		return(false);
		break;
		
		case mouseDown:
		return(false);
		break;
		
		case updateEvt:
		SetPort((GrafPtr)dial);
		BeginUpdate(GetDialogWindow(dial));
		UpdateSettingsControl(dial);
		EndUpdate(GetDialogWindow(dial));
		event->what = nullEvent;
		return(false);
		break;
		
		default:
		return(false);
		break;
	}
}

//--------------------------------------------------------------  DoControlPrefs

void DoControlPrefs (void)
{
	DialogPtr		prefDlg;
	short			i, itemHit;
	Boolean			leaving;
	ModalFilterUPP	controlFilterUPP;
	
	controlFilterUPP = NewModalFilterUPP(ControlFilter);
	
//	CenterDialog(kControlPrefsDialID);
	prefDlg = GetNewDialog(kControlPrefsDialID, nil, kPutInFront);
	if (prefDlg == nil)
		RedAlert(kErrDialogDidntLoad);
	SetPort((GrafPtr)prefDlg);
	for (i = 0; i < 4; i++)
	{
		GetDialogItemRect(prefDlg, i + kRightControl, &controlRects[i]);
		InsetRect(&controlRects[i], -3, -3);
	}
	whichCtrl = 1;
	
	PasStringCopy(leftName, tempLeftStr);
	PasStringCopy(rightName, tempRightStr);
	PasStringCopy(batteryName, tempBattStr);
	PasStringCopy(bandName, tempBandStr);
	tempLeftMap = theGlider.leftKey;
	tempRightMap = theGlider.rightKey;
	tempBattMap = theGlider.battKey;
	tempBandMap = theGlider.bandKey;
	wasEscPauseKey = isEscPauseKey;
	
	leaving = false;
	
	ShowWindow(GetDialogWindow(prefDlg));
	if (isEscPauseKey)
		SelectFromRadioGroup(prefDlg, kESCPausesRadio, 
				kESCPausesRadio, kTABPausesRadio);
	else
		SelectFromRadioGroup(prefDlg, kTABPausesRadio, 
				kESCPausesRadio, kTABPausesRadio);
	
	while (!leaving)
	{
		ModalDialog(controlFilterUPP, &itemHit);
		switch (itemHit)
		{
			case kOkayButton:
			PasStringCopy(tempLeftStr, leftName);
			PasStringCopy(tempRightStr, rightName);
			PasStringCopy(tempBattStr, batteryName);
			PasStringCopy(tempBandStr, bandName);
			theGlider.leftKey = tempLeftMap;
			theGlider.rightKey = tempRightMap;
			theGlider.battKey = tempBattMap;
			theGlider.bandKey = tempBandMap;
			isEscPauseKey = wasEscPauseKey;
			leaving = true;
			break;
			
			case kCancelButton:
			leaving = true;
			break;
			
			case kRightControl:
			case kLeftControl:
			case kBattControl:
			case kBandControl:
			PenSize(2, 2);
			ForeColor(whiteColor);
			FrameRect(&controlRects[whichCtrl]);
			whichCtrl = itemHit - kRightControl;
			ForeColor(redColor);
			FrameRect(&controlRects[whichCtrl]);
			ForeColor(blackColor);
			PenNormal();
			UpdateControlKeyName(prefDlg);
			break;
			
			case kESCPausesRadio:
			case kTABPausesRadio:
			SelectFromRadioGroup(prefDlg, itemHit, kESCPausesRadio, kTABPausesRadio);
			wasEscPauseKey = !wasEscPauseKey;
			break;
			
			case kControlDefaults:
			SetControlsToDefaults(prefDlg);
			UpdateControlKeyName(prefDlg);
			break;
		}
	}
	
	DisposeDialog(prefDlg);
	DisposeModalFilterUPP(controlFilterUPP);
}

//--------------------------------------------------------------  SoundDefaults

void SoundDefaults (DialogPtr theDialog)
{	
	wasIdle = true;
	wasPlay = true;
	SetDialogItemValue(theDialog, kIdleMusicItem, (short)wasIdle);
	SetDialogItemValue(theDialog, kPlayMusicItem, (short)wasPlay);
	UnivSetSoundVolume(3, thisMac.hasSM3);
	SetDialogNumToStr(theDialog, kVolNumberItem, 3L);
	HandleSoundMusicChange(3, true);
}

//--------------------------------------------------------------  UpdateSettingsSound

void UpdateSettingsSound (DialogPtr theDialog)
{
	short		howLoudNow;
	
	DrawDialog(theDialog);
	DrawDefaultButton(theDialog);
	
	UnivGetSoundVolume(&howLoudNow, thisMac.hasSM3);
	
	if (howLoudNow >= 7)
		SetDialogNumToStr(theDialog, kVolNumberItem, 11L);
	else
		SetDialogNumToStr(theDialog, kVolNumberItem, (long)howLoudNow);
	
	FrameDialogItemC(theDialog, 11, kRedOrangeColor8);
}

//--------------------------------------------------------------  HandleSoundMusicChange

void HandleSoundMusicChange (short newVolume, Boolean sayIt)
{
	OSErr		theErr;
	
	isSoundOn = (newVolume != 0);
	
	if (wasIdle)
	{
		if (newVolume == 0)
			StopTheMusic();
		else
		{
			if (!isMusicOn)
			{
				theErr = StartMusic();
				if (theErr != noErr)
				{
					YellowAlert(kYellowNoMusic, theErr);
					failedMusic = true;
				}
			}
		}
	}
	
	if ((newVolume != 0) && (sayIt))
		PlayPrioritySound(kChord2Sound, kChord2Priority);
}

//--------------------------------------------------------------  SoundFilter

pascal Boolean SoundFilter (DialogPtr dial, EventRecord *event, short *item)
{
	short		newVolume;
	
	switch (event->what)
	{
		case keyDown:
		switch ((event->message) & charCodeMask)
		{
			case kReturnKeyASCII:
			case kEnterKeyASCII:
			FlashDialogButton(dial, kOkayButton);
			*item = kOkayButton;
			return(true);
			break;
			
			case kEscapeKeyASCII:
			FlashDialogButton(dial, kCancelButton);
			*item = kCancelButton;
			return(true);
			break;
			
			case kUpArrowKeyASCII:
			*item = kLouderItem;
			return(true);
			break;
			
			case kDownArrowKeyASCII:
			*item = kSofterItem;
			return(true);
			break;
			
			case k0KeyASCII:
			case k1KeyASCII:
			case k2KeyASCII:
			case k3KeyASCII:
			case k4KeyASCII:
			case k5KeyASCII:
			case k6KeyASCII:
			case k7KeyASCII:
			newVolume = (((event->message) & charCodeMask) - k0KeyASCII);
			if (newVolume == 7L)
				SetDialogNumToStr(dial, kVolNumberItem, 11L);
			else
				SetDialogNumToStr(dial, kVolNumberItem, (long)newVolume);
			
			UnivSetSoundVolume(newVolume, thisMac.hasSM3);
			
			HandleSoundMusicChange(newVolume, true);
			return(false);
			break;
			
			case kCapDKeyASCII:
			case kDKeyASCII:
			*item = kSoundDefault;
			FlashDialogButton(dial, kSoundDefault);
			return(true);
			break;
			
			case kCapGKeyASCII:
			case kGKeyASCII:
			*item = kPlayMusicItem;
			return(true);
			break;
			
			case kCapIKeyASCII:
			case kIKeyASCII:
			*item = kIdleMusicItem;
			return(true);
			break;
			
			default:
			return(false);
		}
		break;
		
		case mouseDown:
		return(false);
		break;
		
		case updateEvt:
		SetPort((GrafPtr)dial);
		BeginUpdate(GetDialogWindow(dial));
		UpdateSettingsSound(dial);
		EndUpdate(GetDialogWindow(dial));
		event->what = nullEvent;
		return(false);
		break;
		
		default:
		return(false);
		break;
	}
}

//--------------------------------------------------------------  DoSettingsMain

void DoSoundPrefs (void)
{
	Rect			tempRect;
	DialogPtr		prefDlg;
	short			wasLoudness, tempVolume;
	OSErr			theErr;
	short			itemHit;
	Boolean			leaving;
	ModalFilterUPP	soundFilterUPP;
	
	soundFilterUPP = NewModalFilterUPP(SoundFilter);
	
	BringUpDialog(&prefDlg, kSoundPrefsDialID);
	
	UnivGetSoundVolume(&wasLoudness, thisMac.hasSM3);
	
	wasIdle = isPlayMusicIdle;
	wasPlay = isPlayMusicGame;
	SetDialogItemValue(prefDlg, kIdleMusicItem, (short)wasIdle);
	SetDialogItemValue(prefDlg, kPlayMusicItem, (short)wasPlay);
	leaving = false;
	
	while (!leaving)
	{
		ModalDialog(soundFilterUPP, &itemHit);
		switch (itemHit)
		{
			case kOkayButton:
			isPlayMusicIdle = wasIdle;
			isPlayMusicGame = wasPlay;
			leaving = true;
			UnivGetSoundVolume(&tempVolume, thisMac.hasSM3);
			isSoundOn = (tempVolume != 0);
			break;
			
			case kCancelButton:
			UnivSetSoundVolume(wasLoudness, thisMac.hasSM3);
			HandleSoundMusicChange(wasLoudness, false);
			if (isPlayMusicIdle != wasIdle)
			{
				if (isPlayMusicIdle)
				{
					if (wasLoudness != 0)
					{
						theErr = StartMusic();
						if (theErr != noErr)
						{
							YellowAlert(kYellowNoMusic, theErr);
							failedMusic = true;
						}
					}
				}
				else
					StopTheMusic();
			}
			leaving = true;
			break;
			
			case kSofterItem:
			UnivGetSoundVolume(&tempVolume, thisMac.hasSM3);	
			if (tempVolume > 0)
			{
				GetDialogItemRect(prefDlg, kSofterItem, &tempRect);
				DrawCIcon(1034, tempRect.left, tempRect.top);
				tempVolume--;
				SetDialogNumToStr(prefDlg, kVolNumberItem, (long)tempVolume);
				UnivSetSoundVolume(tempVolume, thisMac.hasSM3);
				HandleSoundMusicChange(tempVolume, true);
				InvalWindowRect(GetDialogWindow(prefDlg), &tempRect);
				DelayTicks(8);
			}
			break;
			
			case kLouderItem:
			UnivGetSoundVolume(&tempVolume, thisMac.hasSM3);	
			if (tempVolume < 7)
			{
				GetDialogItemRect(prefDlg, kLouderItem, &tempRect);
				DrawCIcon(1033, tempRect.left, tempRect.top);
				tempVolume++;
				if (tempVolume == 7)
					SetDialogNumToStr(prefDlg, kVolNumberItem, 11L);
				else
					SetDialogNumToStr(prefDlg, kVolNumberItem, tempVolume);
				UnivSetSoundVolume(tempVolume, thisMac.hasSM3);
				HandleSoundMusicChange(tempVolume, true);
				InvalWindowRect(GetDialogWindow(prefDlg), &tempRect);
				DelayTicks(8);
			}
			break;
			
			case kIdleMusicItem:
			wasIdle = !wasIdle;
			SetDialogItemValue(prefDlg, kIdleMusicItem, (short)wasIdle);
			if (wasIdle)
			{
				UnivGetSoundVolume(&tempVolume, thisMac.hasSM3);
				if (tempVolume != 0)
				{
					theErr = StartMusic();
					if (theErr != noErr)
					{
						YellowAlert(kYellowNoMusic, theErr);
						failedMusic = true;
					}
				}
			}
			else
				StopTheMusic();
			break;
			
			case kPlayMusicItem:
			wasPlay = !wasPlay;
			SetDialogItemValue(prefDlg, kPlayMusicItem, (short)wasPlay);
			break;
			
			case kSoundDefault:
			SoundDefaults(prefDlg);
			break;
		}
	}
	
	DisposeDialog(prefDlg);
	DisposeModalFilterUPP(soundFilterUPP);
}

//--------------------------------------------------------------  DisplayDefaults

void DisplayDefaults (void)
{
	numNeighbors = 9;
	wasDepthPref = kSwitchIfNeeded;
	wasFade = true;
	wasScreen2 = false;
}

//--------------------------------------------------------------  FrameDisplayIcon

void FrameDisplayIcon (DialogPtr theDialog)
{
	Rect		theRect;
	
	switch (numNeighbors)
	{
		case 1:
		GetDialogItemRect(theDialog, kDisplay1Item, &theRect);
		break;
		
		case 3:
		GetDialogItemRect(theDialog, kDisplay3Item, &theRect);
		break;
		
		default:
		GetDialogItemRect(theDialog, kDisplay9Item, &theRect);
		break;
	}
	
	theRect.left -= 3;
	theRect.top += 0;
	theRect.right += 3;
	theRect.bottom -= 1;
	FrameRect(&theRect);
	InsetRect(&theRect, 1, 1);
	FrameRect(&theRect);
}

//--------------------------------------------------------------  DisplayUpdate

void DisplayUpdate (DialogPtr theDialog)
{
	DrawDialog(theDialog);
	DrawDefaultButton(theDialog);
	
	SetDialogItemValue(theDialog, kDoColorFadeItem, (short)wasFade);
	SelectFromRadioGroup(theDialog, kCurrentDepth + wasDepthPref, 
			kCurrentDepth, k16Depth);
//	SetDialogItemValue(theDialog, kUseQDItem, (short)wasQD);
	SetDialogItemValue(theDialog, kUseScreen2Item, (short)wasScreen2);
	
	ForeColor(redColor);
	FrameDisplayIcon(theDialog);
	ForeColor(blackColor);
	FrameDialogItemC(theDialog, 8, kRedOrangeColor8);
	FrameDialogItemC(theDialog, 13, kRedOrangeColor8);
	FrameDialogItemC(theDialog, 14, kRedOrangeColor8);
}

//--------------------------------------------------------------  DisplayFilter

pascal Boolean DisplayFilter (DialogPtr dial, EventRecord *event, short *item)
{	
	switch (event->what)
	{
		case keyDown:
		switch ((event->message) & charCodeMask)
		{
			case kReturnKeyASCII:
			case kEnterKeyASCII:
			FlashDialogButton(dial, kOkayButton);
			*item = kOkayButton;
			return(true);
			break;
			
			case kEscapeKeyASCII:
			FlashDialogButton(dial, kCancelButton);
			*item = kCancelButton;
			return(true);
			break;
			
			case kLeftArrowKeyASCII:
			switch (numNeighbors)
			{
				case 1:
				*item = kDisplay9Item;
				break;
				
				case 3:
				*item = kDisplay1Item;
				break;
				
				case 9:
				*item = kDisplay3Item;
				break;
			}
			return(true);
			break;
			
			case kRightArrowKeyASCII:
			switch (numNeighbors)
			{
				case 1:
				*item = kDisplay3Item;
				break;
				
				case 3:
				*item = kDisplay9Item;
				break;
				
				case 9:
				*item = kDisplay1Item;
				break;
			}
			return(true);
			break;
			
			case kUpArrowKeyASCII:
			switch (wasDepthPref)
			{
				case kSwitchIfNeeded:
				*item = k16Depth;
				break;
				
				case kSwitchTo256Colors:
				*item = kCurrentDepth;
				break;
				
				case kSwitchTo16Grays:
				*item = k256Depth;
				break;
			}
			return(true);
			break;
			
			case kDownArrowKeyASCII:
			switch (wasDepthPref)
			{
				case kSwitchIfNeeded:
				*item = k256Depth;
				break;
				
				case kSwitchTo256Colors:
				*item = k16Depth;
				break;
				
				case kSwitchTo16Grays:
				*item = kCurrentDepth;
				break;
			}
			return(true);
			break;
			
			case k1KeyASCII:
			*item = kDisplay1Item;
			return(true);
			break;
			
			case k3KeyASCII:
			*item = kDisplay3Item;
			return(true);
			break;
			
			case k9KeyASCII:
			*item = kDisplay9Item;
			return(true);
			break;
			
			case kCapBKeyASCII:
			case kBKeyASCII:
			*item = kDoColorFadeItem;
			return(true);
			break;
			
			case kCapDKeyASCII:
			case kDKeyASCII:
			*item = kDispDefault;
			FlashDialogButton(dial, kDispDefault);
			return(true);
			break;
			
			case kCapRKeyASCII:
			case kRKeyASCII:
			*item = kUseScreen2Item;
			FlashDialogButton(dial, kUseQDItem);
			return(true);
			break;
			
			case kCapUKeyASCII:
			case kUKeyASCII:
			*item = kUseQDItem;
			return(true);
			break;
			
			default:
			return(false);
		}
		break;
		
		case mouseDown:
		return(false);
		break;
		
		case updateEvt:
		SetPort((GrafPtr)dial);
		BeginUpdate(GetDialogWindow(dial));
		DisplayUpdate(dial);
		EndUpdate(GetDialogWindow(dial));
		event->what = nullEvent;
		return(false);
		break;
		
		default:
		return(false);
		break;
	}
}

//--------------------------------------------------------------  DoDisplayPrefs

void DoDisplayPrefs (void)
{
	DialogPtr		prefDlg;
	short			itemHit, wasNeighbors;
	Boolean			leaving;
	ModalFilterUPP	displayFilterUPP;
	
	displayFilterUPP = NewModalFilterUPP(DisplayFilter);
	
	BringUpDialog(&prefDlg, kDisplayPrefsDialID);
	if (!thisMac.can8Bit)
	{
		MyDisableControl(prefDlg, kDoColorFadeItem);
		MyDisableControl(prefDlg, k256Depth);
	}
	if (!thisMac.can4Bit)
		MyDisableControl(prefDlg, k16Depth);
	if (thisMac.numScreens < 2)
		MyDisableControl(prefDlg, kUseScreen2Item);
	wasNeighbors = numNeighbors;
	wasFade = isDoColorFade;
	wasDepthPref = isDepthPref;
	wasScreen2 = isUseSecondScreen;
	leaving = false;
	
	while (!leaving)
	{
		ModalDialog(displayFilterUPP, &itemHit);
		switch (itemHit)
		{
			case kOkayButton:
			isDoColorFade = wasFade;
			isDepthPref = wasDepthPref;
			if (isUseSecondScreen != wasScreen2)
				nextRestartChange = true;
			isUseSecondScreen = wasScreen2;
			leaving = true;
			break;
			
			case kCancelButton:
			numNeighbors = wasNeighbors;
			leaving = true;
			break;
			
			case kDisplay1Item:
			ForeColor(whiteColor);
			FrameDisplayIcon(prefDlg);
			numNeighbors = 1;
			ForeColor(redColor);
			FrameDisplayIcon(prefDlg);
			ForeColor(blackColor);
			break;
			
			case kDisplay3Item:
			if (thisMac.screen.right > 512)
			{
				ForeColor(whiteColor);
				FrameDisplayIcon(prefDlg);
				numNeighbors = 3;
				ForeColor(redColor);
				FrameDisplayIcon(prefDlg);
				ForeColor(blackColor);
			}
			break;
			
			case kDisplay9Item:
			if (thisMac.screen.right > 512)
			{
				ForeColor(whiteColor);
				FrameDisplayIcon(prefDlg);
				numNeighbors = 9;
				ForeColor(redColor);
				FrameDisplayIcon(prefDlg);
				ForeColor(blackColor);
			}
			break;
			
			case kDoColorFadeItem:
			wasFade = !wasFade;
			SetDialogItemValue(prefDlg, kDoColorFadeItem, (short)wasFade);
			break;
			
			case kCurrentDepth:
			case k256Depth:
			case k16Depth:
			wasDepthPref = itemHit - kCurrentDepth;
			SelectFromRadioGroup(prefDlg, itemHit, kCurrentDepth, k16Depth);
			break;
			
			case kDispDefault:
			ForeColor(whiteColor);
			FrameDisplayIcon(prefDlg);
			ForeColor(blackColor);
			DisplayDefaults();
			DisplayUpdate(prefDlg);
			break;
			
			case kUseQDItem:
//			wasQD = !wasQD;
//			SetDialogItemValue(prefDlg, kUseQDItem, (short)wasQD);
			break;
			
			case kUseScreen2Item:
			wasScreen2 = !wasScreen2;
			SetDialogItemValue(prefDlg, kUseScreen2Item, (short)wasScreen2);
			break;
		}
	}
	
	DisposeDialog(prefDlg);
	DisposeModalFilterUPP(displayFilterUPP);
}

//--------------------------------------------------------------  SetAllDefaults

void SetAllDefaults (void)
{
	OSErr		theErr;
								// Default brain settings
	willMaxFiles = 48;
	doZooms = true;
	doAutoDemo = true;
	doBackground = false;
	isHouseChecks = true;
	doPrettyMap = true;
	doBitchDialogs = true;
								// Default control settings
	PasStringCopy("\plf arrow", leftName);
	PasStringCopy("\prt arrow", rightName);
	PasStringCopy("\pdn arrow", batteryName);
	PasStringCopy("\pup arrow", bandName);
	theGlider.leftKey = kLeftArrowKeyMap;
	theGlider.rightKey = kRightArrowKeyMap;
	theGlider.battKey = kDownArrowKeyMap;
	theGlider.bandKey = kUpArrowKeyMap;
	isEscPauseKey = false;
								// Default sound settings
	isPlayMusicIdle = true;
	isPlayMusicGame = true;
	UnivSetSoundVolume(3, thisMac.hasSM3);
	isSoundOn = true;
	if (!isMusicOn)
	{
		theErr = StartMusic();
		if (theErr != noErr)
		{
			YellowAlert(kYellowNoMusic, theErr);
			failedMusic = true;
		}
	}
								// Default display settings
	numNeighbors = 9;
	quickerTransitions = false;
	isDepthPref = kSwitchIfNeeded;
	isDoColorFade = true;
}

//--------------------------------------------------------------  FlashSettingsButton

void FlashSettingsButton (short who)
{
	#define		kNormalSettingsIcon		1010
	#define		kInvertedSettingsIcon	1014
	short		theID;
	
	theID = kInvertedSettingsIcon + who;
	DrawCIcon (theID, prefButton[who].left + 4, prefButton[who].top + 4);
	DelayTicks(8);
	theID = kNormalSettingsIcon + who;
	DrawCIcon (theID, prefButton[who].left + 4, prefButton[who].top + 4);
}

//--------------------------------------------------------------  UpdateSettingsMain

void UpdateSettingsMain (DialogPtr theDialog)
{
	Str255		theStr;
	
	DrawDialog(theDialog);
	
	DrawDefaultButton(theDialog);
	
	GetIndString(theStr, 129, 1);
	DrawDialogUserText(theDialog, 7, theStr, false);
	GetIndString(theStr, 129, 2);
	DrawDialogUserText(theDialog, 8, theStr, false);
	GetIndString(theStr, 129, 3);
	DrawDialogUserText(theDialog, 9, theStr, false);
	GetIndString(theStr, 129, 4);
	DrawDialogUserText(theDialog, 10, theStr, false);
	
	ColorFrameRect(&prefButton[0], kRedOrangeColor8);
	ColorFrameRect(&prefButton[1], kRedOrangeColor8);
	ColorFrameRect(&prefButton[2], kRedOrangeColor8);
	ColorFrameRect(&prefButton[3], kRedOrangeColor8);
}

//--------------------------------------------------------------  PrefsFilter

pascal Boolean PrefsFilter (DialogPtr dial, EventRecord *event, short *item)
{
	Point		testPt;
	short		i;
	Boolean		foundHit;
	
	switch (event->what)
	{
		case keyDown:
		switch ((event->message) & charCodeMask)
		{
			case kReturnKeyASCII:
			case kEnterKeyASCII:
			FlashDialogButton(dial, kOkayButton);
			*item = kOkayButton;
			return(true);
			break;
			
			case kCapBKeyASCII:
			case kBKeyASCII:
			*item = kBrainsButton;
			return(true);
			break;
			
			case kCapCKeyASCII:
			case kCKeyASCII:
			*item = kControlsButton;
			return(true);
			break;
			
			case kCapDKeyASCII:
			case kDKeyASCII:
			*item = kDisplayButton;
			return(true);
			break;
			
			case kCapSKeyASCII:
			case kSKeyASCII:
			*item = kSoundButton;
			return(true);
			break;
			
			default:
			return(false);
		}
		break;
		
		case mouseDown:
		testPt = event->where;
		GlobalToLocal(&testPt);
		foundHit = false;
		for (i = 0; i < 4; i++)
		{
			if (PtInRect(testPt, &prefButton[i]))
			{
				*item = kDisplayButton + i;
				foundHit = true;
			}
		}
		return(foundHit);
		break;
		
		case updateEvt:
		if ((WindowPtr)event->message == (WindowPtr)mainWindow)
		{
			SetPortWindowPort(mainWindow);
			BeginUpdate(mainWindow);
			UpdateMainWindow();
			EndUpdate(mainWindow);
			SetPort((GrafPtr)dial);
		}
		else if ((WindowPtr)event->message == GetDialogWindow(dial))
		{
			SetPortDialogPort(dial);
			BeginUpdate(GetDialogWindow(dial));
			UpdateSettingsMain(dial);
			EndUpdate(GetDialogWindow(dial));
		}
		event->what = nullEvent;
		return(false);
		break;
		
		default:
		return(false);
		break;
	}
}

//--------------------------------------------------------------  DoSettingsMain

void DoSettingsMain (void)
{
	#define			kAllDefaultsButton		11
	DialogPtr		prefDlg;
	short			itemHit;
	Boolean			leaving;
	ModalFilterUPP	prefsFilterUPP;
	
	prefsFilterUPP = NewModalFilterUPP(PrefsFilter);
	
	BringUpDialog(&prefDlg, kMainPrefsDialID);
	
	GetDialogItemRect(prefDlg, kDisplayButton, &prefButton[0]);
	InsetRect(&prefButton[0], -4, -4);
	GetDialogItemRect(prefDlg, 4, &prefButton[1]);
	InsetRect(&prefButton[1], -4, -4);
	GetDialogItemRect(prefDlg, 5, &prefButton[2]);
	InsetRect(&prefButton[2], -4, -4);
	GetDialogItemRect(prefDlg, 6, &prefButton[3]);
	InsetRect(&prefButton[3], -4, -4);
	
	leaving = false;
	nextRestartChange = false;
	
	while (!leaving)
	{
		ModalDialog(prefsFilterUPP, &itemHit);
		switch (itemHit)
		{
			case kOkayButton:
			leaving = true;
			break;
			
			case kDisplayButton:
			FlashSettingsButton(0);
			DoDisplayPrefs();
			SetPort((GrafPtr)prefDlg);
			break;
			
			case kSoundButton:
			FlashSettingsButton(1);
			DoSoundPrefs();
			SetPort((GrafPtr)prefDlg);
			FlushEvents(everyEvent, 0);
			break;
			
			case kControlsButton:
			FlashSettingsButton(2);
			DoControlPrefs();
			SetPort((GrafPtr)prefDlg);
			break;
			
			case kBrainsButton:
			if ((OptionKeyDown()) && (!houseUnlocked))
			{
				houseUnlocked = true;
				changeLockStateOfHouse = true;
				saveHouseLocked = false;
			}
			FlashSettingsButton(3);
			DoBrainsPrefs();
			SetPort((GrafPtr)prefDlg);
			break;
			
			case kAllDefaultsButton:
			SetAllDefaults();
			break;
		}
	}
	
	DisposeDialog(prefDlg);
	DisposeModalFilterUPP(prefsFilterUPP);
	
	if (nextRestartChange)
		BitchAboutChanges();
}

//--------------------------------------------------------------  BitchAboutChanges

void BitchAboutChanges (void)
{
	#define		kChangesEffectAlert	1040
	short		hitWhat;
	
//	CenterAlert(kChangesEffectAlert);
	hitWhat = Alert(kChangesEffectAlert, nil);
}

