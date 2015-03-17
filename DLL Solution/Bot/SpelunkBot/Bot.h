// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SPELUNKBOT_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SPELUNKBOT_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SPELUNKBOT_EXPORTS
#define SPELUNKBOT_API extern "C" __declspec(dllexport)
#else
#define SPELUNKBOT_API extern "C" __declspec(dllimport)
#endif

#pragma region Function Declarations

/*
	All DLL functions to be called from Game Maker need to return a string or a double.

	Use these functions to convert values are required.
*/
double ConvertBoolToDouble(bool valToConvert);
char* ConvertBoolToChar(bool valToConvert);

/*
	ResetBotVariables is used to reset any bot variables e.g. _goLeft or _goRight
*/
void ResetBotVariables(void);

/*
	Initialise is used to setup any variables when a bot enters a room.

	Add any additional variable initialisation here.
*/
SPELUNKBOT_API double Initialise(void);
/*
	Update contains the main logic for a C++ bot. 
	
	Use this function to perform the logic for your bot.
*/
SPELUNKBOT_API double Update(double botXPos, double botYPos);
/*
	Getter functions for variables.

	Add additional functions as required.
*/
SPELUNKBOT_API double GetHasGoal(void);
SPELUNKBOT_API double GetIsInAir(void);
SPELUNKBOT_API double GetIsJetpacking(void);
SPELUNKBOT_API double GetItemGoal(void);
SPELUNKBOT_API double GetPathCount(void);
SPELUNKBOT_API double GetTempID(void);
SPELUNKBOT_API double GetFogGoal(void);
SPELUNKBOT_API double GetEndGoal(void);
SPELUNKBOT_API double GetWaitTimer(void);
SPELUNKBOT_API double GetHeadingRight(void);
SPELUNKBOT_API double GetHeadingLeft(void);
SPELUNKBOT_API double GetGoRight(void);
SPELUNKBOT_API double GetGoLeft(void);
SPELUNKBOT_API double GetJump(void);
SPELUNKBOT_API double GetTargetX(void);
SPELUNKBOT_API double GetTargetY(void);
SPELUNKBOT_API double GetAttack(void);

#pragma endregion

#pragma region Variables

double _pathCount;
double _tempID;
double _waitTimer;
double _targetX;
double _targetY;
double _playerPositionX;
double _playerPositionY;
double _playerPositionXNode;
double _playerPositionYNode;
bool _hasGoal;
bool _spIsInAir;
bool _spIsJetpacking;
bool _itemGoal;
bool _fogGoal;
bool _endGoal;
bool _headingRight;
bool _headingLeft;
bool _goRight;
bool _goLeft;
bool _jump;
bool _attack;

#pragma endregion

#pragma region API Imports

HMODULE spelunkbots_hModule = LoadLibrary(TEXT("spelunkbots.dll"));

typedef double(__cdecl *UpdatePlayerVariablesPROC)(char *name, char *value, double type);
UpdatePlayerVariablesPROC UpdatePlayerVariables = (UpdatePlayerVariablesPROC)GetProcAddress(spelunkbots_hModule, "UpdatePlayerVariables");

typedef double(__cdecl * DisplayPlayerVariablesPROC)();
DisplayPlayerVariablesPROC DisplayPlayerVariables = (DisplayPlayerVariablesPROC)GetProcAddress(spelunkbots_hModule, "DisplayMessages");

typedef double(__cdecl *SetScreenXYWHPROC)(double x, double y, double w, double h);
SetScreenXYWHPROC SetScreenXYWH = (SetScreenXYWHPROC)GetProcAddress(spelunkbots_hModule, "SetScreenXYWH");

typedef double(__cdecl *SetCoolGlassesPROC)(double d);
SetCoolGlassesPROC SetCoolGlasses = (SetCoolGlassesPROC)GetProcAddress(spelunkbots_hModule, "SetCoolGlasses");

