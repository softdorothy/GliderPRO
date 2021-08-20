
//============================================================================
//----------------------------------------------------------------------------
//								   SavedGames.c
//----------------------------------------------------------------------------
//============================================================================


#include <StringCompare.h>
#include "Externs.h"
#include "House.h"


#define kSavedGameVersion		0x0200


void SavedGameMismatchError (StringPtr);


gameType	smallGame;

extern	FSSpecPtr	theHousesSpecs;
extern	short		numStarsRemaining, thisHouseIndex;
extern	Boolean		twoPlayerGame;


//==============================================================  Functions
//--------------------------------------------------------------  SaveGame2

void SaveGame2 (void)
{
		// Add NavServices later.
/*
	StandardFileReply	theReply;
	FSSpec				tempSpec;
	Str255				gameNameStr;
	Size				byteCount;
	OSErr				theErr;
	houseType			*thisHousePtr;
	roomType			*srcRoom;
	savedRoom			*destRoom;
	gamePtr				savedGame;
	short				r, i, numRooms, gameRefNum;
	char				wasState;
	
	FlushEvents(everyEvent, 0);
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	thisHousePtr = *thisHouse;
	
	numRooms = thisHousePtr->nRooms;
	
	HSetState((Handle)thisHouse, wasState);
	
	byteCount = sizeof(game2Type) + sizeof(savedRoom) * numRooms;
	savedGame = (gamePtr)NewPtr(byteCount);
	if (savedGame == nil)
	{
		YellowAlert(kYellowFailedSaveGame, MemError());
		return;
	}
	
	GetFirstWordOfString(thisHouseName, gameNameStr);
	if (gameNameStr[0] > 23)
		gameNameStr[0] = 23;
	PasStringConcat(gameNameStr, "\p Game");
	
	StandardPutFile("\pSave Game As:", gameNameStr, &theReply);
	if (!theReply.sfGood)
		return;
	
	if (theReply.sfReplacing)
	{
		theErr = FSMakeFSSpec(theReply.sfFile.vRefNum, theReply.sfFile.parID, 
				theReply.sfFile.name, &tempSpec);
		if (!CheckFileError(theErr, "\pSaved Game"))
			return;
		
		theErr = FSpDelete(&tempSpec);
		if (!CheckFileError(theErr, "\pSaved Game"))
			return;
	}
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	thisHousePtr = *thisHouse;
	
	savedGame->house = theHousesSpecs[thisHouseIndex];
	savedGame->version = kSavedGameVersion;
	savedGame->wasStarsLeft = numStarsRemaining;
	savedGame->timeStamp = thisHousePtr->timeStamp;
	savedGame->where.h = theGlider.dest.left;
	savedGame->where.v = theGlider.dest.top;
	savedGame->score = theScore;
	savedGame->unusedLong = 0L;
	savedGame->unusedLong2 = 0L;
	savedGame->energy = batteryTotal;
	savedGame->bands = bandsTotal;
	savedGame->roomNumber = thisRoomNumber;
	savedGame->gliderState = theGlider.mode;
	savedGame->numGliders = mortals;
	savedGame->foil = foilTotal;
	savedGame->nRooms = numRooms;
	savedGame->facing = theGlider.facing;
	savedGame->showFoil = showFoil;
	
	for (r = 0; r < numRooms; r++)
	{
		destRoom = &(savedGame->savedData[r]);
		srcRoom = &(thisHousePtr->rooms[r]);
		
		destRoom->unusedShort = 0;
		destRoom->unusedByte = 0;
		destRoom->visited = srcRoom->visited;
		for (i = 0; i < kMaxRoomObs; i++)
			destRoom->objects[i] = srcRoom->objects[i];
	}
	
	HSetState((Handle)thisHouse, wasState);
	
	theErr = FSpCreate(&theReply.sfFile, 'ozm5', 'gliG', theReply.sfScript);
	if (CheckFileError(theErr, "\pSaved Game"))
	{
		theErr = FSpOpenDF(&theReply.sfFile, fsCurPerm, &gameRefNum);
		if (CheckFileError(theErr, "\pSaved Game"))
		{
			theErr = SetFPos(gameRefNum, fsFromStart, 0L);
			if (CheckFileError(theErr, "\pSaved Game"))
			{
				theErr = FSWrite(gameRefNum, &byteCount, (Ptr)savedGame);
				if (CheckFileError(theErr, "\pSaved Game"))
				{
					theErr = SetEOF(gameRefNum, byteCount);
					if (CheckFileError(theErr, "\pSaved Game"))
					{
					}
				}
			}
			theErr = FSClose(gameRefNum);
			if (CheckFileError(theErr, "\pSaved Game"))
			{
			}
		}
	}
	DisposePtr((Ptr)savedGame);
	*/
}

