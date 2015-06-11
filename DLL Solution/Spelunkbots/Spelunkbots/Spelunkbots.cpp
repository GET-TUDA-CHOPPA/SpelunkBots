// Spelunkbots.cpp : Defines the exported functions for the DLL application.
// Spelunkbots Source code written by Daniel Scales t:@DanielCake

#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <list>
#include <map>
#include <ostream>
#include <string>
#include "SpelunkbotsConsoleOutput.h"

using namespace std;

#pragma region Defines

// Use GMEXPORT on all functions that are to be accessed
#define GMEXPORT extern "C" __declspec (dllexport)

// Number of x and y nodes
#define X_NODES 42
#define Y_NODES 34

#pragma endregion

#pragma region Entry

// entry point
BOOL WINAPI DllMain(HANDLE hinstDLL, DWORD dwReason, LPVOID lpvReserved)
{
	AllocConsole();
	freopen("CON", "w", stdout);

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

#pragma endregion

#pragma region Structures

struct collectableObject {
	int type;
	int id;
	double x;
	double y;
};

struct enemyObject {
	int type;
	int id;
	double x;
	double y;
	double directionFacing;
	double status;
};

#pragma endregion

#pragma region Variables

int screenTop;
int screenBottom;
int screenWidth;
int screenHeight;

// hold arrays of each type of object in here 
// means user can query the nearest object, or all of them
// and if it is 0, c++ can safely return false
// which is a lot safer than GML (GameMaker language).

// different states held in this, reset each time a new level is loaded

// once this is working, adapt to mark if it has a ladder in - essential
// for navigation

// map holds a bool as to whether there is a land tile in that location
// fog holds a bool as to whether the node has been discovered
// X = 41, Y = 33
double spmap[X_NODES][Y_NODES];
double mapLiquids[X_NODES][Y_NODES];
double mapFog[X_NODES][Y_NODES];

// An array that contains how many spider webs each position contains
double spiderWebs[X_NODES][Y_NODES];
double pushBlocks[X_NODES][Y_NODES];
double bats[X_NODES][Y_NODES];

std::vector<collectableObject> collectablesList;
std::vector<collectableObject> enemiesList;

double hasResetMap = 0;

double screenX;
double screenY;
double screenW;
double screenH;

// Player equipment
bool udjatEye;
bool coolGlasses;

// Game State
bool shopkeepersAngered;

#pragma endregion

#pragma region Convert Methods

/**
 * \brief ConvertToNodeCoordinates converts pixels coordinates in to node coordinates.
 *
 * @param x The x coordinate of the node to convert
 * @param y The y coordinate of the node to convert
 *
 * \note This function is only used within the Spelunkbots DLL.
*/
void ConvertToNodeCoordinates(double &x, double &y)
{
	x /= 16;
	y /= 16;
}

/**
 * \brief ConvertToNodeCoordinates converts pixels coordinates into node coordinates.
 *
 * @param x1 The x coordinate of first node to convert
 * @param y1 The y coordinate of the first node to convert
 * @param x2 The x coordinate of the second node to convert
 * @param y2 The y coordinate of the second node to convert
 *
 * \note This function is only to be used within the Spelunkbots DLL.
*/
void ConvertToNodeCoordinates(double &x1, double &y1, double &x2, double &y2)
{
	ConvertToNodeCoordinates(x1, y1);
	ConvertToNodeCoordinates(x2, y2);
}

#pragma endregion

#pragma region Map Data Methods

/**
 * \todo Usage unknown
 *
 * @param x:
 * @param y
 * @param w
 * @param h
 *
 * \note This function should not be used or changed whem implementing a bot.
*/
GMEXPORT double SetScreenXYWH(double x, double y, double w, double h)
{
	screenX = x;
	screenY = y;
	screenW = w;
	screenH = h;
	return 0;
}

/**
 * \brief A sample functions. All functions to be accessed should follow this format.
*/
GMEXPORT double SampleFunction(double a, double b) 
{
	return a * b;
}

/*
	\todo Usage unknown

	@param d
*/
GMEXPORT double SetCoolGlasses(double d)
{
	if (d == 1)
	{
		coolGlasses = true;
	}
	else
	{
		coolGlasses = false;
	}
	return 0;
}

/*
	\todo Usage unknown

	@param d
*/
GMEXPORT double SetUdjatEye(double d)
{
	if (d == 1)
	{
		udjatEye = true;
	}
	else
	{
		udjatEye = false;
	}
	return 0;
}

/**
 * \brief ResetFogForNewLevel resets the map data when a new level is loaded.
 *
 * \note This function should not be used or changed when implementing a bot.
*/
GMEXPORT double ResetFogForNewLevel()
{
	for (int i = 0; i < X_NODES; i++)
	{
		for (int j = 0; j < Y_NODES; j++)
		{
			spmap[i][j] = 1;
			mapFog[i][j] = 1;
			spiderWebs[i][j] = 0;
			mapLiquids[i][j] = 0;
		}
	}
	hasResetMap = 1;
	return 1;
}

/**
 * \brief TerrainWasDestroyed updates a node to be empty when it is destroyed.
 *
 * @param x The x location of the node
 * @param y The y location of the node
 *
 * \brief This function should not be used or changed whem implementing a bot.
*/
GMEXPORT double TerrainWasDestroyed(double x, double y)
{
	spmap[(int)x][(int)y] = 0;
	return 0;
}

/**
 * \brief SpiderWebCreated increments a node spider web count
 *
 * @param x The x location of the node
 * @param y The y location of the node
 *
 * \note This function should not be used or changed whem implementing a bot.
*/
GMEXPORT double SpiderWebCreated(double x, double y)
{
	spiderWebs[(int)x][(int)y] += 1;
	return 0;
}

/**
 * \brief SpiderWebWasRemoved decrements a nodes spider web count.
 *
 * @param x The x location of the node
 * @param y The y location of the node
 *
 * \note This function should not be used or changed whem implementing a bot.
*/
GMEXPORT double SpiderWebWasRemoved(double x, double y)
{
	if (spiderWebs[(int)x][(int)y] > 0)
	{
		spiderWebs[(int)x][(int)y] -= 1;
	}
	return 0;
}

/**
 * \brief DestroyFog removes the fog from a node.
 *
 * @param x The x location of the node
 * @param y The y location of the node
 *
 * \note This function should not be used or changed whem implementing a bot.
*/
GMEXPORT double DestroyFog(double x, double y)
{
	mapFog[(int)x][(int)y] = 0;
	return 0;
}

/**
 * \brief TerrainIsLadder updates a nodes state to a ladder.
 *
 * @param x The x location of the node
 * @param y The y location of the node
 *
 * \note This function should not be used or changed whem implementing a bot.
*/
GMEXPORT double TerrainIsLadder(double x, double y)
{
	spmap[(int)x][(int)y] = 2;
	return 0;
}

/**
 * \brief TerrainIsGoal sets a node to be the exit.
 *
 * @param x The x location of the node
 * @param y The y location of the node
 *
 * \note This function should not be used or changed whem implementing a bot.
*/
GMEXPORT double TerrainIsGoal(double x, double y)
{
	spmap[(int)x][(int)y] = 3;
	return 0;
}

/**
 * \brief TerrainIsStart sets a node to be the entrance.
 *
 * @param x The x location of the node
 * @param y The y location of the node
 *
 * \note This function should not be used or changed whem implementing a bot.
*/
GMEXPORT double TerrainIsStart(double x, double y)
{
	spmap[(int)x][(int)y] = 4;
	return 0;
}

/**
 * \brief TerrainIsAlter sets a node to be an alter.
 *
 * @param x The x location of the node
 * @param y The y location of the node
 *
 * \note This function should not be used or changed whem implementing a bot.
*/
GMEXPORT double TerrainIsAltar(double x, double y)
{
	spmap[(int)x][(int)y] = 5;
	return 0;
}

/**
 * \brief TerrainIsArrowTrapRight sets a node to be a right-facing arrow trap
 *
 * @param x The x location of the node
 * @param y The y location of the node
 *
 * \note This function should not be used or changed whem implementing a bot.
*/
GMEXPORT double TerrainIsArrowTrapRight(double x, double y)
{
	spmap[(int)x][(int)y] = 6;
	return 0;
}

/**
 * \brief TerrainIsArrowTrapLeft sets a node to be a left-facing arrow trap
 *
 * @param x The x location of the node
 * @param y The y location of the node
 *
 * \note This function should not be used or changed whem implementing a bot.
*/
GMEXPORT double TerrainIsArrowTrapLeft(double x, double y)
{
	spmap[(int)x][(int)y] = 7;
	return 0;
}

/**
 * TerrainIsShopKeeperArea sets a node to be a shop keeper.
 *
 * @param x The x location of the node
 * @param y The y location of the node
 *
 * \note This function should not be used or changed whem implementing a bot.
*/
GMEXPORT double TerrainIsShopKeeperArea(double x, double y)
{
	spmap[(int)x][(int)y] = 8;
	return 0;
}

/**
 * \brief TerrainIsIce sets a node to be ice.
 *
 * @param x The x location of the node
 * @param y The y location of the node
 *
 * \note This function should not be used or changed whem implementing a bot.
*/
GMEXPORT double TerrainIsIce(double x, double y)
{
	spmap[(int)x][(int)y] = 9;
	return 0;
}

/**
 * \brief TerrainIsSpike sets a node be a spike.
 *
 * @param x The x location of the node
 * @param y The y location of the node
 *
 * \note This function should not be used or changed whem implementing a bot.
*/
GMEXPORT double TerrainIsSpike(double x, double y)
{
	spmap[(int)x][(int)y] = 10;
	return 0;
}

/**
 *\brief TerrainIsSpearTrap sets a node to be a spear trap.
 *
 * @param x The x location of the node
 * @param y The y location of the node
 *
 * \note This function should not be used or changed whem implementing a bot.
*/
GMEXPORT double TerrainIsSpearTrap(double x, double y)
{
	spmap[(int)x][(int)y] = 11;
	return 0;
}

/**
 * \brief TerrainIsSwimWater sets a node to be water.
 *
 * @param x The x location of the node
 * @param y The y location of the node
 *
 * \note This function should not be used or changed whem implementing a bot.
*/
GMEXPORT double TerrainIsSwimWater(double x, double y)
{
	mapLiquids[(int)x][(int)y] = 1;
	return 0;
}

/**
 * \brief TerrainIsLave sets a node to be lava.
 *
 * @param x The x location of the node
 * @param y The y location of the node
 *
 * \note This function should not be used or changed whem implementing a bot.
*/
GMEXPORT double TerrainIsLava(double x, double y)
{
	mapLiquids[(int)x][(int)y] = 2;
	return 0;
}

/**
 * \brief TerrainIsGameEntrance sets a node to be the game entrance.
 *
 * @param x The x location of the node
 * @param y The y location of the node
 *
 * \note This function should not be used or changed whem implementing a bot.
*/
GMEXPORT double TerrainIsGameEntrance(double x, double y)
{
	spmap[(int)x][(int)y] = 12;
	return 0;
}

/**
 * \brief TerrainIsTree sets a node to be a tree.
 *
 * @param x The x location of the node
 * @param y The y location of the node
 *
 * \note This function should not be used or changed whem implementing a bot.
*/
GMEXPORT double TerrainIsTree(double x, double y)
{
	spmap[(int)x][(int)y] = 13;
	return 0;
}

/**
 * \brief TerrainIsTreeBranchLeaf sets a node to be a tree branch.
 *
 * @param x The x location of the node
 * @param y The y location of the node
 *
 * \note This function should not be used or changed whem implementing a bot.
*/
GMEXPORT double TerrainIsTreeBranchLeaf(double x, double y)
{
	spmap[(int)x][(int)y] = 14;
	return 0;
}

/**
 * \brief TerrainIsEmpty sets a node to be empty.
 *
 * @param x The x location of the node
 * @param y The y location of the node
 *
 * \note This function should not be used or changed whem implementing a bot.
*/
GMEXPORT double TerrainIsEmpty(double x, double y)
{
	mapLiquids[(int)x][(int)y] = 0;
	return 0;
}

/**
 * \brief GetLiquidValues returns the liquid state of a node.
 *
 * @param x The x location of the node
 * @param y The y location of the node
 *
 * @return Empty : 0, Water : 1, Lava : 2
 *
 * \note This function should not be used or changed whem implementing a bot.
*/
GMEXPORT double GetLiquidValue(double x, double y)
{
	return mapLiquids[(int)x][(int)y];
}

/**
 * \todo Implment function and document.
*/
GMEXPORT double SpringPadAtPosition(double x, double y)
{
	return 0;
}

/**
 * \todo Usage unknown
 *
 * @param x
 * @param y
*/
GMEXPORT double FillShopkeeperArea(double x, double y)
{
	// TODO
	// find each shopkeeper area in the map
	// populate sensibly
	spmap[(int)x][(int)y] = 8;
	return 0;
}

/**
 * \brief Call this each time a new level is loaded in a loop in GameMaker,
 * setting the state of the block as to whether there is a terrain block
 * in that coordinate.
 *
 * \note This function should not be used or changed when implementing a bot.
*/
GMEXPORT double SetMapCoord(double x, double y, double state)
{
	spmap[(int)x][(int)y] = state;
	return state;
}

/**
 * \brief ClearFogFromSquare removes the fog from a given node.
 *
 * @param x The x coordinate of the node
 * @param y The y coordinate of the node
 *
 * \note This function should not be used or changed when implemtning a bot.
*/
GMEXPORT double ClearFogFromSquare(double x, double y)
{
	mapFog[(int)x][(int)y] = 0;
	return 0;
}

#pragma endregion

#pragma region Node State Methods

/**
 * \brief GetNodeState returns an integer associated with a particular state.
 * Node coordinates or pixel coordinates may be used, using the third paramter, usingPixelCoords.
 *
 * @param x The x coordinate of the node to check
 * @param y The y coordinate of the node to check
 * @param usingPixelCoords A bool to inform the API what coordinate system to use; true (1) if using pixel coordinates, false (0) if using node coordinates
 *
 * @return Empty Node: 0, Standard Terrain: 1, Ladder: 2, Exit: 3, Entrance: 4, Alter: 5, Arrow Trap Right: 6, Arrow Trap Left: 7, Is In Shop: 8, Ice: 9, Spike: 10, Spear Trap: 11, Game Entrance: 12
 *
 * \brief If a node is in fog a value of -1 is returned.
 *
 * \note GM script: GetNodeState.
*/
GMEXPORT double GetNodeState(double x, double y, double usingPixelCoords)
{
	if (usingPixelCoords)
		ConvertToNodeCoordinates(x, y);

	if (mapFog[(int)x][(int)y] == 0)
	{
		return spmap[(int)x][(int)y];
	}
	return -1;
}

/**
 * \brief GetFogState returns an integer based on the fog state of a node; 1 if the node is in fog, 0 otherwise.
 *
 * \brief Node coordinates or pixel coordinates may be used, using the third paramter, usingPixelCoords.
 *
 * @param x The x coordinate of the node to check
 * @param y The y coordinate of the node to check
 * @param usingPixelCoords A bool to inform the API what coordinate system to use; true (1) if using pixel coordinates, false (0) if using node coordinates
 *
 * @return The state of the fog: 1 if there is fog, 0 otherwse
 *
 * \note GM script: GetFogState.
*/
GMEXPORT double GetFogState(double x, double y, double usingPixelCoords)
{
	if (usingPixelCoords)
		ConvertToNodeCoordinates(x, y);

	return mapFog[(int)x][(int)y];
}

#pragma endregion

#pragma region Dynamic Objects

/**
 * \brief ClearDynamicObjects removes push blocks and bats from the level.
 *
 * \note This function should not be used or changed whem implemtning a bot.
*/
GMEXPORT double ClearDynamicObjects()
{
	for (int i = 0; i < X_NODES; i++)
	{
		for (int j = 0; j < Y_NODES; j++)
		{
			pushBlocks[i][j] = 0;
			bats[i][j] = 0;
		}
	}
	return 0;
}

/**
 * \brief NodeContainsPushBlock sets a node to contain a dynamic block in a given node.
 *
 * \note This function should not be used or changed whem implementing a bot.
*/
GMEXPORT double NodeContainsPushBlock(double x, double y)
{
	pushBlocks[(int)x][(int)y] = 1;
	return 0;
}

/**
 * \brief GetNodeContainsPushBlock returns an integer value on whether a node contains a push block; 1 if a node contains a push block, zero others.
 * \brief Node coordinates or pixel coordinates may be used, using the third paramter, usingPixelCoords.
 *
 * @param x The x coordinate of the node to check
 * @param y The y coordinate of the node to check
 * @param usingPixelCoords A bool to inform the API what coordinate system to use; true (1) if using pixel coordinates, false (0) if using node coordinates
 *
 * @return 1 if node contains a push block, 0 otherwise
 *
 * \note GM script: GetPushBlock.
*/
GMEXPORT double GetNodeContainsPushBlock(double x, double y, double usingPixelCoords)
{
	if (usingPixelCoords)
		ConvertToNodeCoordinates(x, y);

	if (mapFog[(int)x][(int)y] == 0)
	{
		return pushBlocks[(int)x][(int)y];
	}
	return 0;
}

/**
 * \brief NodeContainsBats increments that number of bats in a given node.
 *
 * \note This function should not be used or changed when implementing a bot.
*/
GMEXPORT double NodeContainsBat(double x, double y)
{
	bats[(int)x][(int)y] += 1;
	return 0;
}

/**
 * \brief GetNodeContainsBat returns the number of bats at a given node.
 * \brief Node coordinates or pixel coordinates may be used, using the third paramter, usingPixelCoords.
 *
 * @param x The x coordinate of the node to check
 * @param y The y coordinate of the node to check
 * @param usingPixelCoords A bool to inform the API what coordinate system to use; true (1) if using pixel coordinates, false (0) if using node coordinates
 *
 * \note GM script: GetBatAtNode.
*/
GMEXPORT double GetNodeContainsBat(double x, double y, double usingPixelCoords)
{
	if (usingPixelCoords)
		ConvertToNodeCoordinates(x, y);

	if (mapFog[(int)x][(int)y] == 0)
	{
		return bats[(int)x][(int)y];
	}
	return 0;
}

/**
 * \brief NumberOfWebsInNode returns the number of webs in a node.
 * \brief Node coordinates or pixel coordinates may be used, using the third paramter, usingPixelCoords.
 *
 * @param x The x coordinate of the node to check
 * @param y The y coordinate of the node to check
 * @param usingPixelCoords A bool to inform the API what coordinate system to use; true (1) if using pixel coordinates, false (0) if using node coordinates
 *
 * @return number of webs
 *
 * \note GM script: GetNumberOfEnemyTypeInNodeXY.
*/
GMEXPORT double NumberOfWebsInNode(double x, double y, double usingPixelCoords)
{
	if (usingPixelCoords)
		ConvertToNodeCoordinates(x, y);

	if (mapFog[(int)x][(int)y] == 0)
	{
		return spiderWebs[(int)x][(int)y];
	}
	return 0;
}
#pragma endregion

#pragma region Collectables

/**
 * \brief ResetCollectables removes all collectables from the collectablesList vector.
 *
 * \note This function should not be used or changed when implementing a bot.
*/
GMEXPORT double ResetCollectables()
{
	collectablesList.clear();
	return 0;
}

/**
 * \brief NodeContainsCollectable adds a collectable to the collectablesList vector.
 *
 * @param x The x coordinate of the collectable
 * @param y The y coordinate of the collectable
 * @param type The type of the collectable
 * @param id The id of the collectable
 *
 * \note This function should not be used or changed when implementing a bot.
*/
GMEXPORT double NodeContainsCollectable(double x, double y, double type, double id)
{
	collectableObject object;
	object.x = x;
	object.y = y;
	object.type = type;
	object.id = id;
	collectablesList.push_back(object);
	return 0;
}

/**
 * \brief UpdateCollectableAtNode updates a collectable at a given node, with a particular id, in the collectablesList vector.
 *
 * @param x The new x coordinate
 * @param y The new y coorindate
 * @param id The id of the collectable to update
 *
 * \note This function should not be used or changed when implementing a bot.
*/
GMEXPORT double UpdateCollectableAtNode(double x, double y, double id)
{
	int cSize = collectablesList.size();
	for (int i = 0; i < cSize; i++)
	{
		if (collectablesList.at(i).id == id)
		{
			if (spmap[(int)x][(int)y] == 0 || coolGlasses || udjatEye)
			{
				collectablesList.at(i).x = x;
				collectablesList.at(i).y = y;
			}
			return 0;
		}
	}
	return 0;
}

/**
 * \brief RemoveCollectableWithID removes a collectable with the given id.
 *
 * @param id The id of the collectable to remove
 *
 * \note This function should not be used or changed when implementing a bot.
*/
GMEXPORT double RemoveCollectableWithID(double id)
{
	int cSize = collectablesList.size();
	for (int i = 0; i < cSize; i++)
	{
		if (collectablesList.at(i).id == id)
		{
			collectablesList.erase(collectablesList.begin() + i);
			return 0;
		}
	}
	return 0;
}

/**
 * \brief NumberOfCollectableTypeInNode returns the number of collectables in a node.
 * \brief Node coordinates or pixel coordinates may be used, using the third paramter, usingPixelCoords.
 *
 * @param type The type of collectable being searched for
 * @param x The x coordinate of the node to check
 * @param y The y coordinate of the node to check
 * @param usingPixelCoords A bool to inform the API what coordinate system to use; true (1) if using pixel coordinates, false (0) if using node coordinates
 *
 * @return The number of collectables in the node
 *
 * \note GM script: NumberOfCollectableTypeInNode.
*/
GMEXPORT double NumberOfCollectableTypeInNode(double type, double x, double y, double usingPixelCoords)
{
	if (usingPixelCoords)
		ConvertToNodeCoordinates(x, y);

	int cSize = collectablesList.size();
	double count = 0;
	if (mapFog[(int)x][(int)y] == 0)
	{
		for (int i = 0; i < cSize; i++)
		{
			if (collectablesList.at(i).type == type)
			{
				if ((int)collectablesList.at(i).x == (int)x && (int)collectablesList.at(i).y == (int)y)
				{
					count += 1;
				}
			}
		}
	}
	return count;
}

/**
 * \brief GetIDOfCollectableInNode returns the ID of a collectable, given the coordinates and type of a collectable.
 * \brief Node coordinates or pixel coordinates may be used, using the third paramter, usingPixelCoords.
 *
 * @param type The type of collectable being searched for
 * @param x The x coordinate of the node to check
 * @param y The y coordinate of the node to check
 * @param usingPixelCoords A bool to inform the API what coordinate system to use; true (1) if using pixel coordinates, false (0) if using node coordinates
 *
 * @return The ID of the collectable
 *
 * \note GM script: GetIDOfFirstCollectableOfTypeInNode.
*/
GMEXPORT double GetIDOfCollectableInNode(double type, double x, double y, double usingPixelCoords)
{
	if (usingPixelCoords)
		ConvertToNodeCoordinates(x, y);

	int cSize = collectablesList.size();
	double count = 0;
	if (mapFog[(int)x][(int)y] == 0)
	{
		for (int i = 0; i < cSize; i++)
		{
			if (collectablesList.at(i).type == type)
			{
				if ((int)collectablesList.at(i).x == (int)x && (int)collectablesList.at(i).y == (int)y)
				{
					return collectablesList.at(i).id;
				}
			}
		}
	}
	return 0;
}

/**
 * \brief IsCollectableInNode returns an integer based on whether a collectable is in a given node.
 * \brief Node coordinates or pixel coordinates may be used, using the third paramter, usingPixelCoords.
 *
 * @param x The x coordinate of the node to check
 * @param y The y coordinate of the node to check
 * @param usingPixelCoords A bool to inform the API what coordinate system to use; true (1) if using pixel coordinates, false (0) if using node coordinates
 *
 * @return 1 if a collectable is in the node, 0 otherwise
 *
 * \note GM script: IsCollectableInNode.
*/
GMEXPORT double IsCollectableInNode(double x, double y, double usingPixelCoords)
{
	if (usingPixelCoords)
		ConvertToNodeCoordinates(x, y);

	for (unsigned ID = 1; ID < 36; ID++)
	{
		if (GetIDOfCollectableInNode(ID, x, y, false))
		{
			return 1;
		}
	}
	return 0;
}

#pragma endregion

#pragma region Enemies

/**
 * \brief ResestEnemies removes all enemies from the enemiesList vector.
 *
 * \note This function should not be used or changed when implementing a bot.
*/
GMEXPORT double ResetEnemies()
{
	enemiesList.clear();
	return 0;
}

/**
 * \brief NodeContainsEnemy adds an enemy to the enemiesList vector.
 *
 * @param x The x coordinate of the enemy
 * @param y The y coordinate of the enemy
 * @param type The type of the enemy
 * @param id The id of the enemy
 *
 * \note This function should not be used or changed when implementing a bot
*/
GMEXPORT double NodeContainsEnemy(double x, double y, double type, double id)
{
	collectableObject object;
	object.x = x;
	object.y = y;
	object.type = type;
	object.id = id;
	enemiesList.push_back(object);
	return 0;
}

/**
 * \brief UpdateEnemyAtNode updates the position of an enemy at a given node, with a particular id, in the enemyList vector with its current position.
 *
 * @param x The new x position
 * @param y The new y position
 * @param id The id of the enemy to update
 *
 * \note This function should not be changed or use whem implementing a bot.
*/
GMEXPORT double UpdateEnemyAtNode(double x, double y, double id)
{
	int cSize = enemiesList.size();
	for (int i = 0; i < cSize; i++)
	{
		if (enemiesList.at(i).id == id)
		{
			enemiesList.at(i).x = x;
			enemiesList.at(i).y = y;
			return 0;
		}
	}
	return 0;
}

/**
 * \brief RemoveEnemyWithID removes an enemy with the given id.
 *
 * @param id The id of the enemy to remove
 *
 * \note This function should not be used or changed when implementing a bot
*/
GMEXPORT double RemoveEnemyWithID(double id)
{
	int cSize = enemiesList.size();
	for (int i = 0; i < cSize; i++)
	{
		if (enemiesList.at(i).id == id)
		{
			enemiesList.erase(enemiesList.begin() + i);
			return 0;
		}
	}
	return 0;
}

/**
 * \brief NumberOfEnemyTypeInNode returns the number of enemies in a node.
 * \brief Node coordinates or pixel coordinates may be used, using the third paramter, usingPixelCoords.
 *
 * @param type The type of enemy being searched for
 * @param x The x coordinate of the node to check
 * @param y The y coordinate of the node to check
 * @param usingPixelCoords A bool to inform the API what coordinate system to use; true (1) if using pixel coordinates, false (0) if using node coordinates
 *
 * @return The number of enemies
 *
 * \note GM script: GetNumberOfEnemyTypeInNodeXY.
*/
GMEXPORT double NumberOfEnemyTypeInNode(double type, double x, double y, double usingPixelCoords)
{
	if (usingPixelCoords)
		ConvertToNodeCoordinates(x, y);

	int cSize = enemiesList.size();
	double count = 0;
	if (mapFog[(int)x][(int)y] == 0)
	{
		for (int i = 0; i < cSize; i++)
		{
			if (enemiesList.at(i).type == type)
			{
				if ((int)enemiesList.at(i).x >= screenX && (int)enemiesList.at(i).x <= screenX + screenW &&
					(int)enemiesList.at(i).y >= screenY && (int)enemiesList.at(i).y <= screenY + screenW)
				{
					if ((int)enemiesList.at(i).x == (int)x && (int)enemiesList.at(i).y == (int)y)
					{
						count += 1;
					}
				}
			}
		}
	}
	return count;
}

/**
 * \brief GetIDOfEnemyInNode returns the ID of an enemy in a given node.
 * \brief Node coordinates or pixel coordinates may be used, using the third paramter, usingPixelCoords.
 *
 * @param type The type of enemy being searched for
 * @param x The x coordinate of the node to check
 * @param y The y coordinate of the node to check
 * @param usingPixelCoords A bool to inform the API what coordinate system to use; true (1) if using pixel coordinates, false (0) if using node coordinates
 *
 * @return The ID of the enemy
 *
 * \note GM script: GetIDOfFirstEnemyInNode.
*/
GMEXPORT double GetIDOfEnemyInNode(double type, double x, double y, double usingPixelCoords)
{
	if (usingPixelCoords)
		ConvertToNodeCoordinates(x, y);

	int cSize = enemiesList.size();
	double count = 0;
	if (mapFog[(int)x][(int)y] == 0)
	{
		for (int i = 0; i < cSize; i++)
		{
			if (enemiesList.at(i).type == type)
			{
				if ((int)enemiesList.at(i).x == (int)x && (int)enemiesList.at(i).y == (int)y)
				{
					return enemiesList.at(i).id;
				}
			}
		}
	}
	return 0;
}

/**
 * \brief IsEnemyInNode returns an integer based on whether an enemy is in a given node.
 * \brief Node coordinates or pixel coordinates may be used, using the third paramter, usingPixelCoords.
 *
 * @param x The x coordinate of the node to check
 * @param y The y coordinate of the node to check
 * @param usingPixelCoords A bool to inform the API what coordinate system to use; true (1) if using pixel coordinates, false (0) if using node coordinates
 *
 * @return 1 if an enemy is in the node, 0 otherwise
 *
 * \note GM script: IsEnemyInNode.
*/
GMEXPORT double IsEnemyInNode(double x, double y, double usingPixelCoords)
{
	if (usingPixelCoords)
		ConvertToNodeCoordinates(x, y);

	for (unsigned ID = 1; ID < 38; ID++)
	{
		if (GetIDOfEnemyInNode(ID, x, y, false) > 0)
		{
			return 1;
		}
	}
	return 0;
}

#pragma endregion

#pragma region Debug

/**
 * \brief SaveDynamicObjectFilesDebug saves each dynamic object in the current level to its associated file.
 *
 * \note This function should not be changed or used when implementing a bot.
*/
GMEXPORT double SaveDynamicObjectFilesDebug()
{
	ofstream fileStream;
	fileStream.open("level_layout.txt");
	for (int i = 0; i < Y_NODES; i++)
	{
		for (int j = 0; j < X_NODES; j++)
		{
			if (mapFog[j][i] == 0)
			{
				fileStream << spmap[j][i];
			}
			else
			{
				fileStream << 0;
			}
			fileStream << " ";
		}
		fileStream << "\n";
	}
	fileStream.close();
	fileStream.open("level_bats.txt");
	for (int i = 0; i < Y_NODES; i++)
	{
		for (int j = 0; j < X_NODES; j++)
		{
			if (mapFog[j][i] == 0)
			{
				fileStream << bats[j][i];
			}
			else
			{
				fileStream << 0;
			}
			fileStream << " ";
		}
		fileStream << "\n";
	}
	fileStream.close();
	fileStream.open("level_liquids.txt");
	for (int i = 0; i < Y_NODES; i++)
	{
		for (int j = 0; j < X_NODES; j++)
		{
			fileStream << mapLiquids[j][i];

			fileStream << " ";
		}
		fileStream << "\n";
	}
	fileStream.close();
	fileStream.open("level_collectables.txt");
	int size = collectablesList.size();
	for (int i = 0; i < size; i++)
	{
		fileStream << "TYPE: ";
		fileStream << collectablesList.at(i).type;
		fileStream << " X: ";
		fileStream << collectablesList.at(i).x;
		fileStream << " Y: ";
		fileStream << collectablesList.at(i).y;
		fileStream << " ID: ";
		fileStream << collectablesList.at(i).id;
		fileStream << "\n";
	}
	fileStream.close();
	fileStream.open("level_enemies.txt");
	size = enemiesList.size();
	for (int i = 0; i < size; i++)
	{
		fileStream << "TYPE: ";
		fileStream << enemiesList.at(i).type;
		fileStream << " X: ";
		fileStream << enemiesList.at(i).x;
		fileStream << " Y: ";
		fileStream << enemiesList.at(i).y;
		fileStream << " ID: ";
		fileStream << enemiesList.at(i).id;
		fileStream << "\n";
	}
	fileStream.close();
	return 0;
}

/**
 * \brief SaveSpiderwebsToFile saves the locations of webs in the current level to a text file.
 *
 * \note This function should not be changed or used whem implementing a bot
*/
GMEXPORT double SaveSpiderwebsToFile()
{
	ofstream fileStream;
	fileStream.open("level_webs.txt");
	for (int i = 0; i < Y_NODES; i++)
	{
		for (int j = 0; j < X_NODES; j++)
		{
			fileStream << spiderWebs[j][i];
			fileStream << " ";
		}
		fileStream << "\n";
	}
	fileStream.close();
	return 0;
}

/**
 * \brief SaveLevelLayoutToFile saves the current level layout into a text file.
 *
 * \note This function should not be changed or used when implementing a bot
*/
GMEXPORT double SaveLevelLayoutToFile()
{
	ofstream fileStream;
	fileStream.open("level_layout.txt");
	for (int i = 0; i < Y_NODES; i++)
	{
		for (int j = 0; j < X_NODES; j++)
		{
			if (mapFog[j][i] == 0)
			{
				fileStream << spmap[j][i];
			}
			else
			{
				fileStream << mapFog[j][i];
			}
			fileStream << " ";
		}
		fileStream << "\n";
	}
	fileStream.close();
	return 0;
}

#pragma endregion

#pragma region Navigation

/**
 * \brief MapSearchNode is used in the calculation of A* search.
*/
class MapSearchNode
{
public:
	int x;
	int y;
	int gScore;
	int hScore;
	int fScore;
	MapSearchNode *parent;
	bool opened;
	bool closed;

	double GetGScore(MapSearchNode *p)
	{
		return p->gScore + ((x == p->x || y == p->y) ? 1 : 1.5);
	}

	double GetHScore(MapSearchNode *p)
	{
		/*
		float xDist = ((x) - (p->x));
		float yDist = ((y) - (p->y));
		float distance = sqrt((xDist * xDist) + (yDist * yDist));
		hScore = (double) distance;*/
		return (abs(p->x - x) + abs(p->y - y));
	}

	double GetFScore()
	{
		return fScore;
	}

	void ComputeScores(MapSearchNode * end)
	{
		gScore = GetGScore(parent);
		hScore = GetHScore(end);
		fScore = gScore + hScore;
	}
};

// A-Star pathfinding.
// heavily based upon http://www.raywenderlich.com/4946/introduction-to-a-pathfinding

std::vector<MapSearchNode*> m_PathList;

// http://xpac27.github.io/a-star-pathfinder-c++-implementation.html

/**
 * \brief CalculatePathFromXYtoXY creates a path using the A* Search algorithm by specifying the players current node position and the node they wish to reach.
 * \brief Node coordinates or pixel coordinates may be used, using the third paramter, usingPixelCoords.
 *
 * @param x1 The x coordinate of the players position
 * @param y1 The y coordinate of the players position
 * @param x2 The x coordinate of the target location
 * @param y2 The y coordinate of the target location
 * @param usingPixelCoords A bool to inform the API what coordinate system to use; true (1) if using pixel coordinates, false (0) if using node coordinates
 *
 * \note GM script: CreateAStarPathFromXYtoXY.
*/
GMEXPORT double CalculatePathFromXYtoXY(double x1, double y1, double x2, double y2, double usingPixelCoords)
{
	if (usingPixelCoords)
		ConvertToNodeCoordinates(x1, y1, x2, y2);

	if (x1 != x2 || y1 != y2)
	{
		m_PathList.clear();

		std::map<int, std::map<int, MapSearchNode*> > grid;
		for (int i = 0; i < X_NODES; i++)
		{
			for (int j = 0; j < Y_NODES; j++)
			{
				grid[i][j] = new MapSearchNode();
				grid[i][j]->x = i;
				grid[i][j]->y = j;
			}
		}

		ofstream fileStream;
		fileStream.open("level_paths.txt");
		// define the new nodes
		MapSearchNode* start = new MapSearchNode();
		start->x = x1;
		start->y = y1;

		fileStream << "START";
		fileStream << " START X: ";
		fileStream << start->x;
		fileStream << " START Y: ";
		fileStream << start->y;

		MapSearchNode* end = new MapSearchNode();
		end->x = x2;
		end->y = y2;

		fileStream << "END";
		fileStream << " END X: ";
		fileStream << end->x;
		fileStream << " END Y: ";
		fileStream << end->y;

		MapSearchNode* current = new MapSearchNode();
		MapSearchNode* child = new MapSearchNode();

		std::list<MapSearchNode*> openList;
		std::list<MapSearchNode*> closedList;
		list<MapSearchNode*>::iterator i;

		unsigned int n = 0;

		openList.push_back(start);
		start->opened = true;

		while (n == 0 || (current != end && n < 50))
		{
			// Look for the smallest f value in the openList
			for (i = openList.begin(); i != openList.end(); i++)
			{
				if (i == openList.begin() || (*i)->GetFScore() <= current->GetFScore())
				{
					current = (*i);
				}
			}

			fileStream << "searching";
			fileStream << " Current X: ";
			fileStream << current->x;
			fileStream << " Current Y: ";
			fileStream << current->y;

			// Stop if we've reached the end
			if (current->x == end->x && current->y == end->y)
			{
				fileStream << "end reached";
				break;
			}

			// Remove the current point from the open list
			openList.remove(current);
			current->opened = false;

			// Add the current point from the open list
			closedList.push_back(current);
			current->closed = true;

			// Get all the current adjacent walkable points
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					if (x == 0 && y == 0)
					{
						// ignore current node, pass
						continue;
					}

					if (x == 0 || y == 0)
					{

						child = grid[current->x + x][current->y + y];

						// if it's closed or not walkable then pass
						if (child->closed || (spmap[child->x][child->y] != 0 && spmap[child->x][child->y] != 3 && spmap[child->x][child->y] != 4 && spmap[child->x][child->y] != 2 && spmap[child->x][child->y] != 9))
						{
							fileStream << "\n";
							fileStream << "closed or not walkable";
							continue;
						}

						// IF AT A CORNER?

						// if it's already in the opened list
						if (child->opened)
						{
							if (child->gScore > child->GetGScore(current))
							{
								child->parent = current;
								child->ComputeScores(end);
							}
						}
						else
						{
							openList.push_back(child);
							child->opened = true;

							// COMPUTE THE G
							child->parent = current;
							child->ComputeScores(end);
						}
					}
				}
			}
			n++;
			fileStream << "\n";
		}

		// Reset
		for (i = openList.begin(); i != openList.end(); i++)
		{
			(*i)->opened = false;
		}
		for (i = closedList.begin(); i != closedList.end(); i++)
		{
			(*i)->closed = false;
		}
		fileStream.close();

		fileStream.open("level_path.txt");
		// resolve the path starting from the end point
		while (current->parent && current != start)
		{
			fileStream << "X ";
			fileStream << current->x;
			fileStream << " Y ";
			fileStream << current->y;
			fileStream << "\n";
			m_PathList.push_back(current);
			current = current->parent;
			n++;
		}
		fileStream.close();
		return 0;
	}
	return 0;
}

