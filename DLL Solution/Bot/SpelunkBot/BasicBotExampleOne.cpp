#include "stdafx.h"
#include "BasicBotExampleOne.h"

/*
	BasicBotExampleOne is a very simple bot. If the exit is within sight when the bot enters a
	room it will move towards it. However, if the exit is not within sight, the bot does nothing.
*/
void BasicBotExampleOne::Update()
{
	if (!_hasGoal)
	{
		// If no goal, search for the exit
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
					CalculatePathFromXYtoXY(_playerPositionX, _playerPositionY, _targetX, _targetY, PIXEL_COORDS);
					std::cout << "FOUND EXIT" << std::endl;
				}
			}
		}
	}
	else
	{
		// If goal, move towards it
		if (_playerPositionXNode < (GetNextPathXPos(_playerPositionXNode, _playerPositionYNode, NODE_COORDS)))
		{
			_goRight = true;
		}
		else
		{
			_goLeft = true;
		}
	}
}