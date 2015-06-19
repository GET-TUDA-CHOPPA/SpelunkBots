// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SPELUNKBOT_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SPELUNKBOT_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#pragma once

#include "stdafx.h"

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
	Used to initialise the bot.
*/
void CreateBot(double botSelector);

/*
	Initialise is used to setup any variables when a bot enters a room.

	Add any additional variable initialisation here.
*/
SPELUNKBOT_API double Initialise(void);
/*
	Update is used to update the bot position and chose which bot function to use.
*/
SPELUNKBOT_API double Update(double botSelector, double botXPos, double botYPos);
/*
	NewLevel is called whenever the bot enters a new level, and can be used to reset known data about
	the previous level without resetting all the bots' varables. 
*/
SPELUNKBOT_API double NewLevel(void);
/*
	Getter functions for variables.

	Add additional functions as required.
*/
SPELUNKBOT_API double GetHasGoal(void);
SPELUNKBOT_API double GetHoldingItem(void);
SPELUNKBOT_API double GetIsInAir(void);
SPELUNKBOT_API double GetIsJetpacking(void);
SPELUNKBOT_API double GetIsHanging(void);
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
SPELUNKBOT_API double GetLookUp(void);
SPELUNKBOT_API double GetDuck(void);
SPELUNKBOT_API double GetHanging(void);
SPELUNKBOT_API double GetRopep(void);

#pragma endregion