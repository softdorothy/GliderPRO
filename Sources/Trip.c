
//============================================================================
//----------------------------------------------------------------------------
//								   	Trip.c
//----------------------------------------------------------------------------
//============================================================================


#include "Externs.h"
#include "Environ.h"


extern	dynaPtr		dinahs;
extern	hotPtr		hotSpots;
extern	short		numDynamics, tvWithMovieNumber;
extern	Boolean		tvOn;


//==============================================================  Functions
//--------------------------------------------------------------  ToggleToaster

void ToggleToaster (short index)
{
	dinahs[index].active = !dinahs[index].active;
}

//--------------------------------------------------------------  ToggleMacPlus

void ToggleMacPlus (short index)
{
	dinahs[index].active = !dinahs[index].active;
	if (dinahs[index].active)
		dinahs[index].timer = 40;
	else
		dinahs[index].timer = 10;
}

//--------------------------------------------------------------  ToggleTV

void ToggleTV (short index)
{
	dinahs[index].active = !dinahs[index].active;
	if ((thisMac.hasQT) && (hasMovie) && (tvInRoom) && (tvWithMovieNumber == index))
	{
		if (dinahs[index].active)
		{
			GoToBeginningOfMovie(theMovie);
			StartMovie(theMovie);
			tvOn = true;
		}
		else
		{
			StopMovie(theMovie);
			tvOn = false;
		}
	}
	dinahs[index].timer = 4;
}

//--------------------------------------------------------------  ToggleCoffee

void ToggleCoffee (short index)
{
	dinahs[index].active = !dinahs[index].active;
	dinahs[index].timer = 4;
}

//--------------------------------------------------------------  ToggleOutlet

void ToggleOutlet (short index)
{
	dinahs[index].active = !dinahs[index].active;
}

//--------------------------------------------------------------  ToggleVCR

void ToggleVCR (short index)
{
	dinahs[index].active = !dinahs[index].active;
	dinahs[index].timer = 4;
}

//--------------------------------------------------------------  ToggleStereo

void ToggleStereos (short index)
{
	if (dinahs[index].timer == 0)
	{
		dinahs[index].active = !dinahs[index].active;
		dinahs[index].timer = 4;
	}
}

//--------------------------------------------------------------  ToggleMicrowave

void ToggleMicrowave (short index)
{
	dinahs[index].active = !dinahs[index].active;
	dinahs[index].timer = 4;
}

//--------------------------------------------------------------  ToggleBalloon

void ToggleBalloon (short index)
{
	dinahs[index].active = !dinahs[index].active;
}

//--------------------------------------------------------------  ToggleCopter

void ToggleCopter (short index)
{
	dinahs[index].active = !dinahs[index].active;
}

//--------------------------------------------------------------  ToggleDart

void ToggleDart (short index)
{
	dinahs[index].active = !dinahs[index].active;
}

//--------------------------------------------------------------  ToggleBall

void ToggleBall (short index)
{
	dinahs[index].active = !dinahs[index].active;
}

//--------------------------------------------------------------  ToggleDrip

void ToggleDrip (short index)
{
	dinahs[index].active = !dinahs[index].active;
}

//--------------------------------------------------------------  ToggleFish

void ToggleFish (short index)
{
	dinahs[index].active = !dinahs[index].active;
}

//--------------------------------------------------------------  TriggerSwitch

void TriggerSwitch (short who)
{
	HandleSwitches(&hotSpots[who]);
}

//--------------------------------------------------------------  TriggerToast

void TriggerToast (short who)
{
	if (!dinahs[who].moving)
	{
		if (dinahs[who].active)
		{
			dinahs[who].vVel = (short)-dinahs[who].count;
			dinahs[who].frame = 0;
			dinahs[who].moving = true;
			PlayPrioritySound(kToastLaunchSound, kToastLaunchPriority);
		}
		else
			dinahs[who].frame = dinahs[who].timer;
	}
}

//--------------------------------------------------------------  TriggerOutlet

void TriggerOutlet (short who)
{
	if (dinahs[who].position == 0)
	{
		if (dinahs[who].active)
		{
			dinahs[who].position = 1;
			dinahs[who].timer = kLengthOfZap;
			PlayPrioritySound(kZapSound, kZapPriority);
		}
		else
			dinahs[who].timer = dinahs[who].count;
	}
}

//--------------------------------------------------------------  TriggerDrip

void TriggerDrip (short who)
{
	if ((!dinahs[who].moving) && (dinahs[who].timer > 7))
		dinahs[who].timer = 7;		// kick off drip
}

//--------------------------------------------------------------  TriggerFish

void TriggerFish (short who)
{
	if ((dinahs[who].active) && (!dinahs[who].moving))
	{
		dinahs[who].whole = dinahs[who].dest;
		dinahs[who].moving = true;
		dinahs[who].frame = 4;
		PlayPrioritySound(kFishOutSound, kFishOutPriority);
	}
}

//--------------------------------------------------------------  TriggerBalloon

void TriggerBalloon (short who)
{
	if (!dinahs[who].moving)
		dinahs[who].timer = kStartSparkle + 1;
}


//--------------------------------------------------------------  TriggerCopter

void TriggerCopter (short who)
{
	if (!dinahs[who].moving)
		dinahs[who].timer = kStartSparkle + 1;
}


//--------------------------------------------------------------  TriggerDart

void TriggerDart (short who)
{
	if (!dinahs[who].moving)
		dinahs[who].timer = kStartSparkle + 1;
}

//--------------------------------------------------------------  UpdateOutletsLighting

void UpdateOutletsLighting (short room, short nLights)
{
	short		i;
	
	for (i = 0; i < numDynamics; i++)
	{
		if ((dinahs[i].type == kOutlet) && (dinahs[i].room == room))
			dinahs[i].hVel = nLights;
	}
}

