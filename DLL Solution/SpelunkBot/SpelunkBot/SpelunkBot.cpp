// SpelunkBot.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "SpelunkBot.h"

SPELUNKBOT_API double Initialise(void)
{
	_hasGoal = false;
	_spIsInAir = false;
	_spIsJetpacking = false;
	_itemGoal = false;
	_pathCount = 0;
	_tempID = 0;
	_fogGoal = true;
	_endGoal = false;
	_waitTimer = 0;
	_headingRight = false;
	_headingLeft = false;
	_goRight = false;
	_goLeft = false;
	_jump = false;
	return 1;
}

SPELUNKBOT_API double Update()
{
	_headingLeft = true;
	_goLeft = true;
	return 1;
}

#pragma region Get functions for GM
double ConvertBoolToDouble(bool valToConvert)
{
	if (valToConvert)
	{
		return 1;
	}
	return 0;
}
SPELUNKBOT_API double GetHasGoal(void)
{
	return ConvertBoolToDouble(_hasGoal);
}
SPELUNKBOT_API double GetIsInAir(void)
{
	return ConvertBoolToDouble(_spIsInAir);
}
SPELUNKBOT_API double GetIsJetpacking(void)
{
	return ConvertBoolToDouble(_spIsJetpacking);
}
SPELUNKBOT_API double GetItemGoal(void)
{
	return ConvertBoolToDouble(_itemGoal);
}
SPELUNKBOT_API double GetPathCount(void)
{
	return _pathCount;
}
SPELUNKBOT_API double GetTempID(void)
{
	return _tempID;
}
SPELUNKBOT_API double GetFogGoal(void)
{
	return ConvertBoolToDouble(_fogGoal);
}
SPELUNKBOT_API double GetEndGoal(void)
{
	return ConvertBoolToDouble(_endGoal);
}
SPELUNKBOT_API double GetWaitTimer(void)
{
	return _waitTimer;
}
SPELUNKBOT_API double GetHeadingRight(void)
{
	return ConvertBoolToDouble(_headingRight);
}
SPELUNKBOT_API double GetHeadingLeft(void)
{
	return ConvertBoolToDouble(_headingLeft);
}
SPELUNKBOT_API double GetGoRight(void)
{
	return ConvertBoolToDouble(_goRight);
}
SPELUNKBOT_API double GetGoLeft(void)
{
	return ConvertBoolToDouble(_goLeft);
}
SPELUNKBOT_API double GetJump(void)
{
	return ConvertBoolToDouble(_jump);
}
#pragma endregion