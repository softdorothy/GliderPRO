
//============================================================================
//----------------------------------------------------------------------------
//								SelectHouse.c
//----------------------------------------------------------------------------
//============================================================================


#include <Aliases.h>
#include <Resources.h>
#include <Sound.h>
#include <StringCompare.h>
#include "DialogUtils.h"
#include "Externs.h"
#include "Environ.h"
#include "House.h"
#include "RectUtils.h"


#define kLoadHouseDialogID		1000
#define kDispFiles				12
#define kLoadTitlePictItem		3
#define kLoadNameFirstItem		5
#define kLoadNameLastItem		16
#define kLoadIconFirstItem		17
#define kLoadIconLastItem		28
#define kScrollUpItem			29
#define kScrollDownItem			30
#define kLoadTitlePict1			1001
#define kLoadTitlePict8			1002
#define kDefaultHousePict1		1003
#define kDefaultHousePict8		1004
#define kGrayedOutUpArrow		1052
#define kGrayedOutDownArrow		1053
#define kMaxExtraHouses			8


void UpdateLoadDialog (DialogPtr);
void PageUpHouses (DialogPtr);
void PageDownHouses (DialogPtr);
pascal Boolean LoadFilter (DialogPtr, EventRecord *, short *);
void SortHouseList (void);
void DoDirSearch (void);


Rect		loadHouseRects[12];
FSSpecPtr	theHousesSpecs;
FSSpec		extraHouseSpecs[kMaxExtraHouses];
long		lastWhenClick;
Point		lastWhereClick;
short		housesFound, thisHouseIndex, maxFiles, willMaxFiles;
short		housePage, demoHouseIndex, numExtraHouses;
char		fileFirstChar[12];

extern	UInt32			doubleTime;


//==============================================================  Functions
//--------------------------------------------------------------  UpdateLoadWindow

#ifndef COMPILEDEMO
void UpdateLoadDialog (DialogPtr theDialog)
{
	Rect		tempRect, dialogRect, dummyRect;
	short		houseStart, houseStop, i, wasResFile, isResFile, count;
//	char		wasState;
	WindowRef	theWindow;
//	RgnHandle	theRegion;
	
	theWindow = GetDialogWindow(theDialog);
	GetWindowBounds(theWindow, kWindowContentRgn, &dialogRect);
	/*
	wasState = HGetState((Handle)(((DialogPeek)theDialog)->window).port.visRgn);
	HLock((Handle)(((DialogPeek)theDialog)->window).port.visRgn);
	dialogRect = (**((((DialogPeek)theDialog)->window).port.visRgn)).rgnBBox;
	HSetState((Handle)(((DialogPeek)theDialog)->window).port.visRgn, wasState);
	*/
	
	DrawDialog(theDialog);
	ColorFrameWHRect(8, 39, 413, 184, kRedOrangeColor8);	// box around files
	
	houseStart = housePage;
	houseStop = housesFound;
	if ((houseStop - houseStart) > kDispFiles)
		houseStop = houseStart + kDispFiles;
	
	wasResFile = CurResFile();
	count = 0;
	
	for (i = 0; i < 12; i++)
		fileFirstChar[i] = 0x7F;
	
	for (i = houseStart; i < houseStop; i++)
	{
		SpinCursor(1);
		
		GetDialogItemRect(theDialog, kLoadIconFirstItem + i - housePage, 
				&tempRect);
		
		if (SectRect(&dialogRect, &tempRect, &dummyRect))
		{
			isResFile = HOpenResFile(theHousesSpecs[i].vRefNum, 
					theHousesSpecs[i].parID, theHousesSpecs[i].name, fsRdPerm);
			if (isResFile != -1)
			{
				if (Get1Resource('icl8', -16455) != nil)
				{
					LargeIconPlot(&tempRect, -16455);
				}
				else
					LoadDialogPICT(theDialog, kLoadIconFirstItem + i - housePage, 
							kDefaultHousePict8);
				CloseResFile(isResFile);
			}
			else
				LoadDialogPICT(theDialog, kLoadIconFirstItem + i - housePage, 
						kDefaultHousePict8);
		}
		
		fileFirstChar[count] = theHousesSpecs[i].name[1];
		if ((fileFirstChar[count] <= 0x7A) && (fileFirstChar[count] > 0x60))
			fileFirstChar[count] -= 0x20;
		count++;
		
		DrawDialogUserText(theDialog, kLoadNameFirstItem + i - housePage, 
				theHousesSpecs[i].name, i == (thisHouseIndex + housePage));
		
	}
	
	InitCursor();
	UseResFile(wasResFile);
}
#endif

