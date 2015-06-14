#include "stdafx.h"
#include "FishBot.h"

void FishBot::Update()
{
	// Evaluate the surrounding blocks
	double block = GetNodeState((_playerPositionXNode) + 1, _playerPositionYNode, NODE_COORDS);
	bool canMoveRight = (block == spEmptyNode);

	block = GetNodeState((_playerPositionXNode) - 1, _playerPositionYNode, NODE_COORDS);
	bool canMoveLeft = (block == spEmptyNode);

	if (_headingRight)
	{
		_facing = 1;
	}
	else
	{
		_facing = -1;
	}

	bool canJump;
	bool canFall;

	canJump = (GetNodeState(_playerPositionXNode + _facing, _playerPositionYNode - 1, NODE_COORDS) == spEmptyNode);
	canFall = (GetNodeState(_playerPositionXNode + _facing, _playerPositionYNode + 1, NODE_COORDS) == spEmptyNode);

	// Detect creature in front
	for (int creature = spGhost; creature <= spSpiderHang; creature += 1)
	{
		_numberOfCreatures = NumberOfEnemyTypeInNode(creature, _playerPositionXNode + _facing, _playerPositionYNode, NODE_COORDS);
		int creatureType = creature;

		if (_numberOfCreatures > 0)
		{
			break;
		}
	}


	// Act on evaluations
	if (_isHanging)
	{
		_duck = true;
		_jump = true;
		_isHanging = false;
	}
	else if (_numberOfCreatures > 0)
	{
		_attack = true;
	}
	else if (_headingRight)
	{
		if (canFall && canMoveRight)
		{
			int heightCount;
			heightCount = 1;
			bool searchingForGround = true;
			do
			{
				heightCount += 1;
				searchingForGround = GetNodeState(_playerPositionXNode + _facing, _playerPositionYNode + heightCount, NODE_COORDS) == spEmptyNode;
			} while (searchingForGround == false);

			if (heightCount > 8)
			{
				_duck = true;
				_ropep = true;
				std::cout << "place rope?" << std::endl;
				_goRight = true;
			}
			else if (heightCount > 4)
			{
				_duck = true;
				_goRight = true;
				_isHanging = GetNodeState(_playerPositionXNode, _playerPositionYNode + 1, NODE_COORDS) == spEmptyNode;
			}
			else
			{
				_goRight = true;
			}
		}
		else if (canMoveRight)
		{
			_goRight = true;
		}
		else if (canJump)
		{
			_jump = true;
		}
		else
		{
			_headingRight = false;
		}
	}
	else
	{
		if (canFall && canMoveLeft)
		{
			int heightCount = 1;
			bool searchingForGround = true;

			do
			{
				heightCount += 1;
				searchingForGround = (GetNodeState(_playerPositionXNode + _facing, _playerPositionYNode + heightCount, NODE_COORDS) == spEmptyNode);
			} while (searchingForGround == false);

			if (heightCount >= 8)
			{
				_duck = true;
				_ropep = true;
				std::cout << "place rope?" << std::endl;
			}
			else if (heightCount > 4)
			{
				_duck = true;
				_goLeft = true;
				_isHanging = GetNodeState(_playerPositionXNode, _playerPositionYNode + 1, NODE_COORDS) == spEmptyNode;
			}
			else
			{
				_goLeft = true;
			}
		}
		else if (canMoveLeft)
		{
			_goLeft = true;
		}
		else if (canJump)
		{
			_jump = true;
		}
		else
		{
			_headingRight = true;
		}
	}
}