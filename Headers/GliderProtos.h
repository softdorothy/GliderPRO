
//============================================================================
//----------------------------------------------------------------------------
//								GliderProtos.h
//----------------------------------------------------------------------------
//============================================================================


//--------------------------------------------------------------  Prototypes

void SetUpAppleEvents (void);							// --- AppleEvents.c

void BringUpBanner (void);								// --- Banner.c
SInt16 CountStarsInHouse (void);
void DisplayStarsRemaining (void);

void SetCoordinateHVD (SInt16, SInt16, SInt16);			// --- Coordinate.c
void DeltaCoordinateD (SInt16);
void UpdateCoordWindow (void);
void OpenCoordWindow (void);
void CloseCoordWindow (void);
void ToggleCoordinateWindow (void);

void NilSavedMaps (void);								// --- DynamicMaps.c
SInt16 BackUpToSavedMap (Rect *, SInt16,  SInt16);
SInt16 ReBackUpSavedMap (Rect *, SInt16, SInt16);
void RestoreFromSavedMap (SInt16, SInt16, Boolean);
void AddSparkle (Rect *);
void AddFlyingPoint (Rect *, SInt16, SInt16, SInt16);
void ReBackUpFlames (SInt16, SInt16);
void AddCandleFlame (SInt16, SInt16, SInt16, SInt16);
void ReBackUpTikiFlames (SInt16, SInt16);
void AddTikiFlame (SInt16, SInt16, SInt16, SInt16);
void ReBackUpBBQCoals (SInt16, SInt16);
void AddBBQCoals (SInt16, SInt16, SInt16, SInt16);
void ReBackUpPendulum (SInt16, SInt16);
void AddPendulum (SInt16, SInt16, SInt16, SInt16);
void ReBackUpStar (SInt16, SInt16);
void AddStar (SInt16, SInt16, SInt16, SInt16);
void StopPendulum (SInt16, SInt16);
void StopStar (SInt16, SInt16);
void AddAShreddedGlider (Rect *);
void RemoveShreds (void);
void ZeroFlamesAndTheLike (void);

void CheckDynamicCollision (SInt16, gliderPtr, Boolean);	// --- Dynamics.c
Boolean DidBandHitDynamic (SInt16);
void RenderToast (SInt16);
void RenderBalloon (SInt16);
void RenderCopter (SInt16);
void RenderDart (SInt16);
void RenderBall (SInt16);
void RenderDrip (SInt16);
void RenderFish (SInt16);
void HandleSparkleObject (SInt16);
void HandleToast (SInt16);
void HandleMacPlus (SInt16);
void HandleTV (SInt16);
void HandleCoffee (SInt16);
void HandleOutlet (SInt16);
void HandleVCR (SInt16);
void HandleStereo (SInt16);
void HandleMicrowave (SInt16);

void HandleBalloon (SInt16);								// --- Dynamics2.c
void HandleCopter (SInt16);
void HandleDart (SInt16);
void HandleBall (SInt16);
void HandleDrip (SInt16);
void HandleFish (SInt16);

void HandleDynamics (void);								// --- Dynamics3.c
void RenderDynamics (void);
void ZeroDinahs (void);
SInt16 AddDynamicObject (SInt16, Rect *, objectType *, SInt16, SInt16, Boolean);

void DoGameOver (void);									// --- GameOver.c
void FlagGameOver (void);
void DoDiedGameOver (void);

void HandleGrease (void);								// --- Grease.c
SInt16 ReBackUpGrease (SInt16, SInt16);
SInt16 AddGrease (SInt16, SInt16, SInt16, SInt16, SInt16, Boolean);
void SpillGrease (SInt16, SInt16);
void RedrawAllGrease (void);

void DoHighScores (void);								// --- HighScores.c
void SortHighScores (void);
void ZeroHighScores (void);
void ZeroAllButHighestScore (void);
Boolean TestHighScore (void);
Boolean WriteScoresToDisk (void);
Boolean ReadScoresFromDisk (void);

Boolean CreateNewHouse (void);							// --- House.c
Boolean InitializeEmptyHouse (void);
SInt16 RealRoomNumberCount (void);
SInt16 GetFirstRoomNumber (void);
void WhereDoesGliderBegin (Rect *, SInt16);
Boolean HouseHasOriginalPicts (void);
SInt16 CountHouseLinks (void);
void GenerateLinksList (void);
void SortRoomsObjects (SInt16);
void SortHouseObjects (void);
SInt16 CountRoomsVisited (void);
void GenerateRetroLinks (void);
void DoGoToDialog (void);
void ConvertHouseVer1To2 (void);
void ShiftWholeHouse (SInt16);