typedef double(__cdecl *SetUdjatEyePROC)(double d);
SetUdjatEyePROC SetUdjatEye = (SetUdjatEyePROC)GetProcAddress(spelunkbots_hModule, "SetUdjatEye");

typedef double(__cdecl *ResetFogForNewLevelPROC)();
ResetFogForNewLevelPROC ResetFogForNewLevel = (ResetFogForNewLevelPROC)GetProcAddress(spelunkbots_hModule, "ResetFogForNewLevel");

typedef double(__cdecl *TerrainWasDestroyedPROC)(double x, double y);
TerrainWasDestroyedPROC TerrainWasDestroyed = (TerrainWasDestroyedPROC)GetProcAddress(spelunkbots_hModule, "TerrainWasDestroyed");

typedef double(__cdecl *SpiderWebCreatedPROC)(double x, double y);
SpiderWebCreatedPROC SpiderWebCreated = (SpiderWebCreatedPROC)GetProcAddress(spelunkbots_hModule, "SpiderWebCreated");

typedef double(__cdecl *SpiderWebWasRemovedPROC)(double x, double y);
SpiderWebWasRemovedPROC SpiderWebWasRemoved = (SpiderWebWasRemovedPROC)GetProcAddress(spelunkbots_hModule, "SpiderWebWasRemoved");

typedef double(__cdecl *DestroyFogPROC)(double x, double y);
DestroyFogPROC DestroyFog = (DestroyFogPROC)GetProcAddress(spelunkbots_hModule, "DestroyFog");

typedef double(__cdecl *TerrainIsLadderPROC)(double x, double y);
TerrainIsLadderPROC TerrainIsLadder = (TerrainIsLadderPROC)GetProcAddress(spelunkbots_hModule, "TerrainIsLadder");

typedef double(__cdecl *TerrainIsGoalPROC)(double x, double y);
TerrainIsGoalPROC TerrainIsGoal = (TerrainIsGoalPROC)GetProcAddress(spelunkbots_hModule, "TerrainIsGoal");

typedef double(__cdecl *TerrainIsStartPROC)(double x, double y);
TerrainIsStartPROC TerrainIsStart = (TerrainIsStartPROC)GetProcAddress(spelunkbots_hModule, "TerrainIsStart");

typedef double(__cdecl *TerrainIsAltarPROC)(double x, double y);
TerrainIsAltarPROC TerrainIsAltar = (TerrainIsAltarPROC)GetProcAddress(spelunkbots_hModule, "TerrainIsAltar");

typedef double(__cdecl *TerrainIsArrowTrapRightPROC)(double x, double y);
TerrainIsArrowTrapRightPROC TerrainIsArrowTrapRight = (TerrainIsArrowTrapRightPROC)GetProcAddress(spelunkbots_hModule, "TerrainIsArrowTrapRight");

typedef double(__cdecl *TerrainIsArrowTrapLeftPROC)(double x, double y);
TerrainIsArrowTrapLeftPROC TerrainIsArrowTrapLeft = (TerrainIsArrowTrapLeftPROC)GetProcAddress(spelunkbots_hModule, "TerrainIsArrowTrapLeft");

typedef double(__cdecl *TerrainIsShopKeeperAreaPROC)(double x, double y);
TerrainIsShopKeeperAreaPROC TerrainIsShopKeeperArea = (TerrainIsShopKeeperAreaPROC)GetProcAddress(spelunkbots_hModule, "TerrainIsShopKeeperArea");

typedef double(__cdecl *TerrainIsIcePROC)(double x, double y);
TerrainIsIcePROC TerrainIsIce = (TerrainIsIcePROC)GetProcAddress(spelunkbots_hModule, "TerrainIsIce");

typedef double(__cdecl *TerrainIsSpikePROC)(double x, double y);
TerrainIsSpikePROC TerrainIsSpike = (TerrainIsSpikePROC)GetProcAddress(spelunkbots_hModule, "TerrainIsSpike");

