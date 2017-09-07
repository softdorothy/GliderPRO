
//============================================================================
//----------------------------------------------------------------------------
//									HouseIO.c
//----------------------------------------------------------------------------
//============================================================================


#include <Aliases.h>
#include <Movies.h>
#include <Resources.h>
#include <StringCompare.h>
#include <TextUtils.h>
#include "Externs.h"
#include "Environ.h"
#include "House.h"
#include "ObjectEdit.h"


#define kSaveChangesAlert		1002
#define kSaveChanges			1
#define kDiscardChanges			2


void LoopMovie (void);
void OpenHouseMovie (void);
void CloseHouseMovie (void);
Boolean IsFileReadOnly (FSSpec *);


Movie		theMovie;
Rect		movieRect;
short		houseRefNum, houseResFork, wasHouseVersion;
Boolean		houseOpen, fileDirty, gameDirty;
Boolean		changeLockStateOfHouse, saveHouseLocked, houseIsReadOnly;
Boolean		hasMovie, tvInRoom;

extern	FSSpecPtr	theHousesSpecs;
extern	short		thisHouseIndex, tvWithMovieNumber;
extern	short		numberRooms, housesFound;
extern	Boolean		noRoomAtAll, quitting, wardBitSet;
extern	Boolean		phoneBitSet, bannerStarCountOn;


//==============================================================  Functions
//--------------------------------------------------------------  LoopMovie

void LoopMovie (void)
{
	Handle		theLoop;
	UserData	theUserData;
	short		theCount;
	
	theLoop = NewHandle(sizeof(long));
	(** (long **) theLoop) = 0;
	theUserData = GetMovieUserData(theMovie);
	theCount = CountUserDataType(theUserData, 'LOOP');
	while (theCount--)
	{
		RemoveUserData(theUserData, 'LOOP', 1);
	}
	AddUserData(theUserData, theLoop, 'LOOP');
}

//--------------------------------------------------------------  OpenHouseMovie

void OpenHouseMovie (void)
{
#ifdef COMPILEQT
	TimeBase	theTime;
	FSSpec		theSpec;
	FInfo		finderInfo;
	Handle		spaceSaver;
	OSErr		theErr;
	short		movieRefNum;
	Boolean		dataRefWasChanged;
	
	if (thisMac.hasQT)
	{
		theSpec = theHousesSpecs[thisHouseIndex];
		PasStringConcat(theSpec.name, "\p.mov");
		
		theErr = FSpGetFInfo(&theSpec, &finderInfo);
		if (theErr != noErr)
			return;
		
		theErr = OpenMovieFile(&theSpec, &movieRefNum, fsCurPerm);
		if (theErr != noErr)
		{
			YellowAlert(kYellowQTMovieNotLoaded, theErr);
			return;
		}
		
		theErr = NewMovieFromFile(&theMovie, movieRefNum, nil, theSpec.name, 
				newMovieActive, &dataRefWasChanged);
		if (theErr != noErr)
		{
			YellowAlert(kYellowQTMovieNotLoaded, theErr);
			theErr = CloseMovieFile(movieRefNum);
			return;
		}
		theErr = CloseMovieFile(movieRefNum);
		
		spaceSaver = NewHandle(307200L);
		if (spaceSaver == nil)
		{
			YellowAlert(kYellowQTMovieNotLoaded, 749);
			CloseHouseMovie();
			return;
		}
		
		GoToBeginningOfMovie(theMovie);
		theErr = LoadMovieIntoRam(theMovie, 
				GetMovieTime(theMovie, 0L), GetMovieDuration(theMovie), 0);
		if (theErr != noErr)
		{
			YellowAlert(kYellowQTMovieNotLoaded, theErr);
			DisposeHandle(spaceSaver);
			CloseHouseMovie();
			return;
		}
		DisposeHandle(spaceSaver);
				
		theErr = PrerollMovie(theMovie, 0, 0x000F0000);
		if (theErr != noErr)
		{
			YellowAlert(kYellowQTMovieNotLoaded, theErr);
			CloseHouseMovie();
			return;
		}
		
		theTime = GetMovieTimeBase(theMovie);
		SetTimeBaseFlags(theTime, loopTimeBase);
		SetMovieMasterTimeBase(theMovie, theTime, nil);
		LoopMovie();
		
		GetMovieBox(theMovie, &movieRect);
		
		hasMovie = true;
	}
#endif
}

