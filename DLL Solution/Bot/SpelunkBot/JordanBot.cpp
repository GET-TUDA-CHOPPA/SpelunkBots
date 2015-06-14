#include "stdafx.h"
#include "JordanBot.h"

void JordanBot::Update()
{
	_canGoRight = IsNodePassable(_playerPositionXNode + 1, _playerPositionYNode, NODE_COORDS);
	_canGoLeft = IsNodePassable(_playerPositionXNode - 1, _playerPositionYNode, NODE_COORDS);

	_canJumpRight = IsNodePassable(_playerPositionXNode + 1, _playerPositionYNode - 1, NODE_COORDS);
	_canJumpLeft = IsNodePassable(_playerPositionXNode - 1, _playerPositionYNode - 1, NODE_COORDS);

	_canJumpGrabRight = IsNodePassable(_playerPositionXNode + 1, _playerPositionYNode - 2, NODE_COORDS);
	_canJumpGrabLeft = IsNodePassable(_playerPositionXNode - 1, _playerPositionYNode - 2, NODE_COORDS);

	if (!_hasGoal)
	{
		for (unsigned nodeY = 0; nodeY < Y_NODES; nodeY++)
		{
			for (unsigned nodeX = 0; nodeX < X_NODES; nodeX++)
			{
				if (GetNodeState(nodeX, nodeY, NODE_COORDS) == spExit)
				{
					_hasGoal = true;
					_targetX = nodeX * PIXELS_IN_NODES;
					_targetY = nodeY * PIXELS_IN_NODES;
					CalculatePathFromXYtoXY(_playerPositionX, _playerPositionY, _targetX, _targetY, PIXEL_COORDS);
					return;
				}
			}
		}

		if (_headingRight && (_canGoRight || _canJumpRight || _canJumpGrabRight))
		{
			if (!_canGoRight)
			{
				_jump = true;
			}

			_goRight = true;
			_headingRight = true;
			_headingLeft = false;
		}
		else if (_headingLeft && (_canGoLeft || _canJumpLeft || _canJumpGrabLeft))
		{
			if (!_canGoLeft)
			{
				_jump = true;
			}

			_goLeft = true;
			_headingLeft = true;
			_headingRight = false;
		}
		else if (_headingRight && (!_canGoRight && !_canJumpRight || !_canJumpGrabRight))
		{
			_goLeft = true;
			_headingLeft = true;
			_headingRight = false;
		}
		else
		{
			_goRight = true;
			_headingRight = true;
			_headingLeft = false;
		}
	}
	else
	{
		if (_pathCount > 60)
		{
			_pathCount = 0;
			CalculatePathFromXYtoXY(_playerPositionX, _playerPositionY, _targetX, _targetY, PIXEL_COORDS);
		}
		_pathCount++;

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
			if (_goRight && (_canJumpRight || _canJumpGrabRight))
			{
				_jump = true;
			}
			else if (_canJumpRight || _canJumpGrabLeft)
			{
				_jump = true;
			}
		}
	}

	if (_headingRight)
	{
		_attack = IsEnemyInNode(_playerPositionXNode + 1, _playerPositionYNode, NODE_COORDS);
	}
	else
	{
		_attack = IsEnemyInNode(_playerPositionX - 1, _playerPositionYNode, NODE_COORDS);
	}

	if (_attack)
	{
		_goLeft = false;
		_goRight = false;
	}
}