typedef double(__cdecl *TerrainIsSpearTrapPROC)(double x, double y);
TerrainIsSpearTrapPROC TerrainIsSpearTrap = (TerrainIsSpearTrapPROC)GetProcAddress(spelunkbots_hModule, "TerrainIsSpearTrap");

typedef double(__cdecl *TerrainIsSwimWaterPROC)(double x, double y);
TerrainIsSwimWaterPROC TerrainIsSwimWater = (TerrainIsSwimWaterPROC)GetProcAddress(spelunkbots_hModule, "TerrainIsSwimWater");

typedef double(__cdecl *TerrainIsLavaPROC)(double x, double y);
TerrainIsLavaPROC TerrainIsLava = (TerrainIsLavaPROC)GetProcAddress(spelunkbots_hModule, "TerrainIsLava");

typedef double(__cdecl *TerrainIsGameEntrancePROC)(double x, double y);
TerrainIsGameEntrancePROC TerrainIsGameEntrance = (TerrainIsGameEntrancePROC)GetProcAddress(spelunkbots_hModule, "TerrainIsGameEntrance");

typedef double(__cdecl *TerrainIsTreePROC)(double x, double y);
TerrainIsTreePROC TerrainIsTree = (TerrainIsTreePROC)GetProcAddress(spelunkbots_hModule, "TerrainIsTree");

typedef double(__cdecl *TerrainIsTreeBranchLeafPROC)(double x, double y);
TerrainIsTreeBranchLeafPROC TerrainIsTreeBranchLeaf = (TerrainIsTreeBranchLeafPROC)GetProcAddress(spelunkbots_hModule, "TerrainIsTreeBranchLeaf");

typedef double(__cdecl *TerrainIsEmptyPROC)(double x, double y);
TerrainIsEmptyPROC TerrainIsEmpty = (TerrainIsEmptyPROC)GetProcAddress(spelunkbots_hModule, "TerrainIsEmpty");

typedef double(__cdecl *GetLiquidValuePROC)(double x, double y);
GetLiquidValuePROC GetLiquidValue = (GetLiquidValuePROC)GetProcAddress(spelunkbots_hModule, "GetLiquidValue");

typedef double(__cdecl *SpringPadAtPositionPROC)(double x, double y);
SpringPadAtPositionPROC SpringPadAtPosition = (SpringPadAtPositionPROC)GetProcAddress(spelunkbots_hModule, "SpringPadAtPosition");

typedef double(__cdecl *FillShopkeeperAreaPROC)(double x, double y);
FillShopkeeperAreaPROC FillShopkeeperArea = (FillShopkeeperAreaPROC)GetProcAddress(spelunkbots_hModule, "FillShopkeeperArea");

typedef double(__cdecl *SetMapCoordPROC)(double x, double y, double state);
SetMapCoordPROC SetMapCoord = (SetMapCoordPROC)GetProcAddress(spelunkbots_hModule, "SetMapCoord");

typedef double(__cdecl *ClearFogFromSquarePROC)(double x, double y);
ClearFogFromSquarePROC ClearFogFromSquare = (ClearFogFromSquarePROC)GetProcAddress(spelunkbots_hModule, "ClearFogFromSquare");

typedef double(__cdecl *GetNodeStatePROC)(double x, double y, double usingPixelCoords);
GetNodeStatePROC GetNodeState = (GetNodeStatePROC)GetProcAddress(spelunkbots_hModule, "GetNodeState");

typedef double(__cdecl *GetFogStatePROC)(double x, double y, double usingPixelCoords);
GetFogStatePROC GetFogState = (GetFogStatePROC)GetProcAddress(spelunkbots_hModule, "GetFogState");

typedef bool(__cdecl *IsClearPathToExitPROC)();
IsClearPathToExitPROC IsClearPathToExit = (IsClearPathToExitPROC)GetProcAddress(spelunkbots_hModule, "IsClearPathToExit");

