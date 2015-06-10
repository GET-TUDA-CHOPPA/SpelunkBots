#include "stdafx.h"
#include "GoldDigger.h"

void GoldDigger::Update()
{
	if (!_hasGoal)
	{
		for (int nodeY = 0; nodeY < Y_NODES; nodeY += 1)
		{
			for (int nodeX = 0; nodeX < X_NODES; nodeX += 1)
			{
				if (NumberOfCollectableTypeInNode(spGoldBar, nodeX, nodeY, NODE_COORDS) > 0)
				{
					_targetX = nodeX * PIXELS_IN_NODES;
					_targetY = nodeY * PIXELS_IN_NODES;
					_hasGoal = true;
					CalculatePathFromXYtoXY(_playerPositionX, _playerPositionY, _targetX, _targetY, PIXEL_COORDS);
					return;
				}
				else if (NumberOfCollectableTypeInNode(spGoldBars, nodeX, nodeY, NODE_COORDS) > 0)
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
			CalculatePathFromXYtoXY(_playerPositionX, _playerPositionY, _targetX, _targetY, PIXEL_COORDS);
		}
		_pathCount += 1;

		// Check if the item we're looking for is still there - or did we collect it?
		if (NumberOfCollectableTypeInNode(spGoldBar, _targetX, _targetY, PIXEL_COORDS) == 0 &&
			NumberOfCollectableTypeInNode(spGoldBars, _targetX, _targetY, PIXEL_COORDS) == 0)
		{
			_hasGoal = false;
		}

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