//--------------------------------------------------------------  SavedGameMismatchError

void SavedGameMismatchError (StringPtr gameName)
{
	#define		kSavedGameErrorAlert	1044
	short		whoCares;
	
	InitCursor();
	
//	CenterAlert(kSavedGameErrorAlert);
	ParamText(gameName, thisHouseName, "\p", "\p");
	
	whoCares = Alert(kSavedGameErrorAlert, nil);
}

//--------------------------------------------------------------  OpenSavedGame

Boolean OpenSavedGame (void)
{
return false;		// TEMP fix this iwth NavServices
/*
	StandardFileReply	theReply;
	SFTypeList			theList;
	houseType			*thisHousePtr;
	roomType			*destRoom;
	savedRoom			*srcRoom;
	gamePtr				savedGame;
	long				byteCount;
	OSErr				theErr;
	short				r, i, gameRefNum;
	char				wasState;
	
	theList[0] = 'gliG';
	
	StandardGetFile(nil, 1, theList, &theReply);
	if (!theReply.sfGood)
		return(false);
	
	theErr = FSpOpenDF(&theReply.sfFile, fsCurPerm, &gameRefNum);
	if (!CheckFileError(theErr, "\pSaved Game"))
		return(false);
	
	theErr = GetEOF(gameRefNum, &byteCount);
	if (!CheckFileError(theErr, "\pSaved Game"))
	{
		theErr = FSClose(gameRefNum);
		return(false);
	}
	
	savedGame = (gamePtr)NewPtr(byteCount);
	if (savedGame == nil)
	{
		YellowAlert(kYellowFailedSaveGame, MemError());
		theErr = FSClose(gameRefNum);
		return(false);
	}
	
	theErr = SetFPos(gameRefNum, fsFromStart, 0L);
	if (!CheckFileError(theErr, "\pSaved Game"))
	{
		DisposePtr((Ptr)savedGame);
		theErr = FSClose(gameRefNum);
		return(false);
	}
	
	theErr = FSRead(gameRefNum, &byteCount, savedGame);
	if (!CheckFileError(theErr, "\pSaved Game"))
	{
		DisposePtr((Ptr)savedGame);
		theErr = FSClose(gameRefNum);
		return(false);
	}
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	thisHousePtr = *thisHouse;
	
	if (!EqualString(savedGame->house.name, thisHouseName, true, true))
	{
		SavedGameMismatchError(savedGame->house.name);
		HSetState((Handle)thisHouse, wasState);
		DisposePtr((Ptr)savedGame);
		theErr = FSClose(gameRefNum);
		return(false);
	}
	else if (thisHousePtr->timeStamp != savedGame->timeStamp)
	{
		YellowAlert(kYellowSavedTimeWrong, 0);
		HSetState((Handle)thisHouse, wasState);
		DisposePtr((Ptr)savedGame);
		theErr = FSClose(gameRefNum);
		return(false);
	}
	else if (savedGame->version != kSavedGameVersion)
	{
		YellowAlert(kYellowSavedVersWrong, kSavedGameVersion);
		HSetState((Handle)thisHouse, wasState);
		DisposePtr((Ptr)savedGame);
		theErr = FSClose(gameRefNum);
		return(false);
	}
	else if (savedGame->nRooms != thisHousePtr->nRooms)
	{
		YellowAlert(kYellowSavedRoomsWrong, savedGame->nRooms - thisHousePtr->nRooms);
		HSetState((Handle)thisHouse, wasState);
		DisposePtr((Ptr)savedGame);
		theErr = FSClose(gameRefNum);
		return(false);
	}
	else
	{
		smallGame.wasStarsLeft = savedGame->wasStarsLeft;
		smallGame.where.h = savedGame->where.h;
		smallGame.where.v = savedGame->where.v;
		smallGame.score = savedGame->score;
		smallGame.unusedLong = savedGame->unusedLong;
		smallGame.unusedLong2 = savedGame->unusedLong2;
		smallGame.energy = savedGame->energy;
		smallGame.bands = savedGame->bands;
		smallGame.roomNumber = savedGame->roomNumber;
		smallGame.gliderState = savedGame->gliderState;
		smallGame.numGliders = savedGame->numGliders;
		smallGame.foil = savedGame->foil;
		smallGame.unusedShort = 0;
		smallGame.facing = savedGame->facing;
		smallGame.showFoil = savedGame->showFoil;
		
		for (r = 0; r < savedGame->nRooms; r++)
		{
			srcRoom = &(savedGame->savedData[r]);
			destRoom = &(thisHousePtr->rooms[r]);
			destRoom->visited = srcRoom->visited;
			for (i = 0; i < kMaxRoomObs; i++)
				destRoom->objects[i] = srcRoom->objects[i];
		}
	}
	HSetState((Handle)thisHouse, wasState);
	
	DisposePtr((Ptr)savedGame);
	
	theErr = FSClose(gameRefNum);
	if (!CheckFileError(theErr, "\pSaved Game"))
		return (false);
	
	return (true);
	*/
}

