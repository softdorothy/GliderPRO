
//============================================================================
//----------------------------------------------------------------------------
//									Music.c
//----------------------------------------------------------------------------
//============================================================================


#include <Resources.h>
#include <Sound.h>
#include "Environ.h"
#include "Externs.h"


#define kBaseBufferMusicID			2000
#define kMaxMusic					7
#define kLastMusicPiece				16
#define kLastGamePiece				6


pascal void MusicCallBack (SndChannelPtr, SndCommand *);
OSErr LoadMusicSounds (void);
OSErr DumpMusicSounds (void);
OSErr OpenMusicChannel (void);
OSErr CloseMusicChannel (void);


SndCallBackUPP	musicCallBackUPP;
SndChannelPtr	musicChannel;
Ptr				theMusicData[kMaxMusic];
short			musicSoundID, musicCursor;
short			musicScore[kLastMusicPiece];
short			gameScore[kLastGamePiece];
short			musicMode;
Boolean			isMusicOn, isPlayMusicIdle, isPlayMusicGame;
Boolean			failedMusic, dontLoadMusic;

extern	Boolean		isSoundOn;


//==============================================================  Functions
//--------------------------------------------------------------  StartMusic

OSErr StartMusic (void)
{
	SndCommand	theCommand;
	OSErr		theErr;
	short		soundVolume;
	
	theErr = noErr;
	
	if (dontLoadMusic)
		return(theErr);
	
	UnivGetSoundVolume(&soundVolume, thisMac.hasSM3);
	
	if ((soundVolume != 0) && (!failedMusic))
	{
		theCommand.cmd = bufferCmd;
		theCommand.param1 = 0;
		theCommand.param2 = (long)(theMusicData[musicSoundID]);
		theErr = SndDoCommand(musicChannel, &theCommand, false);
		if (theErr != noErr)
			return (theErr);
		
		theCommand.cmd = 0;
		theCommand.param1 = 1964;
		theCommand.param2 = SetCurrentA5();
		theErr = SndDoCommand(musicChannel, &theCommand, false);
		if (theErr != noErr)
			return (theErr);
		
		musicCursor++;
		if (musicCursor >= kLastMusicPiece)
			musicCursor = 0;
		musicSoundID = musicScore[musicCursor];
		
		theCommand.cmd = bufferCmd;
		theCommand.param1 = 0;
		theCommand.param2 = (long)(theMusicData[musicSoundID]);
		theErr = SndDoCommand(musicChannel, &theCommand, false);
		if (theErr != noErr)
			return (theErr);
		
		theCommand.cmd = callBackCmd;
		theCommand.param1 = 0;
		theCommand.param2 = SetCurrentA5();
		theErr = SndDoCommand(musicChannel, &theCommand, false);
		
		isMusicOn = true;
	}
	
	return (theErr);
}

//--------------------------------------------------------------  StopTheMusic

void StopTheMusic (void)
{
	SndCommand	theCommand;
	OSErr		theErr;
	
	if (dontLoadMusic)
		return;
	
	theErr = noErr;
	if ((isMusicOn) && (!failedMusic))
	{
		theCommand.cmd = flushCmd;
		theCommand.param1 = 0;
		theCommand.param2 = 0L;
		theErr = SndDoImmediate(musicChannel, &theCommand);
		
		theCommand.cmd = quietCmd;
		theCommand.param1 = 0;
		theCommand.param2 = 0L;
		theErr = SndDoImmediate(musicChannel, &theCommand);
		
		isMusicOn = false;
	}
}

//--------------------------------------------------------------  ToggleMusicWhilePlaying

void ToggleMusicWhilePlaying (void)
{
	OSErr		theErr;
	
	if (dontLoadMusic)
		return;
	
	if (isPlayMusicGame)
	{
		if (!isMusicOn)
			theErr = StartMusic();
	}
	else
	{
		if (isMusicOn)
			StopTheMusic();
	}
}

//--------------------------------------------------------------  SetMusicalPiece

void SetMusicalMode (short newMode)
{	
	if (dontLoadMusic)
		return;
	
	switch (newMode)
	{
		case kKickGameScoreMode:
		musicCursor = 2;
		break;
		
		case kProdGameScoreMode:
		musicCursor = -1;
		break;
		
		default:
		musicMode = newMode;
		musicCursor = 0;
		break;
	}
}

//--------------------------------------------------------------  MusicCallBack

pascal void MusicCallBack (SndChannelPtr theChannel, SndCommand *theCommand)
{
#pragma unused (theChannel)
	long		thisA5, gameA5;
	OSErr		theErr;
	
//	gameA5 = theCommand.param2;
//	thisA5 = SetA5(gameA5);
	
	switch (musicMode)
	{
		case kPlayGameScoreMode:
		musicCursor++;
		if (musicCursor >= kLastGamePiece)
			musicCursor = 1;
		musicSoundID = gameScore[musicCursor];
		if (musicSoundID < 0)
		{
			musicCursor += musicSoundID;
			musicSoundID = gameScore[musicCursor];
		}
		break;
		
		case kPlayWholeScoreMode:
		musicCursor++;
		if (musicCursor >= kLastMusicPiece - 1)
			musicCursor = 0;
		musicSoundID = musicScore[musicCursor];
		break;
		
		default:
		musicSoundID = musicMode;
		break;
	}
	
	theCommand->cmd = bufferCmd;
	theCommand->param1 = 0;
	theCommand->param2 = (long)(theMusicData[musicSoundID]);
	theErr = SndDoCommand(musicChannel, theCommand, false);
	
	theCommand->cmd = callBackCmd;
	theCommand->param1 = 0;
	theCommand->param2 = gameA5;
	theErr = SndDoCommand(musicChannel, theCommand, false);
	
	thisA5 = SetA5(thisA5);
}

