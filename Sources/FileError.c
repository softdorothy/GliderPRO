
//============================================================================
//----------------------------------------------------------------------------
//								   FileError.c
//----------------------------------------------------------------------------
//============================================================================


#include <NumberFormatting.h>
#include <TextUtils.h>
#include "Externs.h"


#define	rFileErrorAlert		140
#define	rFileErrorStrings	140


//==============================================================  Functions
//--------------------------------------------------------------  CheckFileError

// Given a result code (returned from a previous file operation) this…
// function cheks to see if the result code is an error and, if it is…
// a common error for which I have a string message, I bring up an…
// alert with the error message.  If it is an unusual error, I still…
// bring up an alert but with "Miscellaneous file error" and the…
// error ID.

Boolean CheckFileError (short resultCode, StringPtr fileName)
{
	short			dummyInt, stringIndex;
	Str255			errMessage, errNumString;
	
	if (resultCode == noErr)		// No problems?  Then cruise
		return(true);
	
	switch (resultCode)
	{
		case dirFulErr:
			stringIndex = 2;
			break;
		case dskFulErr:
			stringIndex = 3;
			break;
		case ioErr:
			stringIndex = 4;
			break;
		case bdNamErr:
			stringIndex = 5;
			break;
		case fnOpnErr:
			stringIndex = 6;
			break;
		case mFulErr:
			stringIndex = 7;
			break;
		case tmfoErr:
			stringIndex = 8;
			break;
		case wPrErr:
			stringIndex = 9;
			break;
		case fLckdErr:
			stringIndex = 10;
			break;
		case vLckdErr:
			stringIndex = 11;
			break;
		case fBsyErr:
			stringIndex = 12;
			break;
		case dupFNErr:
			stringIndex = 13;
			break;
		case opWrErr:
			stringIndex = 14;
			break;
		case volOffLinErr:
			stringIndex = 15;
			break;
		case permErr:
			stringIndex = 16;
			break;
		case wrPermErr:
			stringIndex = 17;
			break;
		default:
			stringIndex = 1;
			break;
	}
	InitCursor();
	
	GetIndString(errMessage, rFileErrorStrings, stringIndex);
	NumToString((long)resultCode, errNumString);
	ParamText(errMessage, errNumString, fileName, "\p");
	
//	CenterAlert(rFileErrorAlert);
	dummyInt = Alert(rFileErrorAlert, 0L);
	
	return(false);
}


