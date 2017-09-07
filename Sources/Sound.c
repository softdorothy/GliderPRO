
//============================================================================
//----------------------------------------------------------------------------
//									Sound.c
//----------------------------------------------------------------------------
//============================================================================


#include <Resources.h>
#include <Sound.h>
#include "Externs.h"


#define kBaseBufferSoundID			1000
#define kMaxSounds					64
#define kNoSoundPlaying				-1


pascal void CallBack0 (SndChannelPtr, SndCommand *);
pascal void CallBack1 (SndChannelPtr, SndCommand *);
pascal void CallBack2 (SndChannelPtr, SndCommand *);
OSErr LoadBufferSounds (void);
void DumpBufferSounds (void);
OSErr OpenSoundChannels (void);
OSErr CloseSoundChannels (void);


SndCallBackUPP		callBack0UPP, callBack1UPP, callBack2UPP;
SndChannelPtr		channel0, channel1, channel2;
Ptr					theSoundData[kMaxSounds];
short				numSoundsLoaded, priority0, priority1, priority2;
short				soundPlaying0, soundPlaying1, soundPlaying2;
Boolean				soundLoaded[kMaxSounds], dontLoadSounds;
Boolean				channelOpen, isSoundOn, failedSound;


//==============================================================  Functions
//--------------------------------------------------------------  PlayPrioritySound

void PlayPrioritySound (short which, short priority)
{
	short		lowestPriority, whosLowest;
	
	if (failedSound || dontLoadSounds)
		return;
	
	if ((priority == kTriggerPriority) && 
			((priority0 == kTriggerPriority) || 
			((priority1 == kTriggerPriority)) || 
			((priority2 == kTriggerPriority))))
		return;
	
	whosLowest = 0;
	lowestPriority = priority0;
	
	if (priority1 < lowestPriority)
	{
		lowestPriority = priority1;
		whosLowest = 1;
	}
	
	if (priority2 < lowestPriority)
	{
		lowestPriority = priority2;
		whosLowest = 2;
	}
	
	if (priority >= lowestPriority)
	{
		switch (whosLowest)
		{
			case 0:
			PlaySound0(which, priority);
			break;
			
			case 1:
			PlaySound1(which, priority);
			break;
			
			case 2:
			PlaySound2(which, priority);
			break;
		}
	}
}

//--------------------------------------------------------------  FlushAnyTriggerPlaying

void FlushAnyTriggerPlaying (void)
{
	SndCommand	theCommand;
	OSErr		theErr;
	
	if (priority0 == kTriggerPriority)
	{
		theCommand.cmd = quietCmd;
		theCommand.param1 = 0;
		theCommand.param2 = 0;
		theErr = SndDoImmediate(channel0, &theCommand);
		theCommand.cmd = flushCmd;
		theCommand.param1 = 0;
		theCommand.param2 = 0;
		theErr = SndDoImmediate(channel0, &theCommand);
	}
	
	if (priority1 == kTriggerPriority)
	{
		theCommand.cmd = quietCmd;
		theCommand.param1 = 0;
		theCommand.param2 = 0;
		theErr = SndDoImmediate(channel1, &theCommand);
		theCommand.cmd = flushCmd;
		theCommand.param1 = 0;
		theCommand.param2 = 0;
		theErr = SndDoImmediate(channel1, &theCommand);
	}
	
	if (priority2 == kTriggerPriority)
	{
		theCommand.cmd = quietCmd;
		theCommand.param1 = 0;
		theCommand.param2 = 0;
		theErr = SndDoImmediate(channel2, &theCommand);
		theCommand.cmd = flushCmd;
		theCommand.param1 = 0;
		theCommand.param2 = 0;
		theErr = SndDoImmediate(channel2, &theCommand);
	}
}

//--------------------------------------------------------------  PlaySound0

void PlaySound0 (short soundID, short priority)
{
	SndCommand	theCommand;
	OSErr		theErr;
	
	if (failedSound || dontLoadSounds)
		return;
	
	theErr = noErr;
	if (isSoundOn)
	{
		priority0 = priority;
		soundPlaying0 = soundID;
		
		theCommand.cmd = bufferCmd;
		theCommand.param1 = 0;
		theCommand.param2 = (long)(theSoundData[soundID]);
		theErr = SndDoImmediate(channel0, &theCommand);
		
		theCommand.cmd = callBackCmd;
		theCommand.param1 = 0;
		theCommand.param2 = SetCurrentA5();
		theErr = SndDoCommand(channel0, &theCommand, true);
	}
}

//--------------------------------------------------------------  PlaySound1