//--------------------------------------------------------------  LoadMusicSounds

OSErr LoadMusicSounds (void)
{
	Handle		theSound;
	long		soundDataSize;
	OSErr		theErr;
	short		i;
	
	theErr = noErr;
	
	for (i = 0; i < kMaxMusic; i++)
		theMusicData[i] = nil;
	
	for (i = 0; i < kMaxMusic; i++)
	{
		theSound = GetResource('snd ', i + kBaseBufferMusicID);
		if (theSound == nil)
			return (MemError());
		
		HLock(theSound);
		soundDataSize = GetHandleSize(theSound) - 20L;
		HUnlock(theSound);
		
		theMusicData[i] = NewPtr(soundDataSize);
		if (theMusicData[i] == nil)
			return (MemError());
		
		HLock(theSound);
		BlockMove((Ptr)(*theSound + 20L), theMusicData[i], soundDataSize);
		ReleaseResource(theSound);
	}
	return (theErr);
}

//--------------------------------------------------------------  DumpMusicSounds

OSErr DumpMusicSounds (void)
{
	OSErr		theErr;
	short		i;
	
	theErr = noErr;
	
	for (i = 0; i < kMaxMusic; i++)
	{
		if (theMusicData[i] != nil)
			DisposePtr(theMusicData[i]);
		theMusicData[i] = nil;
	}
	
	return (theErr);
}

//--------------------------------------------------------------  OpenMusicChannel

OSErr OpenMusicChannel (void)
{
	OSErr		theErr;
	
	musicCallBackUPP = NewSndCallBackProc(MusicCallBack);
	
	theErr = noErr;
	
	if (musicChannel != nil)
		return (theErr);
	
	musicChannel = nil;
	theErr = SndNewChannel(&musicChannel, 
			sampledSynth, initNoInterp + initMono, 
			(SndCallBackUPP)musicCallBackUPP);
	
	return (theErr);
}

//--------------------------------------------------------------  CloseMusicChannel

OSErr CloseMusicChannel (void)
{
	OSErr		theErr;
	
	theErr = noErr;
	
	if (musicChannel != nil)
		theErr = SndDisposeChannel(musicChannel, true);
	musicChannel = nil;
	
	DisposeSndCallBackUPP(musicCallBackUPP);
	
	return (theErr);
}

//--------------------------------------------------------------  InitMusic

void InitMusic (void)
{
	OSErr		theErr;
	
	if (dontLoadMusic)
		return;
	
	musicChannel = nil;
	
	failedMusic = false;
	isMusicOn = false;
	theErr = LoadMusicSounds();
	if (theErr != noErr)
	{
		YellowAlert(kYellowNoMusic, theErr);
		failedMusic = true;
		return;
	}
	theErr = OpenMusicChannel();
	
	musicScore[0] = 0;
	musicScore[1] = 1;
	musicScore[2] = 2;
	musicScore[3] = 3;
	musicScore[4] = 4;
	musicScore[5] = 4;
	musicScore[6] = 0;
	musicScore[7] = 1;
	musicScore[8] = 2;
	musicScore[9] = 3;
	musicScore[10] = kPlayChorus;
	musicScore[11] = kPlayChorus;
	musicScore[12] = kPlayRefrainSparse1;
	musicScore[13] = kPlayRefrainSparse2;
	musicScore[14] = kPlayChorus;
	musicScore[15] = kPlayChorus;
	
	gameScore[0] = kPlayRefrainSparse2;
	gameScore[1] = kPlayRefrainSparse1;
	gameScore[2] = -1;
	gameScore[3] = kPlayRefrainSparse2;
	gameScore[4] = kPlayChorus;
	gameScore[5] = kPlayChorus;
	
	musicCursor = 0;
	musicSoundID = musicScore[musicCursor];
	musicMode = kPlayWholeScoreMode;
	
	if (isPlayMusicIdle)
	{
		theErr = StartMusic();
		if (theErr != noErr)
		{
			YellowAlert(kYellowNoMusic, theErr);
			failedMusic = true;
		}
	}
}

//--------------------------------------------------------------  KillMusic

void KillMusic (void)
{
	OSErr		theErr;
	
	if (dontLoadMusic)
		return;
	
	theErr = DumpMusicSounds();
	theErr = CloseMusicChannel();
}

//--------------------------------------------------------------  MusicBytesNeeded

long MusicBytesNeeded (void)
{
	Handle		theSound;
	long		totalBytes;
	short		i;
	
	totalBytes = 0L;
	SetResLoad(false);
	for (i = 0; i < kMaxMusic; i++)
	{
		theSound = GetResource('snd ', i + kBaseBufferMusicID);
		if (theSound == nil)
		{
			SetResLoad(true);
			return ((long)ResError());
		}
		totalBytes += GetMaxResourceSize(theSound);
//		ReleaseResource(theSound);
	}
	SetResLoad(true);
	return totalBytes;
}

//--------------------------------------------------------------  TellHerNoMusic

void TellHerNoMusic (void)
{
	#define		kNoMemForMusicAlert	1038
	short		hitWhat;
	
//	CenterAlert(kNoMemForMusicAlert);
	hitWhat = Alert(kNoMemForMusicAlert, nil);
}