void DoHouseInfo (void);								// --- HouseInfo.c

Boolean OpenHouse (void);								// --- HouseIO.c
Boolean OpenSpecificHouse (FSSpec *);
Boolean SaveHouseAs (void);
Boolean ReadHouse (void);
Boolean WriteHouse (Boolean);
Boolean CloseHouse (void);
void OpenHouseResFork (void);
void CloseHouseResFork (void);
Boolean QuerySaveChanges (void);
void YellowAlert (SInt16, SInt16);

Boolean KeepObjectLegal (void);							// --- HouseLegal.c
void CheckHouseForProblems (void);

Boolean SectGlider (gliderPtr, Rect *, Boolean);		// --- Interactions.c
void HandleSwitches (hotPtr);
void HandleInteraction (void);
void FlagStillOvers (gliderPtr);

void InitializeMenus (void);							// --- InterfaceInit.c
void GetExtraCursors (void);
void VariableInit (void);

void GetDemoInput (gliderPtr);							// --- Input.c
void GetInput (gliderPtr);

SInt16 MergeFloorSuite (SInt16, SInt16);					// --- Link.c
void ExtractFloorSuite (SInt16, SInt16 *, SInt16 *);
void UpdateLinkControl (void);
void UpdateLinkWindow (void);
void OpenLinkWindow (void);
void CloseLinkWindow (void);
void HandleLinkClick (Point);

void RedrawSplashScreen (void);							// --- MainWindow.c
void UpdateMainWindow (void);
void UpdateMenuBarWindow (void);
void OpenMainWindow (void);
void CloseMainWindow (void);
void ZoomBetweenWindows (void);
void UpdateEditWindowTitle (void);
void HandleMainClick (Point, Boolean);
//void WashColorIn (void);

void CenterMapOnRoom (SInt16, SInt16);					// --- Map.c
Boolean ThisRoomVisibleOnMap (void);
void FindNewActiveRoomRect (void);
void FlagMapRoomsForUpdate (void);
void UpdateMapWindow (void);
void ResizeMapWindow (SInt16, SInt16);
void OpenMapWindow (void);
void CloseMapWindow (void);
void ToggleMapWindow (void);
void HandleMapClick (EventRecord *);
void MoveRoom (Point);

void DoMarquee (void);									// --- Marquee.c
void StartMarquee (Rect *);
void StartMarqueeHandled (Rect *, SInt16, SInt16);
void StopMarquee (void);
void PauseMarquee (void);
void ResumeMarquee (void);
void DragOutMarqueeRect (Point, Rect *);
void DragMarqueeRect (Point, Rect *, Boolean, Boolean);
void DragMarqueeHandle (Point, SInt16 *);
void DragMarqueeCorner (Point, SInt16 *, SInt16 *, Boolean);
Boolean MarqueeHasHandles (SInt16 *, SInt16 *);
Boolean PtInMarqueeHandle (Point);
void SetMarqueeGliderRect (SInt16, SInt16);
void InitMarquee (void);

void UpdateClipboardMenus (void);						// --- Menu.c
void DoAppleMenu (SInt16);
void DoGameMenu (SInt16);
void DoOptionsMenu (SInt16);
void DoHouseMenu (SInt16);
void DoMenuChoice (long);
void UpdateMenus (Boolean);
void UpdateMapCheckmark (Boolean);
void UpdateToolsCheckmark (Boolean);
void UpdateCoordinateCheckmark (Boolean);
#ifdef COMPILEDEMO
void DoNotInDemo (void);
#endif
void OpenCloseEditWindows (void);

void StartGliderFadingIn (gliderPtr);					// --- Modes.c
void StartGliderTransportingIn (gliderPtr);
void StartGliderFadingOut (gliderPtr);
void StartGliderGoingUpStairs (gliderPtr);
void StartGliderGoingDownStairs (gliderPtr);
void StartGliderMailingIn (gliderPtr, Rect *, hotPtr);
void StartGliderMailingOut (gliderPtr);
void StartGliderDuctingDown (gliderPtr, Rect *, hotPtr);
void StartGliderDuctingUp (gliderPtr, Rect *, hotPtr);
void StartGliderDuctingIn (gliderPtr);
void StartGliderTransporting (gliderPtr, hotPtr);
void FlagGliderNormal (gliderPtr);
void FlagGliderShredding (gliderPtr, Rect *);
void FlagGliderBurning (gliderPtr);
void FlagGliderFaceLeft (gliderPtr);
void FlagGliderFaceRight (gliderPtr);
void FlagGliderInLimbo (gliderPtr, Boolean);
void UndoGliderLimbo (gliderPtr);
void ToggleGliderFacing (gliderPtr);
void InsureGliderFacingRight (gliderPtr);
void InsureGliderFacingLeft (gliderPtr);
void ReadyGliderForTripUpStairs (gliderPtr);
void ReadyGliderForTripDownStairs (gliderPtr);
void StartGliderFoilGoing (gliderPtr);
void StartGliderFoilLosing (gliderPtr);
void TagGliderIdle (gliderPtr);

