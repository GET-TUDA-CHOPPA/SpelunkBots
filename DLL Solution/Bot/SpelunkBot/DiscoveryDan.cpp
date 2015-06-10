#include "stdafx.h"
#include "DiscoveryDan.h"

void DiscoveryDan::Update()
{
	if (!_hasGoal)
	{
		for (int nodeX = 0; nodeX < X_NODES; nodeX += 1)
		{
			if (GetFogState(nodeX, _playerPositionYNode, NODE_COORDS) == 1)
			{
				_targetX = nodeX * PIXELS_IN_NODES;
				_targetY = _playerPositionY;
				_hasGoal = true;
				CalculatePathFromXYtoXY(_playerPositionX, _playerPositionY, _targetX, _targetY, PIXEL_COORDS);
				return;
			}
		}
		for (int nodeX = 0; nodeX < X_NODES; nodeX += 1)
		{
			for (int nodeY = 0; nodeY < Y_NODES; nodeY += 1)
			{
				if (GetNodeState(nodeX, nodeY, NODE_COORDS) == spExit)
				{
					_targetX = nodeX * PIXELS_IN_NODES;
					_targetY = nodeY * PIXELS_IN_NODES;
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

		if (_playerPositionXNode < GetNextPathXPos(_playerPositionXNode, _playerPositionYNode, NODE_COORDS))
		{
			_goRight = true;
		}
		else
		{
			_goLeft = true;
		}

		if ((_playerPositionYNode - 1) > GetNextPathYPos(_playerPositionXNode, _playerPositionYNode, NODE_COORDS))
		{
			_jump = true;
		}
	}
}