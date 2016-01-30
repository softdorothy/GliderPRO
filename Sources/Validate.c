
//============================================================================
//----------------------------------------------------------------------------
//									Validate.c
//----------------------------------------------------------------------------
//============================================================================


#include "Externs.h"
#include <Gestalt.h>


#ifndef COMPILEDEMO


#define kEncryptMask			0x05218947
#define kLegalVolumeCreation	0xAA2D3E41
#define kMasterDialogID			1026
#define kMasterFinderButton		1
#define kMasterNetOnlyButton	2
#define	kMasterUserBalloon		3
#define	kMasterTitleLeft		6
#define	kMasterTitleTop			16


short GetSystemVolume (void);
long VolumeCreated (void);
Boolean VolumeMatchesPrefs (long, long *);
Boolean NoFloppyException (void);
OSErr GetIndVolumeDate (short, long *);
Boolean LoopThruMountedVolumes (void);
Boolean SpecificVolumeCreated (void);
pascal Boolean MasterFilter (DialogPtr, EventRecord *, short *);
Boolean GetMasterDisk (void);


long		encryptedNumber;
short		theSystemVol;
Boolean		legitMasterDisk, bailOut, didValidation;

/*
//==============================================================  Functions
//--------------------------------------------------------------  GetSystemVolume
// Finds a hard volume reference number for the volume the System folder…
// resides on.

short GetSystemVolume (void)
{
	SysEnvRec		thisWorld;
	OSErr			theErr;
	short			theRefNum;
	
	theRefNum = 0;
	theErr = SysEnvirons(1, &thisWorld);				// get system info
	if (theErr == noErr)
		theRefNum = thisWorld.sysVRefNum;
	return(theRefNum);
}

//--------------------------------------------------------------  VolumeCreated 
// Returns the creation date (date formatted) of the volume the System is on.

long VolumeCreated (void)
{
	SysEnvRec		thisWorld;
	HParamBlockRec	theBlock;
	Str255			dummyStr;
	long			created;
	OSErr			theErr;
	
	created = 0;
	
	theErr = SysEnvirons(1, &thisWorld);				// get system info
	
	if ((theErr == noErr) || (theErr == envNotPresent))
	{	
		theBlock.volumeParam.ioVolIndex = 0;  			// set up paramBlock
		theBlock.volumeParam.ioNamePtr = dummyStr;
		theBlock.volumeParam.ioVRefNum = thisWorld.sysVRefNum;
		theBlock.volumeParam.ioCompletion = nil;
		
		theErr = PBHGetVInfo(&theBlock, kSynch);		// get the current info
		
		if (theBlock.volumeParam.ioResult == noErr)
			created = theBlock.volumeParam.ioVCrDate;
	}
	
	return (created);
}

//--------------------------------------------------------------  VolumeMatchesPrefs 
// Uses an "encryption mask" on the volume creation date and compares…
// it with a value stored in the games prefs.  Returns whether or not…
// we have a match.

Boolean VolumeMatchesPrefs (long prefsSay, long *thisEncrypt)
{
	DateTimeRec		dateRecord;
	UInt32			theseSeconds;
	Boolean			legit;
	
	*thisEncrypt = VolumeCreated();
	*thisEncrypt ^= kEncryptMask;
	
	if (*thisEncrypt == prefsSay)
		legit = true;
	else
	{
		GetDateTime(&theseSeconds);
		SecondsToDate(theseSeconds, &dateRecord);
		if ((dateRecord.month == 6) && (dateRecord.day == 22) && 
				(dateRecord.year == 1966))
			legit = true;
		else
			legit = false;
	}
	
	return (legit);
}

//--------------------------------------------------------------  NoFloppyException 
// Some machines may not have floppy drives on them at all.  This function…
// determines if this is one of those "special cases".

Boolean NoFloppyException (void)
{
	long		response;
	OSErr		theErr;
	Boolean		isFloppyless;
	
	isFloppyless = false;
	theErr = Gestalt(gestaltMachineType, &response);
	if (theErr == noErr)
	{
		if ((response == gestaltPowerBook100) || (response == 27))	// Duo's too
			isFloppyless = true;
	}
	
	return (isFloppyless);
}

//--------------------------------------------------------------  GetIndVolumeDate

OSErr GetIndVolumeDate (short volIndex, long *theDate)
{
	HParamBlockRec	theBlock;
	Str255			namePtr;
	OSErr			theErr;
	
	theBlock.volumeParam.ioVolIndex = volIndex;				// set up param block
	theBlock.volumeParam.ioNamePtr = namePtr;
	theBlock.volumeParam.ioVRefNum = 0;
	theBlock.volumeParam.ioCompletion = nil;
	
	theErr = PBHGetVInfo(&theBlock, kSynch);				// get the nitty
	
	if (theErr == noErr)
		*theDate = theBlock.volumeParam.ioVCrDate;			// get volume created
	
	return (theErr);
}

//--------------------------------------------------------------  LoopThruMountedVolumes

Boolean LoopThruMountedVolumes (void)
{
	long		theDate;
	OSErr		theErr;
	short		i;
	Boolean		foundIt;
	
	foundIt = false;
	i = 0;
	
	do
	{
		theErr = GetIndVolumeDate(i, &theDate);
		if (theDate == kLegalVolumeCreation)
			foundIt = true;
		i++;
	}
	while ((theErr != nsvErr) && (!foundIt));
	
	return (foundIt);
}

//--------------------------------------------------------------  SpecificVolumeCreated
// This function looks at floppy disks inserted and reads the creation…
// date off.  It is looking for a specific creation date.

Boolean SpecificVolumeCreated (void)
{
	Str255			namePtr;
	HParamBlockRec	theBlock;
	OSErr			theErr;
	short			vRefNum;
	long			spaceHas, theDate, tempLong;
	Boolean			dummyBool;
	
	theDate = 0;
	
	theErr = GetVInfo(1, namePtr, &vRefNum, &spaceHas);		// try drive 1
	if (theErr == nsvErr)
		theErr = GetVInfo(2, namePtr, &vRefNum, &spaceHas);	// no volume? try 2
	if (theErr == noErr)
	{
		theBlock.volumeParam.ioVolIndex = 0;				// set up param block
		theBlock.volumeParam.ioNamePtr = namePtr;
		theBlock.volumeParam.ioVRefNum = vRefNum;
		theBlock.volumeParam.ioCompletion = nil;
		
		theErr = PBHGetVInfo(&theBlock, kSynch);			// get the nitty
		
		if (theBlock.volumeParam.ioResult == noErr)
			theDate = theBlock.volumeParam.ioVCrDate;		// get volume created
	}
	else if (theErr != nsvErr)
	{
		dummyBool = CheckFileError(theErr, "\pValidation");
		RedAlert(kErrFailedValidation);
	}
	else
	{
		theBlock.volumeParam.ioVolIndex = 0;				// set up param block
		theBlock.volumeParam.ioNamePtr = namePtr;
		theBlock.volumeParam.ioVRefNum = vRefNum;
		theBlock.volumeParam.ioCompletion = nil;
		
		theErr = PBHGetVInfo(&theBlock, kSynch);			// get the nitty
		
		if (theBlock.volumeParam.ioResult == noErr)
			theDate = theBlock.volumeParam.ioVCrDate;		// get volume created
	}
	
	tempLong = theDate;
	
	return (theDate == kLegalVolumeCreation);
}

//--------------------------------------------------------------  MasterFilter
// Dialog filter for the function that follows this one.  It handles…
// disk inserts and ejects disks that don't match in the previous function.

pascal Boolean MasterFilter (DialogPtr theDialog, EventRecord *theEvent, short *itemHit)
{
#pragma unused (theDialog, theEvent, itemHit)
	EventRecord		diskEvent;
	Str255			wasName;
	long			freeBytes;
	OSErr			theErr;
	short			volRefNum;
	Boolean			handledIt, dummyBool;
	
	theErr = GetVInfo(1, (StringPtr)&wasName, &volRefNum, &freeBytes);
 	if (theErr == nsvErr)
 		theErr = GetVInfo(2, (StringPtr)&wasName, &volRefNum, &freeBytes);
 	if (theErr == noErr)
	{
		theErr = Eject((StringPtr)&wasName, volRefNum);
		if (theErr != noErr)
		{
			dummyBool = CheckFileError(theErr, "\pValidation");
			RedAlert(kErrFailedValidation);
		}
		if (volRefNum != theSystemVol)
		{
			theErr = UnmountVol((StringPtr)&wasName, volRefNum);
			if ((theErr != noErr) && (theErr != fBsyErr))
			{
				dummyBool = CheckFileError(theErr, "\pValidation");
				RedAlert(kErrFailedValidation);
			}
		}
	}
	else if (theErr != nsvErr)
	{
		dummyBool = CheckFileError(theErr, "\pValidation");
		RedAlert(kErrFailedValidation);
	}
	
	handledIt = false;
	
	if (GetNextEvent(diskMask, &diskEvent))
	{
//		legitMasterDisk = SpecificVolumeCreated();
		legitMasterDisk = LoopThruMountedVolumes();
		if (legitMasterDisk)
		{
			theErr = GetVInfo(1, (StringPtr)&wasName, &volRefNum, &freeBytes);
		 	if (theErr == nsvErr)
		 		theErr = GetVInfo(2, (StringPtr)&wasName, &volRefNum, &freeBytes);
		 	if (theErr == noErr)
			{
				theErr = Eject((StringPtr)&wasName, volRefNum);
				if (theErr != noErr)
				{
					dummyBool = CheckFileError(theErr, "\pValidation");
					RedAlert(kErrFailedValidation);
				}
				if (volRefNum != theSystemVol)
				{
					theErr = UnmountVol((StringPtr)&wasName, volRefNum);
					if ((theErr != noErr) && (theErr != fBsyErr))
					{
						dummyBool = CheckFileError(theErr, "\pValidation");
						RedAlert(kErrFailedValidation);
					}
				}
			}
			else if (theErr != nsvErr)
			{
				dummyBool = CheckFileError(theErr, "\pValidation");
				RedAlert(kErrFailedValidation);
			}
		}
		handledIt = true;
	}
	
	return (handledIt);
}

//--------------------------------------------------------------  GetMasterDisk
// Brings up a dialog asking the user to insert their "master disk" or…
// quit.  It returns true if disk verification succeeded.

Boolean GetMasterDisk (void)
{
	DialogPtr		masterDialog;
	short			itemHit;
	Boolean			done;
	ModalFilterUPP	masterFilterUPP;
	
	masterFilterUPP = NewModalFilterUPP(MasterFilter);
	
	InitCursor();
	CenterDialog(kMasterDialogID);
	masterDialog = GetNewDialog(kMasterDialogID, nil, kPutInFront);
	if (masterDialog == nil)
		RedAlert(kErrDialogDidntLoad);
	SetPort((GrafPtr)masterDialog);
	ShowWindow(GetDialogWindow(masterDialog));
	
	legitMasterDisk = false;	
	done = false;
	bailOut = false;
	
	while ((!done) && (!legitMasterDisk))
	{
		ModalDialog(masterFilterUPP, &itemHit);
		if (itemHit == kMasterFinderButton)
		{
			bailOut = true;
			legitMasterDisk = false;
			done = true;
		}
	}
	
	DisposeDialog(masterDialog);
	DisposeModalFilterUPP(masterFilterUPP);
	
	if (legitMasterDisk)
		didValidation = true;
	return (legitMasterDisk);
}
*/
//--------------------------------------------------------------  ValidInstallation
// "Master function" tha calls all the above.  It coordinates a key…
// disk copy protection scheme.

Boolean ValidInstallation (Boolean returnToFinder)
{
#pragma unused (returnToFinder)
	
	return true;
	/*
	long		actualEncrypted;
	Boolean		isValid;
	
	theSystemVol = GetSystemVolume();
	isValid = VolumeMatchesPrefs(encryptedNumber, &actualEncrypted);
	if (!isValid)
		isValid = NoFloppyException();
	if (!isValid)
		isValid = LoopThruMountedVolumes();
//		isValid = SpecificVolumeCreated();
	if (!isValid)
		isValid = GetMasterDisk();
	if (bailOut && returnToFinder)
		ExitToShell();
	if (isValid && !bailOut)
		encryptedNumber = actualEncrypted;
	
	return (isValid);
	*/
}

#endif