//--------------------------------------------------------------  PageUpHouses

#ifndef COMPILEDEMO
void PageUpHouses (DialogPtr theDial)
{
	Rect		tempRect;
	
	if (housePage < kDispFiles)
	{
		SysBeep(1);
		return;
	}
	
	housePage -= kDispFiles;
	thisHouseIndex = kDispFiles - 1;
	
	ShowDialogItem(theDial, kScrollDownItem);
	if (housePage < kDispFiles)
	{
		GetDialogItemRect(theDial, kScrollUpItem, &tempRect);
		HideDialogItem(theDial, kScrollUpItem);
		DrawCIcon(kGrayedOutUpArrow, tempRect.left, tempRect.top);
	}
	
	QSetRect(&tempRect, 8, 39, 421, 223);
	EraseRect(&tempRect);
	InvalWindowRect(GetDialogWindow(theDial), &tempRect);
}
#endif

//--------------------------------------------------------------  PageDownHouses

#ifndef COMPILEDEMO
void PageDownHouses (DialogPtr theDial)
{
	Rect		tempRect;
	
	if (housePage >= (housesFound - kDispFiles))
	{
		SysBeep(1);
		return;
	}
	
	housePage += kDispFiles;
	thisHouseIndex = 0;
	
	ShowDialogItem(theDial, kScrollUpItem);
	if (housePage >= (housesFound - kDispFiles))
	{
		GetDialogItemRect(theDial, kScrollDownItem, &tempRect);
		HideDialogItem(theDial, kScrollDownItem);
		DrawCIcon(kGrayedOutDownArrow, tempRect.left, tempRect.top);
	}
	
	QSetRect(&tempRect, 8, 39, 421, 223);
	EraseRect(&tempRect);
	InvalWindowRect(GetDialogWindow(theDial), &tempRect);
}
#endif

//--------------------------------------------------------------  LoadFilter
#ifndef COMPILEDEMO