OSErr StartMusic (void);								// --- Music.c
void StopTheMusic (void);
void ToggleMusicWhilePlaying (void);
void SetMusicalMode (SInt16);
void InitMusic (void);
void KillMusic (void);
long MusicBytesNeeded (void);
void TellHerNoMusic (void);

Boolean AddNewObject (Point, SInt16, Boolean);			// --- ObjectAdd.c
SInt16 FindObjectSlotInRoom (SInt16);
Boolean DoesRoomNumHaveObject (SInt16, SInt16);
void ShoutNoMoreObjects (void);

void DrawSimpleBlowers (SInt16, Rect *);					// --- ObjectDraw.c
void DrawTiki (Rect *, SInt16);
void DrawInvisibleBlower (Rect *);
void DrawLiftArea (Rect *);
void DrawTable (Rect *, SInt16);
void DrawShelf (Rect *);
void DrawCabinet (Rect *);
void DrawSimpleFurniture (SInt16, Rect *);
void DrawCounter (Rect *);
void DrawDresser (Rect *);
void DrawDeckTable (Rect *, SInt16);
void DrawStool (Rect *, SInt16);
void DrawInvisObstacle (Rect *);
void DrawInvisBounce (Rect *);
void DrawRedClock (Rect *);
void DrawBlueClock (Rect *);
void DrawYellowClock (Rect *);
void DrawCuckoo (Rect *);
void DrawSimplePrizes (SInt16, Rect *);
void DrawGreaseRt (Rect *, SInt16, Boolean);
void DrawGreaseLf (Rect *, SInt16, Boolean);
void DrawFoil (Rect *);
void DrawInvisBonus (Rect *);
void DrawSlider (Rect *);

void DrawMailboxLeft (Rect *, SInt16);					// --- ObjectDraw2.c
void DrawMailboxRight (Rect *, SInt16);
void DrawSimpleTransport (SInt16, Rect *);
void DrawInvisTransport (Rect *);
void DrawLightSwitch (Rect *, Boolean);
void DrawMachineSwitch (Rect *, Boolean);
void DrawThermostat (Rect *, Boolean);
void DrawPowerSwitch (Rect *, Boolean);
void DrawKnifeSwitch (Rect *, Boolean);
void DrawInvisibleSwitch (Rect *);
void DrawTrigger (Rect *);
void DrawSoundTrigger (Rect *);
void DrawSimpleLight (SInt16, Rect *);
void DrawFlourescent (Rect *);
void DrawSimpleAppliance (SInt16, Rect *);
void DrawMacPlus (Rect *, Boolean, Boolean);
void DrawTrackLight (Rect *);
void DrawInvisLight (Rect *);
void DrawTV (Rect *, Boolean, Boolean);
void DrawCoffee (Rect *, Boolean, Boolean);
void DrawOutlet (Rect *);
void DrawVCR (Rect *, Boolean, Boolean);
void DrawStereo (Rect *, Boolean, Boolean);
void DrawMicrowave (Rect *, Boolean, Boolean);
void DrawBalloon (Rect *);
void DrawCopter (Rect *);
void DrawDart (Rect *, SInt16);
void DrawBall (SInt16, Rect *);
void DrawFish (SInt16, Rect *);
void DrawDrip (Rect *);
void DrawMirror (Rect *);
void DrawSimpleClutter (SInt16, Rect *);
void DrawFlower (Rect *, SInt16);
void DrawWallWindow (Rect *);
void DrawCalendar (Rect *);
void DrawBulletin (Rect *);
void DrawPictObject (SInt16, Rect *);
void DrawPictWithMaskObject (SInt16, Rect *);
void DrawPictSansWhiteObject (SInt16, Rect *);
void DrawCustPictSansWhite (SInt16, Rect *);

void DrawARoomsObjects (SInt16, Boolean);				// --- ObjectDrawAll.c

