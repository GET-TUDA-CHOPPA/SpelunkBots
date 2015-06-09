#include "stdafx.h"
#include "IBot.h"

IBot::IBot()
{
	InitialiseDLLFunctions();
	InitialiseVariables();
}

IBot::~IBot()
{

}

void IBot::InitialiseVariables()
{
	_targetX = 0;
	_targetY = 0;
	_pathCount = 0;
	_tempID = 0;
	_waitTimer = 0;
	_playerPositionX = 0;
	_playerPositionY = 0;
	_playerPositionXNode = 0;
	_playerPositionYNode = 0;
	_hasGoal = false;
	_spIsInAir = false;
	_spIsJetpacking = false;
	_itemGoal = false;
	_fogGoal = true;
	_endGoal = false;
	_headingRight = false;
	_headingLeft = false;
	_goRight = false;
	_goLeft = false;
	_jump = false;
	_attack = false;
}

void IBot::InitialiseDLLFunctions()
{
	spelunkbots_hModule = LoadLibrary(TEXT("spelunkbots.dll"));

	UpdatePlayerVariables = (UpdatePlayerVariablesPROC)GetProcAddress(spelunkbots_hModule, "UpdatePlayerVariables");
	DisplayPlayerVariables = (DisplayPlayerVariablesPROC)GetProcAddress(spelunkbots_hModule, "DisplayMessages");
	SetScreenXYWH = (SetScreenXYWHPROC)GetProcAddress(spelunkbots_hModule, "SetScreenXYWH");
	SetCoolGlasses = (SetCoolGlassesPROC)GetProcAddress(spelunkbots_hModule, "SetCoolGlasses");
	SetUdjatEye = (SetUdjatEyePROC)GetProcAddress(spelunkbots_hModule, "SetUdjatEye");
	ResetFogForNewLevel = (ResetFogForNewLevelPROC)GetProcAddress(spelunkbots_hModule, "ResetFogForNewLevel");
	TerrainWasDestroyed = (TerrainWasDestroyedPROC)GetProcAddress(spelunkbots_hModule, "TerrainWasDestroyed");
	SpiderWebCreated = (SpiderWebCreatedPROC)GetProcAddress(spelunkbots_hModule, "SpiderWebCreated");
	SpiderWebWasRemoved = (SpiderWebWasRemovedPROC)GetProcAddress(spelunkbots_hModule, "SpiderWebWasRemoved");
	DestroyFog = (DestroyFogPROC)GetProcAddress(spelunkbots_hModule, "DestroyFog");
	TerrainIsLadder = (TerrainIsLadderPROC)GetProcAddress(spelunkbots_hModule, "TerrainIsLadder");
	TerrainIsGoal = (TerrainIsGoalPROC)GetProcAddress(spelunkbots_hModule, "TerrainIsGoal");
	TerrainIsStart = (TerrainIsStartPROC)GetProcAddress(spelunkbots_hModule, "TerrainIsStart");
	TerrainIsAltar = (TerrainIsAltarPROC)GetProcAddress(spelunkbots_hModule, "TerrainIsAltar");
	TerrainIsArrowTrapRight = (TerrainIsArrowTrapRightPROC)GetProcAddress(spelunkbots_hModule, "TerrainIsArrowTrapRight");
	TerrainIsArrowTrapLeft = (TerrainIsArrowTrapLeftPROC)GetProcAddress(spelunkbots_hModule, "TerrainIsArrowTrapLeft");
	TerrainIsShopKeeperArea = (TerrainIsShopKeeperAreaPROC)GetProcAddress(spelunkbots_hModule, "TerrainIsShopKeeperArea");
	TerrainIsIce = (TerrainIsIcePROC)GetProcAddress(spelunkbots_hModule, "TerrainIsIce");
	TerrainIsSpike = (TerrainIsSpikePROC)GetProcAddress(spelunkbots_hModule, "TerrainIsSpike");
	TerrainIsSpearTrap = (TerrainIsSpearTrapPROC)GetProcAddress(spelunkbots_hModule, "TerrainIsSpearTrap");
	TerrainIsSwimWater = (TerrainIsSwimWaterPROC)GetProcAddress(spelunkbots_hModule, "TerrainIsSwimWater");
	TerrainIsLava = (TerrainIsLavaPROC)GetProcAddress(spelunkbots_hModule, "TerrainIsLava");
	TerrainIsGameEntrance = (TerrainIsGameEntrancePROC)GetProcAddress(spelunkbots_hModule, "TerrainIsGameEntrance");
	TerrainIsTree = (TerrainIsTreePROC)GetProcAddress(spelunkbots_hModule, "TerrainIsTree");
	TerrainIsTreeBranchLeaf = (TerrainIsTreeBranchLeafPROC)GetProcAddress(spelunkbots_hModule, "TerrainIsTreeBranchLeaf");
	TerrainIsEmpty = (TerrainIsEmptyPROC)GetProcAddress(spelunkbots_hModule, "TerrainIsEmpty");
	GetLiquidValue = (GetLiquidValuePROC)GetProcAddress(spelunkbots_hModule, "GetLiquidValue");
	SpringPadAtPosition = (SpringPadAtPositionPROC)GetProcAddress(spelunkbots_hModule, "SpringPadAtPosition");
	FillShopkeeperArea = (FillShopkeeperAreaPROC)GetProcAddress(spelunkbots_hModule, "FillShopkeeperArea");
	SetMapCoord = (SetMapCoordPROC)GetProcAddress(spelunkbots_hModule, "SetMapCoord");
	ClearFogFromSquare = (ClearFogFromSquarePROC)GetProcAddress(spelunkbots_hModule, "ClearFogFromSquare");
	GetNodeState = (GetNodeStatePROC)GetProcAddress(spelunkbots_hModule, "GetNodeState");
	GetFogState = (GetFogStatePROC)GetProcAddress(spelunkbots_hModule, "GetFogState");
	IsClearPathToExit = (IsClearPathToExitPROC)GetProcAddress(spelunkbots_hModule, "IsClearPathToExit");
	ClearDynamicObjects = (ClearDynamicObjectsPROC)GetProcAddress(spelunkbots_hModule, "ClearDynamicObjects");
	NodeContainsPushBlock = (NodeContainsPushBlockPROC)GetProcAddress(spelunkbots_hModule, "NodeContainsPushBlock");
	GetNodeContainsPushBlock = (GetNodeContainsPushBlockPROC)GetProcAddress(spelunkbots_hModule, "GetNodeContainsPushBlock");
	NodeContainsBat = (NodeContainsBatPROC)GetProcAddress(spelunkbots_hModule, "NodeContainsBat");
	GetNodeContainsBat = (GetNodeContainsBatPROC)GetProcAddress(spelunkbots_hModule, "GetNodeContainsBat");
	ResetCollectables = (ResetCollectablesPROC)GetProcAddress(spelunkbots_hModule, "ResetCollectables");
	NodeContainsCollectable = (NodeContainsCollectablePROC)GetProcAddress(spelunkbots_hModule, "NodeContainsCollectable");
	UpdateCollectableAtNode = (UpdateCollectableAtNodePROC)GetProcAddress(spelunkbots_hModule, "UpdateCollectableAtNode");
	RemoveCollectableWithID = (RemoveCollectableWithIDPROC)GetProcAddress(spelunkbots_hModule, "RemoveCollectableWithID");
	NumberOfCollectableTypeInNode = (NumberOfCollectableTypeInNodePROC)GetProcAddress(spelunkbots_hModule, "NumberOfCollectableTypeInNode");
	ResetEnemies = (ResetEnemiesPROC)GetProcAddress(spelunkbots_hModule, "SetScResetEnemiesreenXYWH");
	NodeContainsEnemy = (NodeContainsEnemyPROC)GetProcAddress(spelunkbots_hModule, "NodeContainsEnemy");
	UpdateEnemyAtNode = (UpdateEnemyAtNodePROC)GetProcAddress(spelunkbots_hModule, "UpdateEnemyAtNode");
	RemoveEnemyWithID = (RemoveEnemyWithIDPROC)GetProcAddress(spelunkbots_hModule, "RemoveEnemyWithID");
	NumberOfWebsInNode = (NumberOfWebsInNodePROC)GetProcAddress(spelunkbots_hModule, "NumberOfWebsInNode");
	NumberOfEnemyTypeInNode = (NumberOfEnemyTypeInNodePROC)GetProcAddress(spelunkbots_hModule, "NumberOfEnemyTypeInNode");
	GetIDOfEnemyInNode = (GetIDOfEnemyInNodePROC)GetProcAddress(spelunkbots_hModule, "GetIDOfEnemyInNode");
	GetIDOfCollectableInNode = (GetIDOfCollectableInNodePROC)GetProcAddress(spelunkbots_hModule, "GetIDOfCollectableInNode");
	SaveDynamicObjectFilesDebug = (SaveDynamicObjectFilesDebugPROC)GetProcAddress(spelunkbots_hModule, "SaveDynamicObjectFilesDebug");
	SaveSpiderwebsToFile = (SaveSpiderwebsToFilePROC)GetProcAddress(spelunkbots_hModule, "SaveSpiderwebsToFile");
	SaveLevelLayoutToFile = (SaveLevelLayoutToFilePROC)GetProcAddress(spelunkbots_hModule, "SetScrSaveLevelLayoutToFileeenXYWH");
	CalculatePathFromXYtoXY = (CalculatePathFromXYtoXYPROC)GetProcAddress(spelunkbots_hModule, "CalculatePathFromXYtoXY");
	GetNextPathXPos = (GetNextPathXPosPROC)GetProcAddress(spelunkbots_hModule, "GetNextPathXPos");
	GetNextPathYPos = (GetNextPathYPosPROC)GetProcAddress(spelunkbots_hModule, "GetNextPathYPos");
	IsEnemyInNode = (IsEnemyInNodePROC)GetProcAddress(spelunkbots_hModule, "IsEnemyInNode");
	IsCollectableInNode = (IsCollectableInNodePROC)GetProcAddress(spelunkbots_hModule, "IsCollectableInNode");
	IsNodePassable = (IsNodePassablePROC)GetProcAddress(spelunkbots_hModule, "IsNodePassable");
}

void IBot::Reset()
{
	_headingRight = false;
	_headingLeft = false;
	_goRight = false;
	_goLeft = false;
	_jump = false;
	_attack = false;
}

void IBot::UpdateBotPosition(double nodeX, double nodeY)
{
	_playerPositionX = nodeX * PIXELS_IN_NODES;
	_playerPositionY = nodeY * PIXELS_IN_NODES;

	_playerPositionXNode = nodeX;
	_playerPositionYNode = nodeY;
}