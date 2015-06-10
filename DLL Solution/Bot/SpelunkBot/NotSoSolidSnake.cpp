#include "stdafx.h"
#include "NotSoSolidSnake.h"

void NotSoSolidSnake::Update()
{
	if (!_hasGoal)
	{
		for (int nodeX = 0; nodeX < X_NODES; nodeX += 1)
		{
			for (int nodeY = 0; nodeY < Y_NODES; nodeY += 1)
			{
				if (NumberOfEnemyTypeInNode(spSnake, nodeX, nodeY, PIXEL_COORDS))
				{
					_targetX = nodeX * PIXELS_IN_NODES;
					_targetY = (nodeY - 1) * PIXELS_IN_NODES;
					_hasGoal = true;
					CalculatePathFromXYtoXY(_playerPositionX, _playerPositionY, _targetX, _targetY, PIXEL_COORDS);
					return;
				}
			}
		}
	}
	else
	{
		if (_pathCount > GetPathCount())
		{
			_pathCount = 0;
			_hasGoal = false;
		}
		_pathCount += 1;

		// go towards the x point of the closest node on the path
		if (_playerPositionXNode < GetNextPathXPos(_playerPositionXNode, _playerPositionYNode, NODE_COORDS))
		{
			_goRight = true;
		}
		else
		{
			_goLeft = true;
		}

		// Jump if below the nearest y point.
		if (_playerPositionYNode > GetNextPathYPos(_playerPositionXNode, _playerPositionYNode, NODE_COORDS))
		{
			_jump = true;
		}
	}
}