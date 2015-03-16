// SpelunkBot.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Bot.h"

#pragma region DEFINES

// Use for functions that take either pixel or node coordinates
#define NODE_COORDS 0
#define PIXEL_COORDS 1

// Nodes in the x and y axes
#define Y_NODES 34
#define X_NODES 42

// Number of pixels in each node
#define PIXELS_IN_NODES 16

// Variable types - useful for when calling UpdatePlayerVariables()
#define BOOLEAN 0
#define DOUBLE 1
#define STRING 2

#pragma endregion

SPELUNKBOT_API double Initialise(void)
{
	_targetX = 0;
	_targetY = 0;
	_pathCount = 0;
	_tempID = 0;
	_waitTimer = 0;
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
	return 1;
}

SPELUNKBOT_API double Update(double botXPos, double botYPos)
{
	// Sample bot

	_playerPositionX = botXPos;
	_playerPositionY = botYPos;
	_playerPositionXNode = botXPos * PIXELS_IN_NODES;
	_playerPositionYNode = botYPos * PIXELS_IN_NODES;

	if (!_hasGoal)
	{
		for (int y = 0; y < Y_NODES; y++)
		{
			for (int x = 0; x < X_NODES; x++)
			{
				if (GetNodeState(x, y, NODE_COORDS) == spExit)
				{
					_hasGoal = true;
					_itemGoal = true;
					_targetX = x * PIXELS_IN_NODES;
					_targetY = y * PIXELS_IN_NODES;
					CalculatePathFromXYtoXY(_playerPositionXNode, _playerPositionYNode, _targetX, _targetY, PIXEL_COORDS);
					std::cout << "FOUND EXIT" << std::endl;
					return 0;
				}
			}
		}
	}
	
	if (botXPos < (GetNextPathXPos(botXPos, botYPos, NODE_COORDS)))
	{
		_headingRight = _goRight = true;
		_headingLeft = _goLeft = false;
	}
	else
	{
		_headingRight = _goRight = false;
		_headingLeft = _goLeft = true;
	}
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
char* ConvertBoolToChar(bool valToConvert)
{
	if (valToConvert)
	{
		return "1";
	}
	return "0";
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
SPELUNKBOT_API double GetTargetX(void)
{
	return _targetX;
}
SPELUNKBOT_API double GetTargetY(void)
{
	return _targetY;
}
#pragma endregion