void PlaySound1 (short soundID, short priority)
{
	SndCommand	theCommand;
	OSErr		theErr;
	
	if (failedSound || dontLoadSounds)
		return;
	
	theErr = noErr;
	if (isSoundOn)
	{
		priority1 = priority;
		soundPlaying1 = soundID;
		
		theCommand.cmd = bufferCmd;
		theCommand.param1 = 0;
		theCommand.param2 = (long)(theSoundData[soundID]);
		theErr = SndDoImmediate(channel1, &theCommand);
		
		theCommand.cmd = callBackCmd;
		theCommand.param1 = 0;
		theCommand.param2 = SetCurrentA5();
		theErr = SndDoCommand(channel1, &theCommand, true);
	}
}

//--------------------------------------------------------------  PlaySound2

void PlaySound2 (short soundID, short priority)
{
	SndCommand	theCommand;
	OSErr		theErr;
	
	if (failedSound || dontLoadSounds)
		return;
	
	theErr = noErr;
	if (isSoundOn)
	{
		theCommand.cmd = bufferCmd;
		theCommand.param1 = 0;
		theCommand.param2 = (long)(theSoundData[soundID]);
		theErr = SndDoImmediate(channel2, &theCommand);
		
		theCommand.cmd = callBackCmd;
		theCommand.param1 = 0;
		theCommand.param2 = SetCurrentA5();
		theErr = SndDoCommand(channel2, &theCommand, true);
		
		priority2 = priority;
		soundPlaying2 = soundID;
	}
}

//--------------------------------------------------------------  CallBack0

pascal void CallBack0 (SndChannelPtr theChannel, SndCommand *theCommand)
{
#pragma unused (theChannel)
	long		thisA5, gameA5;
	
	gameA5 = theCommand->param2;
	thisA5 = SetA5(gameA5);
	
	priority0 = 0;
	soundPlaying0 = kNoSoundPlaying;
	
	thisA5 = SetA5(thisA5);
}

//--------------------------------------------------------------  CallBack1

pascal void CallBack1 (SndChannelPtr theChannel, SndCommand *theCommand)
{
#pragma unused (theChannel)
	long		thisA5, gameA5;
	
	gameA5 = theCommand->param2;
	thisA5 = SetA5(gameA5);
	
	priority1 = 0;
	soundPlaying1 = kNoSoundPlaying;
	
	thisA5 = SetA5(thisA5);
}

//--------------------------------------------------------------  CallBack2

pascal void CallBack2 (SndChannelPtr theChannel, SndCommand *theCommand)
{
#pragma unused (theChannel)
	long		thisA5, gameA5;
	
	gameA5 = theCommand->param2;
	thisA5 = SetA5(gameA5);
	
	priority2 = 0;
	soundPlaying2 = kNoSoundPlaying;
	
	thisA5 = SetA5(thisA5);
}

//--------------------------------------------------------------  LoadTriggerSound

OSErr LoadTriggerSound (short soundID)
{
	Handle		theSound;
	long		soundDataSize;
	OSErr		theErr;
	
	if ((dontLoadSounds) || (theSoundData[kMaxSounds - 1] != nil))
		theErr = -1;
	else
	{
//		FlushAnyTriggerPlaying();
		
		theErr = noErr;
		
		theSound = GetResource('snd ', soundID);
		if (theSound == nil)
		{
			theErr = -1;
		}
		else
		{
			soundDataSize = GetHandleSize(theSound) - 20L;
			theSoundData[kMaxSounds - 1] = NewPtr(soundDataSize);
			HLock(theSound);
			if (theSoundData[kMaxSounds - 1] == nil)
			{
				ReleaseResource(theSound);
				theErr = MemError();
			}
			else
			{
				BlockMove((Ptr)(*theSound + 20L), theSoundData[kMaxSounds - 1], soundDataSize);
				ReleaseResource(theSound);
			}
		}
	}
	
	return (theErr);
}

//--------------------------------------------------------------  DumpTriggerSound

void DumpTriggerSound (void)
{
	if (theSoundData[kMaxSounds - 1] != nil)
		DisposePtr(theSoundData[kMaxSounds - 1]);
	theSoundData[kMaxSounds - 1] = nil;
}

//--------------------------------------------------------------  LoadBufferSounds

OSErr LoadBufferSounds (void)
{
	Handle		theSound;
	long		soundDataSize;
	OSErr		theErr;
	short		i;
	
	theErr = noErr;
	
	for (i = 0; i < kMaxSounds - 1; i++)
	{
		theSound = GetResource('snd ', i + kBaseBufferSoundID);
		if (theSound == nil)
			return (MemError());
		
		HLock(theSound);
		soundDataSize = GetHandleSize(theSound) - 20L;
		HUnlock(theSound);
		
		theSoundData[i] = NewPtr(soundDataSize);
		if (theSoundData[i] == nil)
			return (MemError());
		
		HLock(theSound);
		BlockMove((Ptr)(*theSound + 20L), theSoundData[i], soundDataSize);
		ReleaseResource(theSound);
	}
	
	theSoundData[kMaxSounds - 1] = nil;
	
	return (theErr);
}