//--------------------------------------------------------------  CloseHouseMovie

void CloseHouseMovie (void)
{
#ifdef COMPILEQT
	OSErr		theErr;
	
	if ((thisMac.hasQT) && (hasMovie))
	{
		theErr = LoadMovieIntoRam(theMovie, 
				GetMovieTime(theMovie, 0L), GetMovieDuration(theMovie), flushFromRam);
		DisposeMovie(theMovie);
	}
#endif
	hasMovie = false;
}

//--------------------------------------------------------------  OpenHouse
// Opens a house (whatever current selection is).  Returns true if all went well.

Boolean OpenHouse (void)
{
	OSErr		theErr;
	Boolean		targetIsFolder, wasAliased;
	
	if (houseOpen)
	{
		if (!CloseHouse())
			return(false);
	}
	if ((housesFound < 1) || (thisHouseIndex == -1))
		return(false);
	
	theErr = ResolveAliasFile(&theHousesSpecs[thisHouseIndex], true, 
			&targetIsFolder, &wasAliased);
	if (!CheckFileError(theErr, thisHouseName))
		return (false);
	
	#ifdef COMPILEDEMO
	if (!EqualString(theHousesSpecs[thisHouseIndex].name, "\pDemo House", false, true))
		return (false);
	#endif
	
	houseIsReadOnly = IsFileReadOnly(&theHousesSpecs[thisHouseIndex]);
	
	theErr = FSpOpenDF(&theHousesSpecs[thisHouseIndex], fsCurPerm, &houseRefNum);
	if (!CheckFileError(theErr, thisHouseName))
		return (false);
	
	houseOpen = true;
	OpenHouseResFork();
	
	hasMovie = false;
	tvInRoom = false;
	tvWithMovieNumber = -1;
	OpenHouseMovie();
	
	return (true);
}

//--------------------------------------------------------------  OpenSpecificHouse
// Opens the specific house passed in.

#ifndef COMPILEDEMO
Boolean OpenSpecificHouse (FSSpec *specs)
{
	short		i;
	Boolean		itOpened;
	
	if ((housesFound < 1) || (thisHouseIndex == -1))
		return (false);
	
	itOpened = true;
	
	for (i = 0; i < housesFound; i++)
	{
		if ((theHousesSpecs[i].vRefNum == specs->vRefNum) && 
				(theHousesSpecs[i].parID == specs->parID) && 
				(EqualString(theHousesSpecs[i].name, specs->name, false, true)))
		{
			thisHouseIndex = i;
			PasStringCopy(theHousesSpecs[thisHouseIndex].name, thisHouseName);
			if (OpenHouse())
				itOpened = ReadHouse();
			else
				itOpened = false;
			break;
		}
	}
	
	return (itOpened);
}
#endif

//--------------------------------------------------------------  SaveHouseAs