pascal Boolean LoadFilter (DialogPtr dial, EventRecord *event, short *item)
{
	short		screenCount, i, wasIndex;
	char		theChar;
	
	switch (event->what)
	{
		case keyDown:
		theChar = (event->message) & charCodeMask;
		switch (theChar)
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
			
			case kPageUpKeyASCII:
			*item = kScrollUpItem;
			return (true);
			break;
			
			case kPageDownKeyASCII:
			*item = kScrollDownItem;
			return (true);
			break;
			
			case kUpArrowKeyASCII:
			InvalWindowRect(GetDialogWindow(dial), &loadHouseRects[thisHouseIndex]);
			thisHouseIndex -= 4;
			if (thisHouseIndex < 0)
			{
				screenCount = housesFound - housePage;
				if (screenCount > kDispFiles)
					screenCount = kDispFiles;
				
				thisHouseIndex += 4;
				thisHouseIndex = (((screenCount - 1) / 4) * 4) + 
						(thisHouseIndex % 4);
				if (thisHouseIndex >= screenCount)
					thisHouseIndex -= 4;
			}
			InvalWindowRect(GetDialogWindow(dial), &loadHouseRects[thisHouseIndex]);
			return(true);
			break;
			
			case kDownArrowKeyASCII:
			InvalWindowRect(GetDialogWindow(dial), &loadHouseRects[thisHouseIndex]);
			thisHouseIndex += 4;
			screenCount = housesFound - housePage;
			if (screenCount > kDispFiles)
				screenCount = kDispFiles;
			if (thisHouseIndex >= screenCount)
				thisHouseIndex %= 4;
			InvalWindowRect(GetDialogWindow(dial), &loadHouseRects[thisHouseIndex]);
			return(true);
			break;
			
			case kLeftArrowKeyASCII:
			InvalWindowRect(GetDialogWindow(dial), &loadHouseRects[thisHouseIndex]);
			thisHouseIndex--;
			if (thisHouseIndex < 0)
			{
				screenCount = housesFound - housePage;
				if (screenCount > kDispFiles)
					screenCount = kDispFiles;
				thisHouseIndex = screenCount - 1;
			}
			InvalWindowRect(GetDialogWindow(dial), &loadHouseRects[thisHouseIndex]);
			return(true);
			break;
			
			case kTabKeyASCII:
			case kRightArrowKeyASCII:
			InvalWindowRect(GetDialogWindow(dial), &loadHouseRects[thisHouseIndex]);
			thisHouseIndex++;
			screenCount = housesFound - housePage;
			if (screenCount > kDispFiles)
				screenCount = kDispFiles;
			if (thisHouseIndex >= screenCount)
				thisHouseIndex = 0;
			InvalWindowRect(GetDialogWindow(dial), &loadHouseRects[thisHouseIndex]);
			return(true);
			break;
			
			default:
			if (((theChar > 0x40) && (theChar <= 0x5A)) || 
					((theChar > 0x60) && (theChar <= 0x7A)))
			{
				if ((theChar > 0x60) && (theChar <= 0x7A))
					theChar -= 0x20;
				wasIndex = thisHouseIndex;
				thisHouseIndex = -1;
				i = 0;
				do
				{
					if ((fileFirstChar[i] >= theChar) && (fileFirstChar[i] != 0x7F))
						thisHouseIndex = i;
					i++;
				}
				while ((thisHouseIndex == -1) && (i < 12));
				if (thisHouseIndex == -1)
				{
					screenCount = housesFound - housePage;
					if (screenCount > kDispFiles)
						screenCount = kDispFiles;
					thisHouseIndex = screenCount - 1;
				}
				if (wasIndex != thisHouseIndex)
				{
					InvalWindowRect(GetDialogWindow(dial), &loadHouseRects[wasIndex]);
					InvalWindowRect(GetDialogWindow(dial), &loadHouseRects[thisHouseIndex]);
				}
				return(true);
			}
			else
				return(false);
		}
		break;
		
		case mouseDown:
		lastWhenClick = event->when - lastWhenClick;
		SubPt(event->where, &lastWhereClick);
		return(false);
		break;
		
		case mouseUp:
		lastWhenClick = event->when;
		lastWhereClick = event->where;
		return(false);
		break;
		
		case updateEvt:
		BeginUpdate(GetDialogWindow(dial));
		UpdateLoadDialog(dial);
		EndUpdate(GetDialogWindow(dial));
		event->what = nullEvent;
		return(false);
		break;
		
		default:
		return(false);
		break;
	}
}
#endif

//--------------------------------------------------------------  DoLoadHouse

