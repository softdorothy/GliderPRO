//============================================================================
//----------------------------------------------------------------------------
//								DialogUtils.h
//----------------------------------------------------------------------------
//============================================================================


#include <Dialogs.h>


void BringUpDialog (DialogPtr *theDialog, short dialogID);
//void GetPutDialogCorner (Point *);
//void GetGetDialogCorner (Point *);
//void CenterDialog (short);
void GetDialogRect (Rect *, short);
//void TrueCenterDialog (short);
//void CenterAlert (short);
//void ZoomOutDialogRect (short);
//void ZoomOutAlertRect (short);
void FlashDialogButton (DialogPtr, short);
void DrawDefaultButton (DialogPtr);
void GetDialogString (DialogPtr, short, StringPtr);
void SetDialogString (DialogPtr, short, StringPtr);
short GetDialogStringLen (DialogPtr, short);
void GetDialogItemValue (DialogPtr, short, short *);
void SetDialogItemValue (DialogPtr, short, short);
void ToggleDialogItemValue (DialogPtr, short);
void SetDialogNumToStr (DialogPtr, short, long);
void GetDialogNumFromStr (DialogPtr, short, long *);
void GetDialogItemRect (DialogPtr, short, Rect *);
void SetDialogItemRect (DialogPtr, short, Rect *);
void OffsetDialogItemRect (DialogPtr, short, short, short);
void SelectFromRadioGroup (DialogPtr, short, short, short);
//void AddMenuToPopUp (DialogPtr, short, MenuHandle);
void GetPopUpMenuValue (DialogPtr, short, short *);
void SetPopUpMenuValue (DialogPtr, short, short);
void MyEnableControl(DialogPtr, short);
void MyDisableControl(DialogPtr, short);
void DrawDialogUserText (DialogPtr, short, StringPtr, Boolean);
void DrawDialogUserText2 (DialogPtr, short, StringPtr);
void LoadDialogPICT (DialogPtr, short, short);
void FrameDialogItem (DialogPtr, short);
void FrameDialogItemC (DialogPtr, short, long);
void FrameOvalDialogItem (DialogPtr, short);
void BorderDialogItem (DialogPtr, short, short);
void ShadowDialogItem (DialogPtr, short, short);
void EraseDialogItem (DialogPtr, short);