//--------------------------------------------------------------  DumpBufferSounds

void DumpBufferSounds (void)
{
	short		i;
	
	for (i = 0; i < kMaxSounds; i++)
	{
		if (theSoundData[i] != nil)
			DisposePtr(theSoundData[i]);
		theSoundData[i] = nil;
	}
}

//--------------------------------------------------------------  OpenSoundChannels

OSErr OpenSoundChannels (void)
{
	OSErr		theErr;
	
	callBack0UPP = NewSndCallBackProc(CallBack0);
	callBack1UPP = NewSndCallBackProc(CallBack1);
	callBack2UPP = NewSndCallBackProc(CallBack2);
	
	theErr = noErr;
	
	if (channelOpen)
		return (theErr);
	
	theErr = SndNewChannel(&channel0, 
			sampledSynth, initNoInterp + initMono, 
			(SndCallBackUPP)callBack0UPP);
	if (theErr == noErr)
		channelOpen = true;
	else
		return (theErr);
	
	theErr = SndNewChannel(&channel1, 
			sampledSynth, initNoInterp + initMono, 
			(SndCallBackUPP)callBack1UPP);
	if (theErr == noErr)
		channelOpen = true;
	else
		return (theErr);
	
	theErr = SndNewChannel(&channel2, 
			sampledSynth, initNoInterp + initMono, 
			(SndCallBackUPP)callBack2UPP);
	if (theErr == noErr)
		channelOpen = true;
	
	return (theErr);
}

//--------------------------------------------------------------  CloseSoundChannels

OSErr CloseSoundChannels (void)
{
	OSErr		theErr;
	
	theErr = noErr;
	
	if (!channelOpen)
		return (theErr);
	
	if (channel0 != nil)
		theErr = SndDisposeChannel(channel0, true);
	channel0 = nil;
	
	if (channel1 != nil)
		theErr = SndDisposeChannel(channel1, true);
	channel1 = nil;
	
	if (channel2 != nil)
		theErr = SndDisposeChannel(channel2, true);
	channel2 = nil;
	
	if (theErr == noErr)
		channelOpen = false;
	
	DisposeSndCallBackUPP(callBack0UPP);
	DisposeSndCallBackUPP(callBack1UPP);
	DisposeSndCallBackUPP(callBack2UPP);
	
	return (theErr);
}

//--------------------------------------------------------------  InitSound

void InitSound (void)
{
	OSErr		theErr;
		
	if (dontLoadSounds)
		return;
	
	failedSound = false;
	
	channel0 = nil;
	channel1 = nil;
	channel2 = nil;
	
	priority0 = 0;
	priority1 = 0;
	priority2 = 0;
	soundPlaying0 = kNoSoundPlaying;
	soundPlaying1 = kNoSoundPlaying;
	soundPlaying2 = kNoSoundPlaying;
	
	theErr = LoadBufferSounds();
	if (theErr != noErr)
	{
		YellowAlert(kYellowFailedSound, theErr);
		failedSound = true;
	}
	
	if (!failedSound)
	{
		theErr = OpenSoundChannels();
		if (theErr != noErr)
		{
			YellowAlert(kYellowFailedSound, theErr);
			failedSound = true;
		}
	}
}

//--------------------------------------------------------------  KillSound

void KillSound (void)
{
	OSErr		theErr;
	
	if (dontLoadSounds)
		return;
	
	DumpBufferSounds();
	theErr = CloseSoundChannels();
}

//--------------------------------------------------------------  SoundBytesNeeded

long SoundBytesNeeded (void)
{
	Handle		theSound;
	long		totalBytes;
	short		i;
	
	totalBytes = 0L;
	SetResLoad(false);
	for (i = 0; i < kMaxSounds - 1; i++)
	{
		theSound = GetResource('snd ', i + kBaseBufferSoundID);
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

//--------------------------------------------------------------  TellHerNoSounds

void TellHerNoSounds (void)
{
	#define		kNoMemForSoundsAlert	1039
	short		hitWhat;
	
//	CenterAlert(kNoMemForSoundsAlert);
	hitWhat = Alert(kNoMemForSoundsAlert, nil);
}

//--------------------------------------------------------------  BitchAboutSM3

void BitchAboutSM3 (void)
{
	#define		kNoSoundManager3Alert	1030
	short		hitWhat;
	
//	CenterAlert(kNoSoundManager3Alert);
	hitWhat = Alert(kNoSoundManager3Alert, nil);
}