#ifndef COMPILEDEMO
void DoLoadHouse (void)
{
	Rect			tempRect;
	DialogPtr		theDial;
	short			i, item, wasIndex, screenCount;
	Boolean			leaving, whoCares;
	ModalFilterUPP	loadFilterUPP;
	
	loadFilterUPP = NewModalFilterUPP(LoadFilter);
	
	BringUpDialog(&theDial, kLoadHouseDialogID);
	if (housesFound <= kDispFiles)
	{
		GetDialogItemRect(theDial, kScrollUpItem, &tempRect);
		HideDialogItem(theDial, kScrollUpItem);
		DrawCIcon(kGrayedOutUpArrow, tempRect.left, tempRect.top);
		
		GetDialogItemRect(theDial, kScrollDownItem, &tempRect);
		HideDialogItem(theDial, kScrollDownItem);
		DrawCIcon(kGrayedOutDownArrow, tempRect.left, tempRect.top);
	}
	else
	{
		if (thisHouseIndex < kDispFiles)
		{
			GetDialogItemRect(theDial, kScrollUpItem, &tempRect);
			HideDialogItem(theDial, kScrollUpItem);
			DrawCIcon(kGrayedOutUpArrow, tempRect.left, tempRect.top);
		}
		else if (thisHouseIndex > (housesFound - kDispFiles))
		{				
			GetDialogItemRect(theDial, kScrollDownItem, &tempRect);
			HideDialogItem(theDial, kScrollDownItem);
			DrawCIcon(kGrayedOutDownArrow, tempRect.left, tempRect.top);
		}
	}
	wasIndex = thisHouseIndex;
	housePage = (thisHouseIndex / kDispFiles) * kDispFiles;
	thisHouseIndex -= housePage;
	
	for (i = 0; i < 12; i++)
	{
		GetDialogItemRect(theDial, kLoadNameFirstItem + i, 
				&loadHouseRects[i]);
		GetDialogItemRect(theDial, kLoadIconFirstItem + i, 
				&tempRect);
		loadHouseRects[i].top = tempRect.top;
		loadHouseRects[i].bottom++;
	}
	
	leaving = false;
	
	while (!leaving)
	{
		ModalDialog(loadFilterUPP, &item);
		
		if (item == kOkayButton)
		{
			thisHouseIndex += housePage;
			if (thisHouseIndex != wasIndex)
			{
				whoCares = CloseHouse();
				PasStringCopy(theHousesSpecs[thisHouseIndex].name, 
						thisHouseName);
				if (OpenHouse())
					whoCares = ReadHouse();
			}
			leaving = true;
		}
		else if (item == kCancelButton)
		{
			thisHouseIndex = wasIndex;
			leaving = true;
		}
		else if ((item >= kLoadNameFirstItem) && 
				(item <= kLoadNameLastItem))
		{
			screenCount = housesFound - housePage;
			if (screenCount > kDispFiles)
				screenCount = kDispFiles;
			if ((item - kLoadNameFirstItem != thisHouseIndex) && 
					(item - kLoadNameFirstItem < screenCount))
			{
				InvalWindowRect(GetDialogWindow(theDial), &loadHouseRects[thisHouseIndex]);
				thisHouseIndex = item - kLoadNameFirstItem;
				InvalWindowRect(GetDialogWindow(theDial), &loadHouseRects[thisHouseIndex]);
			}
						
			if (lastWhereClick.h < 0)
				lastWhereClick.h = -lastWhereClick.h;
			if (lastWhereClick.v < 0)
				lastWhereClick.v = -lastWhereClick.v;
			if ((lastWhenClick < doubleTime) && (lastWhereClick.h < 5) && 
					(lastWhereClick.v < 5))
			{
				thisHouseIndex += housePage;
				if (thisHouseIndex != wasIndex)
				{
					MyDisableControl(theDial, kOkayButton);
					MyDisableControl(theDial, kCancelButton);
					whoCares = CloseHouse();
					PasStringCopy(theHousesSpecs[thisHouseIndex].name, 
							thisHouseName);
					if (OpenHouse())
						whoCares = ReadHouse();
				}
				leaving = true;
			}
		}
		else if ((item >= kLoadIconFirstItem) && 
				(item <= kLoadIconLastItem))
		{
			screenCount = housesFound - housePage;
			if (screenCount > kDispFiles)
				screenCount = kDispFiles;
			if ((item - kLoadIconFirstItem != thisHouseIndex) && 
					(item - kLoadIconFirstItem < screenCount))
			{
				InvalWindowRect(GetDialogWindow(theDial), &loadHouseRects[thisHouseIndex]);
				thisHouseIndex = item - kLoadIconFirstItem;
				InvalWindowRect(GetDialogWindow(theDial), &loadHouseRects[thisHouseIndex]);
			}
			
			if (lastWhereClick.h < 0)
				lastWhereClick.h = -lastWhereClick.h;
			if (lastWhereClick.v < 0)
				lastWhereClick.v = -lastWhereClick.v;
			if ((lastWhenClick < doubleTime) && (lastWhereClick.h < 5) && 
					(lastWhereClick.v < 5))
			{
				thisHouseIndex += housePage;
				if (thisHouseIndex != wasIndex)
				{
					MyDisableControl(theDial, kOkayButton);
					MyDisableControl(theDial, kCancelButton);
					whoCares = CloseHouse();
					PasStringCopy(theHousesSpecs[thisHouseIndex].name, 
							thisHouseName);
					if (OpenHouse())
						whoCares = ReadHouse();
				}
				leaving = true;
			}
		}
		else if (item == kScrollUpItem)
		{
			PageUpHouses(theDial);
		}
		else if (item == kScrollDownItem)
		{
			PageDownHouses(theDial);
		}
	}
	
	DisposeDialog(theDial);
	DisposeModalFilterUPP(loadFilterUPP);
}
#endif

//--------------------------------------------------------------  SortHouseList