void DoSelectionClick (Point, Boolean);					// --- ObjectEdit.c
void DoNewObjectClick (Point);
void DeleteObject (void);
void DuplicateObject (void);
void MoveObject (SInt16, Boolean);
void DeselectObject (void);
Boolean ObjectHasHandle (SInt16 *, SInt16 *);
void HandleBlowerGlider (void);
void SelectNextObject (void);
void SelectPrevObject (void);
void GetThisRoomsObjRects (void);
void DrawThisRoomsObjects (void);
void HiliteAllObjects (void);
void GoToObjectInRoom (SInt16, SInt16, SInt16);
void GoToObjectInRoomNum (SInt16, SInt16);

void DoObjectInfo (void);								// --- ObjectInfo.c

void GetObjectRect (objectPtr, Rect *);					// --- ObjectRects.c
SInt16 CreateActiveRects (SInt16);
SInt16 VerticalRoomOffset (SInt16);
void OffsetRectRoomRelative (Rect *, SInt16);
SInt16 GetUpStairsRightEdge (void);
SInt16 GetDownStairsLeftEdge (void);

SInt16 GetRoomLinked (objectType *);						// --- Objects.c
Boolean ObjectIsLinkTransport (objectType *);
Boolean ObjectIsLinkSwitch (objectType *);
void ListAllLocalObjects (void);
Boolean SetObjectState (SInt16, SInt16, SInt16, SInt16);
Boolean GetObjectState (SInt16, SInt16);
void BringSendFrontBack (Boolean);
Boolean IsThisValid (SInt16, SInt16);
void AddTempManholeRect (Rect *);

void NewGame (SInt16);									// --- Play.c
void DoDemoGame (void);
void HideGlider (gliderPtr);
void StrikeChime (void);
void RestoreEntireGameScreen (void);

void HandleGlider (gliderPtr);							// --- Player.c
void FinishGliderUpStairs (gliderPtr);
void FinishGliderDownStairs (gliderPtr);
void FinishGliderDuctingIn (gliderPtr);
void DeckGliderInFoil (gliderPtr);
void RemoveFoilFromGlider (gliderPtr);
void OffsetGlider (gliderPtr, SInt16);
void OffAMortal (gliderPtr);

void AddRectToWorkRects (Rect *);						// --- Render.c
void AddRectToBackRects (Rect *);
void AddRectToWorkRectsWhole (Rect *);
void RenderGlider (gliderPtr, Boolean);
void CopyRectsQD (void);
void DirectWork2Main8 (Rect *);
void DirectBack2Work8 (Rect *);
void DirectGeneric2Work8 (long, long, Rect *, Rect *);
void DirectWork2Main4 (Rect *);
void DirectBack2Work4 (Rect *);
void DirectGeneric2Work4 (long, long, Rect *, Rect *);
void CopyRectsAssm (void);
void DirectFillBack8 (Rect *, Byte);
void DirectFillWork8 (Rect *, Byte);
void DirectFillBack4 (Rect *, Byte);
void DirectFillWork4 (Rect *, Byte);
void RenderFrame (void);
void InitGarbageRects (void);
void CopyRectBackToWork (Rect *);
void CopyRectWorkToBack (Rect *);
void CopyRectWorkToMain (Rect *);
void CopyRectMainToWork (Rect *);
void CopyRectMainToBack (Rect *);
void AddToMirrorRegion (Rect *);
void ZeroMirrorRegion (void);

void SetInitialTiles (SInt16, Boolean);					// --- Room.c
Boolean CreateNewRoom (SInt16, SInt16);
void DoRoomInfo (void);
void ReadyBackground (SInt16, SInt16 *);
void ReflectCurrentRoom (Boolean);
void CopyRoomToThisRoom (SInt16);
void CopyThisRoomToRoom (void);
void ForceThisRoom (SInt16);
Boolean RoomExists (SInt16, SInt16, SInt16 *);
Boolean RoomNumExists (SInt16);
void DeleteRoom (Boolean);
SInt16 DoesNeighborRoomExist (SInt16);
void SelectNeighborRoom (SInt16);
SInt16 GetNeighborRoomNumber (SInt16);
Boolean GetRoomFloorSuite (SInt16, SInt16 *, SInt16 *);
SInt16 GetRoomNumber (SInt16, SInt16);
Boolean	IsRoomAStructure (SInt16);
void DetermineRoomOpenings (void);
SInt16 GetOriginalBounding (SInt16);
SInt16 GetNumberOfLights (SInt16);
Boolean IsShadowVisible (void);
Boolean DoesRoomHaveFloor (void);
Boolean DoesRoomHaveCeiling (void);