typedef double(__cdecl *ClearDynamicObjectsPROC)();
ClearDynamicObjectsPROC ClearDynamicObjects = (ClearDynamicObjectsPROC)GetProcAddress(spelunkbots_hModule, "ClearDynamicObjects");

typedef double(__cdecl *NodeContainsPushBlockPROC)(double x, double y);
NodeContainsPushBlockPROC NodeContainsPushBlock = (NodeContainsPushBlockPROC)GetProcAddress(spelunkbots_hModule, "NodeContainsPushBlock");

typedef double(__cdecl *GetNodeContainsPushBlockPROC)(double x, double y, double usingPixelCoords);
GetNodeContainsPushBlockPROC GetNodeContainsPushBlock = (GetNodeContainsPushBlockPROC)GetProcAddress(spelunkbots_hModule, "GetNodeContainsPushBlock");

typedef double(__cdecl *NodeContainsBatPROC)(double x, double y);
NodeContainsBatPROC NodeContainsBat = (NodeContainsBatPROC)GetProcAddress(spelunkbots_hModule, "NodeContainsBat");

typedef double(__cdecl *GetNodeContainsBatPROC)(double x, double y, double usingPixelCOords);
GetNodeContainsBatPROC GetNodeContainsBat = (GetNodeContainsBatPROC)GetProcAddress(spelunkbots_hModule, "GetNodeContainsBat");

typedef double(__cdecl *ResetCollectablesPROC)();
ResetCollectablesPROC ResetCollectables = (ResetCollectablesPROC)GetProcAddress(spelunkbots_hModule, "ResetCollectables");

typedef double(__cdecl *NodeContainsCollectablePROC)(double x, double y, double type, double id);
NodeContainsCollectablePROC NodeContainsCollectable = (NodeContainsCollectablePROC)GetProcAddress(spelunkbots_hModule, "NodeContainsCollectable");

typedef double(__cdecl *UpdateCollectableAtNodePROC)(double x, double y, double id);
UpdateCollectableAtNodePROC UpdateCollectableAtNode = (UpdateCollectableAtNodePROC)GetProcAddress(spelunkbots_hModule, "UpdateCollectableAtNode");

typedef double(__cdecl *RemoveCollectableWithIDPROC)(double id);
RemoveCollectableWithIDPROC RemoveCollectableWithID = (RemoveCollectableWithIDPROC)GetProcAddress(spelunkbots_hModule, "RemoveCollectableWithID");

typedef double(__cdecl *NumberOfCollectableTypeInNodePROC)(double type, double x, double y, double usingPixelCoords);
NumberOfCollectableTypeInNodePROC NumberOfCollectableTypeInNode = (NumberOfCollectableTypeInNodePROC)GetProcAddress(spelunkbots_hModule, "NumberOfCollectableTypeInNode");

typedef double(__cdecl *ResetEnemiesPROC)();
ResetEnemiesPROC ResetEnemies = (ResetEnemiesPROC)GetProcAddress(spelunkbots_hModule, "SetScResetEnemiesreenXYWH");

typedef double(__cdecl *NodeContainsEnemyPROC)(double x, double y, double type, double id);
NodeContainsEnemyPROC NodeContainsEnemy = (NodeContainsEnemyPROC)GetProcAddress(spelunkbots_hModule, "NodeContainsEnemy");

typedef double(__cdecl *UpdateEnemyAtNodePROC)(double x, double y, double id);
UpdateEnemyAtNodePROC UpdateEnemyAtNode = (UpdateEnemyAtNodePROC)GetProcAddress(spelunkbots_hModule, "UpdateEnemyAtNode");

typedef double(__cdecl *RemoveEnemyWithIDPROC)(double id);
RemoveEnemyWithIDPROC RemoveEnemyWithID = (RemoveEnemyWithIDPROC)GetProcAddress(spelunkbots_hModule, "RemoveEnemyWithID");