/**
 * \todo Implement and document.
*/
GMEXPORT bool IsClearPathToExit()
{
	return 0;
}

/**
 * \brief GetNextPathXPos returns the X value of the next node along the path created by the A* search; calculatePathFromXYtoXY must be called before using this function.
 * \brief Node coordinates or pixel coordinates may be used, using the third paramter, usingPixelCoords.
 *
 * @param x The x coordinate of the players location
 * @param y The y coordinate of the players location
 * @param usingPixelCoords A bool to inform the API what coordinate system to use; true (1) if using pixel coordinates, false (0) if using node coordinates
 *
 * @returns x coordinate of the next node
 *
 * \note GM script: GetNearestXPos.
*/
GMEXPORT double GetNextPathXPos(double x, double y, double usingPixelCoords)
{
	if (usingPixelCoords)
		ConvertToNodeCoordinates(x, y);

	ofstream fileStream;
	fileStream.open("distance.txt");
	float smallestDistance = 0;
	int pos = 0;
	for (int i = 0; i < m_PathList.size(); i++)
	{
		float xDist = ((x)-(m_PathList.at(i)->x));
		float yDist = ((y)-(m_PathList.at(i)->y));
		float distance = sqrt((xDist * xDist) + (yDist * yDist));
		fileStream << distance;
		if (distance < smallestDistance || i == 0)
		{
			if (distance > 0)
			{
				smallestDistance = distance;

				pos = i;
			}
		}
	}
	fileStream.close();
	if (pos > 0)
		pos--;
	if (m_PathList.size() > 0)
		return m_PathList.at(pos)->x;
	return x;
}