void ReadyLevel (void);									// --- RoomGraphics.c
void DrawLocale (void);
void RedrawRoomLighting (void);

Boolean PictIDExists (SInt16);							// --- RoomInfo.c

void HandleBands (void);								// --- RubberBands.c
Boolean AddBand (gliderPtr, SInt16, SInt16, Boolean);
void KillAllBands (void);

void SaveGame2 (void);									// --- SavedGames.c
Boolean OpenSavedGame (void);
void SaveGame (Boolean);

void RefreshScoreboard (SInt16);							// --- Scoreboard.c
void HandleDynamicScoreboard (void);
void QuickGlidersRefresh (void);
void QuickScoreRefresh (void);
void QuickBatteryRefresh (Boolean);
void QuickBandsRefresh (Boolean);
void QuickFoilRefresh (Boolean);
void HandleScore (void);
void AdjustScoreboardHeight (void);
void BlackenScoreboard (void);

//void PutRoomScrap (void);								// --- Scrap.c
//void PutObjectScrap (void);
void GetRoomScrap (void);
void GetObjectScrap (void);
//void SeeIfValidScrapAvailable (Boolean);
Boolean HasDragManager (void);
//Boolean DragRoom (EventRecord *, Rect *, SInt16);

void DoLoadHouse (void);								// --- SelectHouse.c
void BuildHouseList (void);
void AddExtraHouse (FSSpec *);

void DoSettingsMain (void);								// --- Settings.c

void PlayPrioritySound (SInt16, SInt16);					// --- Sound.c
void FlushAnyTriggerPlaying (void);
void PlaySound0 (SInt16, SInt16);
void PlaySound1 (SInt16, SInt16);
void PlaySound2 (SInt16, SInt16);
OSErr LoadTriggerSound (SInt16);
void DumpTriggerSound (void);
void InitSound (void);
void KillSound (void);
long SoundBytesNeeded (void);
void TellHerNoSounds (void);
void BitchAboutSM3 (void);

void InitScoreboardMap (void);							// --- StructuresInit.c
void InitGliderMap (void);
void InitBlowers (void);
void InitFurniture (void);
void InitPrizes (void);
void InitTransports (void);
void InitSwitches (void);
void InitLights (void);
void InitAppliances (void);
void InitEnemies (void);

void CreateOffscreens (void);							// --- StructuresInit2.c
void CreatePointers (void);
void InitSrcRects (void);

void UpdateToolsWindow (void);							// --- Tools.c
void EraseSelectedTool (void);
void SelectTool (SInt16);
void OpenToolsWindow (void);
void CloseToolsWindow (void);
void ToggleToolsWindow (void);
void HandleToolsClick (Point);
void NextToolMode (void);
void PrevToolMode (void);
void SetSpecificToolMode (SInt16);

SInt16 WhatAreWeLinkedTo (SInt16, Byte);					// --- Transit.c
void ReadyGliderFromTransit (gliderPtr, SInt16);
void MoveRoomToRoom (gliderPtr, SInt16);
void TransportRoomToRoom (gliderPtr);
void MoveDuctToDuct (gliderPtr);
void MoveMailToMail (gliderPtr);
void ForceKillGlider (void);
void FollowTheLeader (void);

void PourScreenOn (Rect *);								// --- Transitions.c
void WipeScreenOn (SInt16, Rect *);
void DumpScreenOn (Rect *);
//void DissBits (Rect *);
//void DissBitsChunky (Rect *);
//void FillColorNoise (Rect *);
//void FillSnow (Rect *);

void ToggleToaster (SInt16);								// --- Trip.c
void ToggleMacPlus (SInt16);
void ToggleTV (SInt16);
void ToggleCoffee (SInt16);
void ToggleOutlet (SInt16);
void ToggleVCR (SInt16);
void ToggleStereos (SInt16);
void ToggleMicrowave (SInt16);
void ToggleBalloon (SInt16);
void ToggleCopter (SInt16);
void ToggleDart (SInt16);
void ToggleBall (SInt16);
void ToggleDrip (SInt16);
void ToggleFish (SInt16);
void TriggerSwitch (SInt16);
void TriggerToast (SInt16);
void TriggerOutlet (SInt16);
void TriggerDrip (SInt16);
void TriggerFish (SInt16);
void TriggerBalloon (SInt16);
void TriggerCopter (SInt16);
void TriggerDart (SInt16);
void UpdateOutletsLighting (SInt16, SInt16);

void ArmTrigger (hotPtr);								// --- Triggers.c
void HandleTriggers (void);
void ZeroTriggers (void);

