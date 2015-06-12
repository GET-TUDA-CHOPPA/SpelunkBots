#include "stdafx.h"
#include "Indie.h"

void Indie::Update()
{
	if (_holdingItem)
	{
		_goRight = true;
		_lookUp = true;
	}
	else
	{
		if (!_hasGoal)
		{
			for (int nodeX = 0; nodeX < X_NODES; nodeX += 1)
			{
				for (int nodeY = 0; nodeY < Y_NODES; nodeY += 1)
				{
					if (NumberOfCollectableTypeInNode(spGoldBar, nodeX, nodeY, NODE_COORDS))
					{
						_targetX = nodeX * PIXELS_IN_NODES;
						_targetY = nodeY * PIXELS_IN_NODES;
						_hasGoal = true;
						_itemGoal = false;
						CalculatePathFromXYtoXY(_playerPositionX, _playerPositionY, _targetX, _targetY, PIXEL_COORDS);
						return;
					}
				}
			}
			for (int nodeX = 0; nodeX < X_NODES; nodeX += 1)
			{
				for (int nodeY = 0; nodeY < Y_NODES; nodeY += 1)
				{
					if (NumberOfCollectableTypeInNode(spGoldIdol, nodeX, nodeY, NODE_COORDS))
					{
						_targetX = nodeX * PIXELS_IN_NODES;
						_targetY = nodeY * PIXELS_IN_NODES;
						_hasGoal = true;
						_itemGoal = true;
						if (_playerPositionXNode != nodeX || _playerPositionYNode != nodeY)
						{
							CalculatePathFromXYtoXY(_playerPositionX, _playerPositionY, _targetX, _targetY, PIXEL_COORDS);
						}
						return;
					}
				}
			}
		}
		else
		{
			if (_pathCount > GetPathCount() && !_itemGoal)
			{
				_pathCount = 0;
				_hasGoal = false;
				_itemGoal = false;
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

			// Jump if below the nearest y point.
			if ((_playerPositionYNode - 1) > GetNextPathYPos(_playerPositionXNode, _playerPositionYNode, NODE_COORDS))
			{
				_jump = true;
			}

			if (_playerPositionX < _targetX + 16 && _playerPositionX > _targetX - 16 && _playerPositionY < _targetY + 16 && _playerPositionY > _targetY - 16)
			{
				if (_itemGoal)
				{
					_duck = true;
					_attack = true;
					_holdingItem = true;
				}
			}
		}
	}
}