void SortHouseList (void)
{
	FSSpec		tempSpec;
	short		i, h, whosFirst;
	
	i = 0;			// remove exact duplicate houses
	while (i < housesFound)
	{
		h = i + 1;
		while (h < housesFound)
		{
			if ((EqualString(theHousesSpecs[i].name, theHousesSpecs[h].name, true, true)) && 
					(theHousesSpecs[i].vRefNum == theHousesSpecs[i].vRefNum) && 
					(theHousesSpecs[i].parID == theHousesSpecs[i].parID))
			{
				theHousesSpecs[h] = theHousesSpecs[housesFound - 1];
				housesFound--;
			}
			h++;
		}
		i++;
	}
	
	for (i = 0; i < housesFound - 1; i++)
	{
		for (h = 0; h < (housesFound - i - 1); h++)
		{
			whosFirst = WhichStringFirst(theHousesSpecs[h].name, 
					theHousesSpecs[h + 1].name);
			if (whosFirst == 1)
			{
				tempSpec = theHousesSpecs[h + 1];
				theHousesSpecs[h + 1] = theHousesSpecs[h];
				theHousesSpecs[h] = tempSpec;
			}
		}
	}
}

//--------------------------------------------------------------  DoDirSearch

void DoDirSearch (void)
{
	#define		kMaxDirectories		32
	CInfoPBRec	theBlock;
	Str255		nameString;
	long		theDirs[kMaxDirectories];
	OSErr		theErr, notherErr;
	short		count, i, currentDir, numDirs;
	
	for (i = 0; i < kMaxDirectories; i++)
		theDirs[i] = 0L;
	currentDir = 0;
	theDirs[currentDir] = thisMac.dirID;
	numDirs = 1;
	
	theBlock.hFileInfo.ioCompletion = nil;
	theBlock.hFileInfo.ioVRefNum = thisMac.vRefNum;
	theBlock.hFileInfo.ioNamePtr = nameString;
	
	while ((currentDir < numDirs) && (currentDir < kMaxDirectories))
	{
		count = 1;
		theErr = noErr;
		
		while (theErr == noErr)
		{
			SpinCursor(1);
			theBlock.hFileInfo.ioFDirIndex = count;
			theBlock.hFileInfo.ioDirID = theDirs[currentDir];
			theErr = PBGetCatInfo(&theBlock, false);
			
			if (theErr == noErr)
			{
				if ((theBlock.hFileInfo.ioFlAttrib & 0x10) == 0x00)
				{
					if ((theBlock.hFileInfo.ioFlFndrInfo.fdType == 'gliH') && 
							(theBlock.hFileInfo.ioFlFndrInfo.fdCreator == 'ozm5') && 
							(housesFound < maxFiles))
					{
						notherErr = FSMakeFSSpec(thisMac.vRefNum, 
								theBlock.hFileInfo.ioFlParID, nameString, 
								&theHousesSpecs[housesFound]);
						if (notherErr == noErr)
							housesFound++;
					}
				}
				else if ((theBlock.hFileInfo.ioFlAttrib & 0x10) == 0x10)
				{
					if (numDirs < kMaxDirectories)
					{
						theDirs[numDirs] = theBlock.hFileInfo.ioDirID;
						numDirs++;
					}
				}
				count++;
			}
		}
		currentDir++;
	}
	
	if (housesFound < 1)
	{
		thisHouseIndex = -1;
		YellowAlert(kYellowNoHouses, 0);
	}
	else
	{
		SortHouseList();
		thisHouseIndex = 0;
		for (i = 0; i < housesFound; i++)
		{
			if (EqualString(theHousesSpecs[i].name, thisHouseName, false, true))
			{
				thisHouseIndex = i;
				break;
			}
		}
		PasStringCopy(theHousesSpecs[thisHouseIndex].name, thisHouseName);
		
		demoHouseIndex = -1;
		for (i = 0; i < housesFound; i++)
		{
			if (EqualString(theHousesSpecs[i].name, "\pDemo House", false, true))
			{
				demoHouseIndex = i;
				break;
			}
		}
	}
}

//--------------------------------------------------------------  BuildHouseList

void BuildHouseList (void)
{
	short		i;
	
	if (thisMac.hasSystem7)
	{
		housesFound = 0;						// zero the number of houses found
		for (i = 0; i < numExtraHouses; i++)	// 1st, insert extra houses into list
		{
			theHousesSpecs[housesFound] = extraHouseSpecs[i];
			housesFound++;
		}
		DoDirSearch();							// now, search folders for the rest
	}
}

//--------------------------------------------------------------  AddExtraHouse

void AddExtraHouse (FSSpec *newHouse)
{
	if (numExtraHouses >= kMaxExtraHouses)
		return;
	
	extraHouseSpecs[numExtraHouses] = *newHouse;
	numExtraHouses++;
}