//--------------------------------------------------------------  SaveGame

// This is probably about 3 days away from becoming the "old" functionÉ
// for saving games.

void SaveGame (Boolean doSave)
{
	houseType		*thisHousePtr;
	UInt32			stamp;
	char			wasState;
	
	if (twoPlayerGame)
		return;
	
	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);
	thisHousePtr = *thisHouse;
	
	if (doSave)
	{
		thisHousePtr->savedGame.version = kSavedGameVersion;
		thisHousePtr->savedGame.wasStarsLeft = numStarsRemaining;
		GetDateTime(&stamp);
		thisHousePtr->savedGame.timeStamp = (long)stamp;
		thisHousePtr->savedGame.where.h = theGlider.dest.left;
		thisHousePtr->savedGame.where.v = theGlider.dest.top;
		thisHousePtr->savedGame.score = theScore;
		thisHousePtr->savedGame.unusedLong = 0L;
		thisHousePtr->savedGame.unusedLong2 = 0L;
		thisHousePtr->savedGame.energy = batteryTotal;
		thisHousePtr->savedGame.bands = bandsTotal;
		thisHousePtr->savedGame.roomNumber = thisRoomNumber;
		thisHousePtr->savedGame.gliderState = theGlider.mode;
		thisHousePtr->savedGame.numGliders = mortals;
		thisHousePtr->savedGame.foil = foilTotal;
		thisHousePtr->savedGame.unusedShort = 0;
		thisHousePtr->savedGame.facing = theGlider.facing;
		thisHousePtr->savedGame.showFoil = showFoil;
		
		thisHousePtr->hasGame = true;
	}
	else
	{
		thisHousePtr->hasGame = false;
	}
	
	HSetState((Handle)thisHouse, wasState);
	
	if (doSave)
	{
		if (!WriteHouse(theMode == kEditMode))
			YellowAlert(kYellowFailedWrite, 0);
	}
}

