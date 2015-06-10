#include "stdafx.h"
#include "SeanBean.h"

void SeanBean::Update()
{
	if (!_hasGoal)
	{
		// Search for the spike!
		for (int nodeY = 0; nodeY < Y_NODES; nodeY += 1)
		{
			for ( int nodeX = 0; nodeX < X_NODES; nodeX += 1)
			{
				if (GetNodeState(nodeX, nodeY, NODE_COORDS) == spSpike)
				{
					_hasGoal = true;
					_targetX = nodeX * PIXELS_IN_NODES;
					_targetY = nodeY * PIXELS_IN_NODES;
					CalculatePathFromXYtoXY(_playerPositionX, _playerPositionY, _targetX, _targetY, PIXEL_COORDS);
					std::cout << "FOUND: X: " << (_targetX / 16) << " Y: " << (_targetY / 16) << std::endl;
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
			CalculatePathFromXYtoXY(_playerPositionX, _playerPositionY, _targetX, _targetY, PIXEL_COORDS);
		}
		_pathCount += 1;
		
		if (_playerPositionXNode <GetNextPathXPos(_playerPositionXNode, _playerPositionYNode, NODE_COORDS))
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