#ifndef COMPILEDEMO
Boolean SaveHouseAs (void)
{
	// TEMP - fix this later -- use NavServices (see House.c)
/*
	StandardFileReply	theReply;
	FSSpec				oldHouse;
	OSErr				theErr;
	Boolean				noProblems;
	Str255				tempStr;
	
	noProblems = true;
	
	GetLocalizedString(15, tempStr);
	StandardPutFile(tempStr, thisHouseName, &theReply);
	if (theReply.sfGood)
	{
		oldHouse = theHousesSpecs[thisHouseIndex];
			
		CloseHouseResFork();						// close this house file
		theErr = FSClose(houseRefNum);
		if (theErr != noErr)
		{
			CheckFileError(theErr, "\pPreferences");
			return(false);
		}
													// create new house file
		theErr = FSpCreate(&theReply.sfFile, 'ozm5', 'gliH', theReply.sfScript);
		if (!CheckFileError(theErr, theReply.sfFile.name))
			return (false);
		HCreateResFile(theReply.sfFile.vRefNum, theReply.sfFile.parID, 
				theReply.sfFile.name);
		if (ResError() != noErr)
			YellowAlert(kYellowFailedResCreate, ResError());
		PasStringCopy(theReply.sfFile.name, thisHouseName);
													// open new house data fork
		theErr = FSpOpenDF(&theReply.sfFile, fsRdWrPerm, &houseRefNum);
		if (!CheckFileError(theErr, thisHouseName))
			return (false);
		
		houseOpen = true;
		
		noProblems = WriteHouse(false);				// write out house data
		if (!noProblems)
			return(false);
		
		BuildHouseList();
		if (OpenSpecificHouse(&theReply.sfFile))	// open new house again
		{
		}
		else
		{
			if (OpenSpecificHouse(&oldHouse))
			{
				YellowAlert(kYellowOpenedOldHouse, 0);
			}
			else
			{
				YellowAlert(kYellowLostAllHouses, 0);
				noProblems = false;
			}
		}
	}
	
	
	return (noProblems);
	*/
	return false;
}
#endif

//--------------------------------------------------------------  ReadHouse
// With a house open, this function reads in the actual bits of data…
// into memory.

Boolean ReadHouse (void)
{
	long		byteCount;
	OSErr		theErr;
	short		whichRoom;
	
	if (!houseOpen)
	{
		YellowAlert(kYellowUnaccounted, 2);
		return (false);
	}
	
	if (gameDirty || fileDirty)
	{
		if (houseIsReadOnly)
		{
			if (!WriteScoresToDisk())
			{
				YellowAlert(kYellowFailedWrite, 0);
				return(false);
			}
		}
		else if (!WriteHouse(false))
			return(false);
	}
	
	theErr = GetEOF(houseRefNum, &byteCount);
	if (theErr != noErr)
	{
		CheckFileError(theErr, thisHouseName);
		return(false);
	}
	
	#ifdef COMPILEDEMO
	if (byteCount != 16526L)
		return (false);
	#endif
	
	if (thisHouse != nil)
		DisposeHandle((Handle)thisHouse);
	
	thisHouse = (houseHand)NewHandle(byteCount);
	if (thisHouse == nil)
	{
		YellowAlert(kYellowNoMemory, 10);
		return(false);
	}
	MoveHHi((Handle)thisHouse);
	
	theErr = SetFPos(houseRefNum, fsFromStart, 0L);
	if (theErr != noErr)
	{
		CheckFileError(theErr, thisHouseName);
		return(false);
	}
	
	HLock((Handle)thisHouse);
	theErr = FSRead(houseRefNum, &byteCount, *thisHouse);
	if (theErr != noErr)
	{
		CheckFileError(theErr, thisHouseName);
		HUnlock((Handle)thisHouse);
		return(false);
	}
	
	numberRooms = (*thisHouse)->nRooms;
	#ifdef COMPILEDEMO
	if (numberRooms != 45)
		return (false);
	#endif
	if ((numberRooms < 1) || (byteCount == 0L))
	{
		numberRooms = 0;
		noRoomAtAll = true;
		YellowAlert(kYellowNoRooms, 0);
		HUnlock((Handle)thisHouse);
		return(false);
	}
	
	wasHouseVersion = (*thisHouse)->version;
	if (wasHouseVersion >= kNewHouseVersion)
	{
		YellowAlert(kYellowNewerVersion, 0);
		HUnlock((Handle)thisHouse);
		return(false);
	}
	
	houseUnlocked = (((*thisHouse)->timeStamp & 0x00000001) == 0);
	#ifdef COMPILEDEMO
	if (houseUnlocked)
		return (false);
	#endif
	changeLockStateOfHouse = false;
	saveHouseLocked = false;
	
	whichRoom = (*thisHouse)->firstRoom;
	#ifdef COMPILEDEMO
	if (whichRoom != 0)
		return (false);
	#endif
	
	wardBitSet = (((*thisHouse)->flags & 0x00000001) == 0x00000001);
	phoneBitSet = (((*thisHouse)->flags & 0x00000002) == 0x00000002);
	bannerStarCountOn = (((*thisHouse)->flags & 0x00000004) == 0x00000000);
	
	HUnlock((Handle)thisHouse);
	
	noRoomAtAll = (RealRoomNumberCount() == 0);
	thisRoomNumber = -1;
	previousRoom = -1;
	if (!noRoomAtAll)
		CopyRoomToThisRoom(whichRoom);
	
	if (houseIsReadOnly)
	{
		houseUnlocked = false;
		if (ReadScoresFromDisk())
		{
		}
	}
	
	objActive = kNoObjectSelected;
	ReflectCurrentRoom(true);
	gameDirty = false;
	fileDirty = false;
	UpdateMenus(false);
	
	return (true);
}