/**
 * \brief GetNextPathXPos returns the Y value of the next node along the path created by the A* search; calculatePathFromXYtoXY must be called before using this function.
 * \brief Node coordinates or pixel coordinates may be used, using the third paramter, usingPixelCoords.
 *
 * @param x The x coordinate of the players location
 * @param y The y coordinate of the players location
 * @param usingPixelCoords A bool to inform the API what coordinate system to use; true (1) if using pixel coordinates, false (0) if using node coordinates
 *
 * @return The y coordinate of the next node
 *
 * \note GM script: GetNearestYPos.
*/
GMEXPORT double GetNextPathYPos(double x, double y, double usingPixelCoords)
{
	if (usingPixelCoords)
		ConvertToNodeCoordinates(x, y);

	float smallestDistance = 0;
	int pos = 0;
	for (int i = 0; i < m_PathList.size(); i++)
	{
		float xDist = ((x)-(m_PathList.at(i)->x));
		float yDist = ((y)-(m_PathList.at(i)->y));
		float distance = sqrt((xDist * xDist) + (yDist * yDist));
		if (distance < smallestDistance || i == 0)
		{
			if (distance > 0)
			{
				smallestDistance = distance;
				pos = i;
			}
		}
	}
	if (pos > 0)
		pos--;
	if (m_PathList.size() > 0)
		return m_PathList.at(pos)->y;
	return y;
}

/**
 * \brief IsNodePassable returns an integer based on whether a given node is passable.
 * \brief Node coordinates or pixel coordinates may be used, using the third paramter, usingPixelCoords.
 *
 * @param x The x coordinate of the node to check
 * @param y The y coordinate of the node to check
 * @param usingPixelCoords A bool to inform the API what coordinate system to use; true (1) if using pixel coordinates, false (0) if using node coordinates
 *
 * @return 1 if the node is passable, 0 otherwise
 *
 * \note GM script: IsNodePassable.
*/
GMEXPORT double IsNodePassable(double x, double y, double usingPixelCoords)
{
	int passableTypes[] = { 0, 2, 3, 4, 12 };

	if (usingPixelCoords)
		ConvertToNodeCoordinates(x, y);

	// Can't walk through pushable blocks
	if (GetNodeContainsPushBlock(x, y, false))
		return 0;

	int nodeState = GetNodeState(x, y, false);

	for (int var : passableTypes)
	{
		if (nodeState == var)
			return 1;
	}

	return 0;
}

#pragma endregion