typedef double(__cdecl *NumberOfWebsInNodePROC)(double x, double y, double usingPixelCoords);
NumberOfWebsInNodePROC NumberOfWebsInNode = (NumberOfWebsInNodePROC)GetProcAddress(spelunkbots_hModule, "NumberOfWebsInNode");

typedef double(__cdecl *NumberOfEnemyTypeInNodePROC)(double type, double x, double y, double usingPixelCoords);
NumberOfEnemyTypeInNodePROC NumberOfEnemyTypeInNode = (NumberOfEnemyTypeInNodePROC)GetProcAddress(spelunkbots_hModule, "NumberOfEnemyTypeInNode");

typedef double(__cdecl *GetIDOfEnemyInNodePROC)(double type, double x, double y, double usingPixelCoords);
GetIDOfEnemyInNodePROC GetIDOfEnemyInNode = (GetIDOfEnemyInNodePROC)GetProcAddress(spelunkbots_hModule, "GetIDOfEnemyInNode");

typedef double(__cdecl *GetIDOfCollectableInNodePROC)(double type, double x, double y, double usingPixelCoords);
GetIDOfCollectableInNodePROC GetIDOfCollectableInNode = (GetIDOfCollectableInNodePROC)GetProcAddress(spelunkbots_hModule, "GetIDOfCollectableInNode");

typedef double(__cdecl *SaveDynamicObjectFilesDebugPROC)();
SaveDynamicObjectFilesDebugPROC SaveDynamicObjectFilesDebug = (SaveDynamicObjectFilesDebugPROC)GetProcAddress(spelunkbots_hModule, "SaveDynamicObjectFilesDebug");

typedef double(__cdecl *SaveSpiderwebsToFilePROC)();
SaveSpiderwebsToFilePROC SaveSpiderwebsToFile = (SaveSpiderwebsToFilePROC)GetProcAddress(spelunkbots_hModule, "SaveSpiderwebsToFile");

typedef double(__cdecl *SaveLevelLayoutToFilePROC)();
SaveLevelLayoutToFilePROC SaveLevelLayoutToFile = (SaveLevelLayoutToFilePROC)GetProcAddress(spelunkbots_hModule, "SetScrSaveLevelLayoutToFileeenXYWH");

typedef double(__cdecl *CalculatePathFromXYtoXYPROC)(double x1, double y1, double x2, double y2, double usingPixelCoords);
CalculatePathFromXYtoXYPROC CalculatePathFromXYtoXY = (CalculatePathFromXYtoXYPROC)GetProcAddress(spelunkbots_hModule, "CalculatePathFromXYtoXY");

typedef double(__cdecl *GetNextPathXPosPROC)(double x, double y, double usingPixelCoords);
GetNextPathXPosPROC GetNextPathXPos = (GetNextPathXPosPROC)GetProcAddress(spelunkbots_hModule, "GetNextPathXPos");

typedef double(__cdecl *GetNextPathYPosPROC)(double x, double y, double usingPixelCoords);
GetNextPathYPosPROC GetNextPathYPos = (GetNextPathYPosPROC)GetProcAddress(spelunkbots_hModule, "GetNextPathYPos");

typedef double(__cdecl *IsEnemyInNodePROC)(double x, double y, double usingPixelCoords);
IsEnemyInNodePROC IsEnemyInNode = (IsEnemyInNodePROC)GetProcAddress(spelunkbots_hModule, "IsEnemyInNode");

typedef double(__cdecl *IsCollectableInNodePROC)(double x, double y, double usingPixelCoords);
IsCollectableInNodePROC IsCollectableInNode = (IsCollectableInNodePROC)GetProcAddress(spelunkbots_hModule, "IsCollectableInNode");

typedef double(__cdecl *IsNodePassablePROC)(double x, double y, double usingPixelCoords);
IsNodePassablePROC IsNodePassable = (IsNodePassablePROC)GetProcAddress(spelunkbots_hModule, "IsNodePassable");

#pragma endregion