//--------------------------------------------------------------  WriteHouse
// This function writes out the house data to disk.

Boolean WriteHouse (Boolean checkIt)
{
	UInt32			timeStamp;
	long			byteCount;
	OSErr			theErr;
	
	if (!houseOpen)
	{
		YellowAlert(kYellowUnaccounted, 4);
		return (false);
	}
	
	theErr = SetFPos(houseRefNum, fsFromStart, 0L);
	if (theErr != noErr)
	{
		CheckFileError(theErr, thisHouseName);
		return(false);
	}
	
	CopyThisRoomToRoom();
	
	if (checkIt)
		CheckHouseForProblems();
	
	HLock((Handle)thisHouse);
	byteCount = GetHandleSize((Handle)thisHouse);
	
	if (fileDirty)
	{
		GetDateTime(&timeStamp);
		timeStamp &= 0x7FFFFFFF;
		
		if (changeLockStateOfHouse)
			houseUnlocked = !saveHouseLocked;
		
		if (houseUnlocked)								// house unlocked
			timeStamp &= 0x7FFFFFFE;
		else
			timeStamp |= 0x00000001;
		(*thisHouse)->timeStamp = (long)timeStamp;
		(*thisHouse)->version = wasHouseVersion;
	}
	
	theErr = FSWrite(houseRefNum, &byteCount, *thisHouse);
	if (theErr != noErr)
	{
		CheckFileError(theErr, thisHouseName);
		HUnlock((Handle)thisHouse);
		return(false);
	}
	
	theErr = SetEOF(houseRefNum, byteCount);
	if (theErr != noErr)
	{
		CheckFileError(theErr, thisHouseName);
		HUnlock((Handle)thisHouse);
		return(false);
	}
	
	HUnlock((Handle)thisHouse);
	
	if (changeLockStateOfHouse)
	{
		changeLockStateOfHouse = false;
		ReflectCurrentRoom(true);
	}
	
	gameDirty = false;
	fileDirty = false;
	UpdateMenus(false);
	return (true);
}

//--------------------------------------------------------------  CloseHouse
// This function closes the current house that is open.

Boolean CloseHouse (void)
{
	OSErr		theErr;
	
	if (!houseOpen)
		return (true);
	
	if (gameDirty)
	{
		if (houseIsReadOnly)
		{
			if (!WriteScoresToDisk())
				YellowAlert(kYellowFailedWrite, 0);
		}
		else if (!WriteHouse(theMode == kEditMode))
			YellowAlert(kYellowFailedWrite, 0);
	}
	else if (fileDirty)
	{
#ifndef COMPILEDEMO
		if (!QuerySaveChanges())	// false signifies user canceled
			return(false);
#endif
	}
	
	CloseHouseResFork();
	CloseHouseMovie();
	
	theErr = FSClose(houseRefNum);
	if (theErr != noErr)
	{
		CheckFileError(theErr, thisHouseName);
		return(false);
	}
	
	houseOpen = false;
	
	return (true);
}

//--------------------------------------------------------------  OpenHouseResFork
// Opens the resource fork of the current house that is open.

void OpenHouseResFork (void)
{
	if (houseResFork == -1)
	{
		houseResFork = FSpOpenResFile(&theHousesSpecs[thisHouseIndex], fsCurPerm);
		if (houseResFork == -1)
			YellowAlert(kYellowFailedResOpen, ResError());
		else
			UseResFile(houseResFork);
	}
}

//--------------------------------------------------------------  CloseHouseResFork
// Closes the resource fork of the current house that is open.

void CloseHouseResFork (void)
{
	if (houseResFork != -1)
	{
		CloseResFile(houseResFork);
		houseResFork = -1;
	}
}

//--------------------------------------------------------------  QuerySaveChanges
// If changes were made, this function will present the user with a…
// dialog asking them if they would like to save the changes.

#ifndef COMPILEDEMO
Boolean QuerySaveChanges (void)
{
	short		hitWhat;
	Boolean		whoCares;
	
	if (!fileDirty)
		return(true);
	
	InitCursor();
//	CenterAlert(kSaveChangesAlert);
	ParamText(thisHouseName, "\p", "\p", "\p");
	hitWhat = Alert(kSaveChangesAlert, nil);
	if (hitWhat == kSaveChanges)
	{
		if (wasHouseVersion < kHouseVersion)
			ConvertHouseVer1To2();
		wasHouseVersion = kHouseVersion;
		if (WriteHouse(true))
			return (true);
		else
			return (false);
	}
	else if (hitWhat == kDiscardChanges)
	{
		fileDirty = false;
		if (!quitting)
		{
			whoCares = CloseHouse();
			if (OpenHouse())
				whoCares = ReadHouse();
		}
		UpdateMenus(false);
		return (true);
	}
	else
		return (false);
}
#endif

//--------------------------------------------------------------  YellowAlert
// This is a dialog used to present an error code and explanation…
// to the user when a non-lethal error has occurred.  Ideally, of…
// course, this never is called.

void YellowAlert (short whichAlert, short identifier)
{
	#define		kYellowAlert	1006
	Str255		errStr, errNumStr;
	short		whoCares;
	
	InitCursor();
	
	GetIndString(errStr, kYellowAlert, whichAlert);
	NumToString((long)identifier, errNumStr);
	
//	CenterAlert(kYellowAlert);
	ParamText(errStr, errNumStr, "\p", "\p");
	
	whoCares = Alert(kYellowAlert, nil);
}

//--------------------------------------------------------------  IsFileReadOnly

Boolean IsFileReadOnly (FSSpec *theSpec)
{
#pragma unused (theSpec)
	
	return false;
	/*
	Str255			tempStr;
	ParamBlockRec	theBlock;
	HParamBlockRec	hBlock;
	VolumeParam		*volPtr;
	OSErr			theErr;
	
	volPtr = (VolumeParam *)&theBlock;
	volPtr->ioCompletion = nil;
	volPtr->ioVolIndex = 0;
	volPtr->ioNamePtr = tempStr;
	volPtr->ioVRefNum = theSpec->vRefNum;
	
	theErr = PBGetVInfo(&theBlock, false);
	if (CheckFileError(theErr, "\pRead/Write"))
	{
		if (((volPtr->ioVAtrb & 0x0080) == 0x0080) || 
				((volPtr->ioVAtrb & 0x8000) == 0x8000))
			return (true);		// soft/hard locked bits
		else
		{
			hBlock.fileParam.ioCompletion = nil;
			hBlock.fileParam.ioVRefNum = theSpec->vRefNum;
			hBlock.fileParam.ioFVersNum = 0;
			hBlock.fileParam.ioFDirIndex = 0;
			hBlock.fileParam.ioNamePtr = theSpec->name;
			hBlock.fileParam.ioDirID = theSpec->parID;
			
			theErr = PBHGetFInfo(&hBlock, false);
			if (CheckFileError(theErr, "\pRead/Write"))
			{
				if ((hBlock.fileParam.ioFlAttrib & 0x0001) == 0x0001)
					return (true);
				else
					return (false);
			}
			else
				return (false);
		}
	}
	else
		return (false);
	*/
}

