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

#define X_NODES 42
#define Y_NODES 34

// entry point
BOOL WINAPI DllMain(
	HANDLE hinstDLL,
	DWORD dwReason,
	LPVOID lpvReserved
	)
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

#define GMEXPORT extern "C" __declspec (dllexport)

using namespace std;

/*
	TranslateToNodeCoordinates converts pixels coordinates into node coordinates.

	x: x coordinate of the node to convert
	y: y coordinate of the node to convert

	This function is only used within the Spelunkbots DLL.
*/
void TranslateToNodeCoordinates(double &x, double &y)
{
	x /= 16;
	y /= 16;
}

/*
	TranslateToNodeCoordinates converts pixels coordinates into node coordinates.

	x1: x coordinate of first node to convert
	y1: y coordinate of the first node to convert
	x2: x coordinate of the second node to convert
	y2: y coordinate of the second node to convert

	This function is only used within the Spelunkbots DLL.
*/
void TranslateToNodeCoordinates(double &x1, double &y1, double &x2, double &y2)
{
	TranslateToNodeCoordinates(x1, y1);
	TranslateToNodeCoordinates(x2, y2);
}

GMEXPORT double SetScreenXYWH(double x, double y, double w, double h)
{
	screenX = x;
	screenY = y;
	screenW = w;
	screenH = h;
	return 0;
}

/*
	A sample functions. All functions to be accessed should follow this format.
*/
GMEXPORT double SampleFunction(double a, double b) {
	return a * b;
}

#pragma section "Layout"

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

/*
	ResetFogForNewLevel resets the map data when a new level is loaded

	This function should not be used or changed when implementing a bot
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

GMEXPORT double TerrainWasDestroyed(double x, double y)
{
	spmap[(int)x][(int)y] = 0;
	return 0;
}

GMEXPORT double SpiderWebCreated(double x, double y)
{
	spiderWebs[(int)x][(int)y] += 1;
	return 0;
}

GMEXPORT double SpiderWebWasRemoved(double x, double y)
{
	if (spiderWebs[(int)x][(int)y] > 0)
	{
		spiderWebs[(int)x][(int)y] -= 1;
	}
	return 0;
}

GMEXPORT double DestroyFog(double x, double y)
{
	mapFog[(int)x][(int)y] = 0;
	return 0;
}

GMEXPORT double TerrainIsLadder(double x, double y)
{
	spmap[(int)x][(int)y] = 2;
	return 0;
}

GMEXPORT double TerrainIsGoal(double x, double y)
{
	spmap[(int)x][(int)y] = 3;
	return 0;
}

GMEXPORT double TerrainIsStart(double x, double y)
{
	spmap[(int)x][(int)y] = 4;
	return 0;
}

GMEXPORT double TerrainIsAltar(double x, double y)
{
	spmap[(int)x][(int)y] = 5;
	return 0;
}

GMEXPORT double TerrainIsArrowTrapRight(double x, double y)
{
	spmap[(int)x][(int)y] = 6;
	return 0;
}

GMEXPORT double TerrainIsArrowTrapLeft(double x, double y)
{
	spmap[(int)x][(int)y] = 7;
	return 0;
}

GMEXPORT double TerrainIsShopKeeperArea(double x, double y)
{
	spmap[(int)x][(int)y] = 8;
	return 0;
}

GMEXPORT double TerrainIsIce(double x, double y)
{
	spmap[(int)x][(int)y] = 9;
	return 0;
}

GMEXPORT double TerrainIsSpike(double x, double y)
{
	spmap[(int)x][(int)y] = 10;
	return 0;
}

GMEXPORT double TerrainIsSpearTrap(double x, double y)
{
	spmap[(int)x][(int)y] = 11;
	return 0;
}

GMEXPORT double TerrainIsSwimWater(double x, double y)
{
	mapLiquids[(int)x][(int)y] = 1;
	return 0;
}

GMEXPORT double TerrainIsLava(double x, double y)
{
	mapLiquids[(int)x][(int)y] = 2;
	return 0;
}

GMEXPORT double TerrainIsGameEntrance(double x, double y)
{
	spmap[(int)x][(int)y] = 12;
	return 0;
}

GMEXPORT double TerrainIsTree(double x, double y)
{
	spmap[(int)x][(int)y] = 13;
	return 0;
}

GMEXPORT double TerrainIsTreeBranchLeaf(double x, double y)
{
	spmap[(int)x][(int)y] = 14;
	return 0;
}

GMEXPORT double TerrainIsEmpty(double x, double y)
{
	mapLiquids[(int)x][(int)y] = 0;
	return 0;
}

GMEXPORT double GetLiquidValue(double x, double y)
{
	return mapLiquids[(int)x][(int)y];
}

GMEXPORT double SpringPadAtPosition(double x, double y)
{
	return 0;
}

/*
	TODO
*/
GMEXPORT double FillShopkeeperArea(double x, double y)
{
	// TODO
	// find each shopkeeper area in the map
	// populate sensibly
	spmap[(int)x][(int)y] = 8;
	return 0;
}

/*
	Call this each time a new level is loaded in a loop in GameMaker
	Setting the state of the block as to whether there is a terrain block
	in that coordinate.

	Should also be called when a terrain block is destroyed by *anything*

	This function should not be used or changed when implementing a bot
*/
GMEXPORT double SetMapCoord(double x, double y, double state)
{
	spmap[(int)x][(int)y] = state;
	return state;
}

/*
	ClearFogFromSquare removes the fog from a give node.

	x: x coordinate of the node
	y: y coordinate of the node

	This function should not be used or changed when implemtning a bot
*/
GMEXPORT double ClearFogFromSquare(double x, double y)
{
	mapFog[(int)x][(int)y] = 0;
	return 0;
}

/*
	GetNodeState returns an integer associated with a particular state.
	Node coordinates or pixel coordinates may be used, using the third paramter, usingPixelCoords.

	x: x coordinate of the node to check
	y: y coordinate of the node to check
	usingPixelCoords: true (1) if using pixel coordinates, false (0) if using node coordinates

	returns: Empty Node: 0, Standard Terrain: 1, Ladder: 2, Exit: 3, Entrance: 4, Alter: 5, Arrow Trap Right: 6, Arrow Trap Left: 7, Is In Shop: 8, Ice: 9, Spike: 10, Spear Trap: 11, Game Entrance: 12

	If a node is in fog a value of -1 is returned.

	GM script: GetNodeState
*/
GMEXPORT double GetNodeState(double x, double y, double usingPixelCoords)
{
	if (usingPixelCoords)
		TranslateToNodeCoordinates(x, y);

	if (mapFog[(int)x][(int)y] == 0)
	{
		return spmap[(int)x][(int)y];
	}
	return -1;
}

/*
	GetFogState returns an integer on the fog state of a node; 1 if the node is in fog, 0 otherwise.

	Node coordinates or pixel coordinates may be used, using the third paramter, usingPixelCoords.

	GM script: GetFogState
*/
GMEXPORT double GetFogState(double x, double y, double usingPixelCoords)
{
	if (usingPixelCoords)
		TranslateToNodeCoordinates(x, y);

	return mapFog[(int)x][(int)y];
}


/*
	TODO
*/
GMEXPORT bool IsClearPathToExit()
{
	return 0;
}


#pragma section "Dynamic Objects"

/*
	ClearDynamicObjects removes push blocks and bats from the level.

	This function should not be used or changed whem implemtning a bot
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

/*
	NodeContainsPushBlock sets a node to contain a dynamic block in a given node.

	This function should not be used or changed whem implementing a bot
*/
GMEXPORT double NodeContainsPushBlock(double x, double y)
{
	pushBlocks[(int)x][(int)y] = 1;
	return 0;
}

/*
	GetNodeContainsPushBlock returns an integer value on whether a node contains a push block; 1 if a node contains a push block, zero others.
	Node coordinates or pixel coordinates may be used, using the third paramter, usingPixelCoords.

	x: x coordinate of the node to check
	y: y coordinate of the node to check
	usingPixelCoords: true (1) if using pixel coordinates, false (0) if using node coordinates

	GM script: GetPushBlock
*/
GMEXPORT double GetNodeContainsPushBlock(double x, double y, double usingPixelCoords)
{
	if (usingPixelCoords)
		TranslateToNodeCoordinates(x, y);

	if (mapFog[(int)x][(int)y] == 0)
	{
		return pushBlocks[(int)x][(int)y];
	}
	return 0;
}

/*
	NodeContainsBats increments that number of bats in a given node.

	This function should not be used or changed when implementing a bot
*/
GMEXPORT double NodeContainsBat(double x, double y)
{
	bats[(int)x][(int)y] += 1;
	return 0;
}

/*
	GetNodeContainsBat returns the number of bats at a given node.
	Node coordinates or pixel coordinates may be used, using the third paramter, usingPixelCoords.

	x: x coordinate of the node to check
	y: y coordinate of the node to check
	usingPixelCoords: true (1) if using pixel coordinates, false (0) if using node coordinates

	GM script: GetBatAtNode
*/
GMEXPORT double GetNodeContainsBat(double x, double y, double usingPixelCoords)
{
	if (usingPixelCoords)
		TranslateToNodeCoordinates(x, y);

	if (mapFog[(int)x][(int)y] == 0)
	{
		return bats[(int)x][(int)y];
	}
	return 0;
}

#pragma section "Collectables"

/*
	ResetCollectables removes all collectables from the collectablesList vector.

	This function should not be used or changed when implementing a bot
*/
GMEXPORT double ResetCollectables()
{
	collectablesList.clear();
	return 0;
}

/*
	NodeContainsCollectable adds a collectable to the collectablesList vector.

	x: x coordinate of the collectable
	y: y coordinate of the collectable
	type: type of the collectable
	id: id of the collectable

	This function should not be used or changed when implementing a bot
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

/*
	UpdateCollectableAtNode updates a collectable, matching the given an id, in the collectablesList vector.

	x: the new x coordinate
	y: the new y coorindate
	id: the id of the collectable to update

	This function should not be used or changed when implementing a bot
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

/*
	RemoveCollectableWithID removes a collectable with the given id.

	id: the id of the collectable to remove

	This function should not be used or changed when implementing a bot
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

/*
	NumberOfCollectableTypeInNode returns the number of collectables in a node.
	Node coordinates or pixel coordinates may be used, using the third paramter, usingPixelCoords.

	type: the type of collectable being searched for
	x: x coordinate of the node to check
	y: y coordinate of the node to check
	usingPixelCoords: true (1) if using pixel coordinates, false (0) if using node coordinates

	returns: number of collectables in the node

	GM script: NumberOfCollectableTypeInNode
*/
GMEXPORT double NumberOfCollectableTypeInNode(double type, double x, double y, double usingPixelCoords)
{
	if (usingPixelCoords)
		TranslateToNodeCoordinates(x, y);

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

#pragma section "Enemies"

/*
	ResestEnemies removes all enemies from the enemiesList vector.

	This function should not be used or changed when implementing a bot
*/
GMEXPORT double ResetEnemies()
{
	enemiesList.clear();
	return 0;
}

/*
	NodeContainsEnemy adds an enemy to the enemiesList vector.

	x: x coordinate of the enemy
	y: y coordinate of the enemy
	type: type of the enemy
	id: id of the enemy

	This function should not be used or changed when implementing a bot
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

/*
	UpdateEnemyAtNode updates the position of an enemy, matching the given id, in the enemyList vector with its current position.

	x: the new x position
	y: the new y position
	id: the id of the enemy to update

	This function should not be changed or use whem implementing a bot
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

/*
	RemoveEnemyWithID removes an enemy with the given id.

	id: the id of the enemy to remove

	This function should not be used or changed when implementing a bot
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

/*
	NumberOfEnemyTypeInNode returns the number of enemies in a node.
	Node coordinates or pixel coordinates may be used, using the third paramter, usingPixelCoords.

	x: the x coordinate of the node to check
	y: the y coordinate of the node to check
	usingPixelCoords: true (1) if using pixel coordinates, false (0) if using node coordinates

	returns: number of webs

	GM script: GetNumberOfEnemyTypeInNodeXY
*/
GMEXPORT double NumberOfWebsInNode(double x, double y, double usingPixelCoords)
{
	if (usingPixelCoords)
		TranslateToNodeCoordinates(x, y);

	if (mapFog[(int)x][(int)y] == 0)
	{
		return spiderWebs[(int)x][(int)y];
	}
	return 0;
}

/*
	NumberOfEnemyTypeInNode returns the number of enemies in a node.
	Node coordinates or pixel coordinates may be used, using the third paramter, usingPixelCoords.

	type: the type of enemy being searched for
	x: the x coordinate of the node to check
	y: the y coordinate of the node to check
	usingPixelCoords: true (1) if using pixel coordinates, false (0) if using node coordinates

	returns: number of enemies

	GM script: GetNumberOfEnemyTypeInNodeXY
*/
GMEXPORT double NumberOfEnemyTypeInNode(double type, double x, double y, double usingPixelCoords)
{
	if (usingPixelCoords)
		TranslateToNodeCoordinates(x, y);

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

/*
	GetIDOfEnemyInNode returns the ID of an enemy in a given node.
	Node coordinates or pixel coordinates may be used, using the third paramter, usingPixelCoords.

	type: the type of enemy being searched for
	x: the x coordinate of the node to check
	y: the y coordinate of the node to check
	usingPixelCoords: true (1) if using pixel coordinates, false (0) if using node coordinates

	returns: id of the enemy

	GM script: GetIDOfFirstEnemyInNode
*/
GMEXPORT double GetIDOfEnemyInNode(double type, double x, double y, double usingPixelCoords)
{
	if (usingPixelCoords)
		TranslateToNodeCoordinates(x, y);

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

/*
	GetIDOfCollectableInNode returns the ID of a collectable, given the coordinates and type of a collectable.
	Node coordinates or pixel coordinates may be used, using the third paramter, usingPixelCoords.

	type: the type of collectable being searched for
	x: the x coordinate of the node to check
	y: the y coordinate of the node to check
	usingPixelCoords: true (1) if using pixel coordinates, false if using node coordinates (0)

	returns: ID of the collectable

	GM script: GetIDOfFirstCollectableOfTypeInNode
*/
GMEXPORT double GetIDOfCollectableInNode(double type, double x, double y, double usingPixelCoords)
{
	if (usingPixelCoords)
		TranslateToNodeCoordinates(x, y);

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

#pragma section "Debug"

/*
	SaveDynamicObjectFilesDebug saves each dynamic type of the current level to its associated file.

	This function should not be changed or used when implementing a bot
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

/*
	SaveSpiderwebsToFile saves the locations of webs in the current level to a text file.

	This function should not be changed or used whem implementing a bot
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

/*
	SaveLevelLayoutToFile saves the current level layout into a text file.

	This function should not be changed or used when implementing a bot
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

/*
	MapSearchNode is used in the calculation of A* search
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

struct nodeValue {
	MapSearchNode node;
	double value;
};

MapSearchNode goal;

// A-Star pathfinding.
// heavily based upon http://www.raywenderlich.com/4946/introduction-to-a-pathfinding

std::vector<MapSearchNode*> m_PathList;
std::vector<MapSearchNode*> openList;



// http://xpac27.github.io/a-star-pathfinder-c++-implementation.html

/*
	CalculatePathFromXYtoXY creates a path using the A* Search algorithm by specifying players current node position and the node they wish to reach.
	Node coordinates or pixel coordinates may be used, using the third paramter, usingPixelCoords.

	x1: x coordinate of the players position
	y1: y coordinate of the players position
	x2: x coordinate of the target location
	y2: y coordinate of the target location
	usingPixelCoords: true (1) if using pixel coordinates, false (0) if using node coordinates

	GM script: CreateAStarPathFromXYtoXY
*/
GMEXPORT double CalculatePathFromXYtoXY(double x1, double y1, double x2, double y2, double usingPixelCoords)
{
	if (usingPixelCoords)
		TranslateToNodeCoordinates(x1, y1, x2, y2);

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

/*
	GetNextPathXPos returns the X value of the next node along the path created by the A* search; calculatePathFromXYtoXY must be called before using this function.
	Node coordinates or pixel coordinates may be used, using the third paramter, usingPixelCoords.

	x: x coordinate of the players location
	y: y coordinate of the players location
	usingPixelCoords: true (1) if using pixel coordinates, false (0) if using node coordinates

	returns: x coordinate of the next node

	GM script: GetNearestXPos
*/
GMEXPORT double GetNextPathXPos(double x, double y, double usingPixelCoords)
{
	if (usingPixelCoords)
		TranslateToNodeCoordinates(x, y);

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

/*
	GetNextPathXPos returns the Y value of the next node along the path created by the A* search; calculatePathFromXYtoXY must be called before using this function.
	Node coordinates or pixel coordinates may be used, using the third paramter, usingPixelCoords.

	x: x coordinate of the players location
	y: y coordinate of the players location
	usingPixelCoords: true (1) if using pixel coordinates, false (0) if using node coordinates

	returns: y coordinate of the next node

	GM script: GetNearestYPos
*/
GMEXPORT double GetNextPathYPos(double x, double y, double usingPixelCoords)
{
	if (usingPixelCoords)
		TranslateToNodeCoordinates(x, y);

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

/*
	IsEnemyInNode returns an integer based on whether an enemy is in a given node.
	Node coordinates or pixel coordinates may be used, using the third paramter, usingPixelCoords.

	x: the x coordinate of the node to check
	y: the y coordinate of the node to check
	usingPixelCoords: true (1) if using pixel coordinates, false (0) if using node coordinates

	returns: 1 if an enemy is in the node, 0 otherwise

	GM script: IsEnemyInNode
*/
GMEXPORT double IsEnemyInNode(double x, double y, double usingPixelCoords)
{
	if (usingPixelCoords)
		TranslateToNodeCoordinates(x, y);

	for (unsigned ID = 1; ID < 38; ID++)
	{
		if (GetIDOfEnemyInNode(ID, x, y, false) > 0)
		{
			return 1;
		}
	}
	return 0;
}

/*
	IsCollectableInNode returns an integer based on whether a collectable is in a given node.
	Node coordinates or pixel coordinates may be used, using the third paramter, usingPixelCoords.

	type: the type of enemy being searched for
	x: the x coordinate of the node to check
	y: the y coordinate of the node to check
	usingPixelCoords: true (1) if using pixel coordinates, false (0) if using node coordinates

	returns: 1 if a collectable is in the node, 0 otherwise

	GM script: IsCollectableInNode
*/
GMEXPORT double IsCollectableInNode(double x, double y, double usingPixelCoords)
{
	if (usingPixelCoords)
		TranslateToNodeCoordinates(x, y);

	for (unsigned ID = 1; ID < 36; ID++)
	{
		if (GetIDOfCollectableInNode(ID, x, y, false))
		{
			return 1;
		}
	}
	return 0;
}

/*
	IsNodePassable returns an integer based on whether a given node is passable.
	Node coordinates or pixel coordinates may be used, using the third paramter, usingPixelCoords.

	x: the x coordinate of the node to check
	y: the y coordinate of the node to check
	usingPixelCoords: true (1) if using pixel coordinates, false (0) if using node coordinates

	returns: 1 if the node is passable, 0 otherwise

	GM script: IsNodePassable
*/
GMEXPORT double IsNodePassable(double x, double y, double usingPixelCoords)
{
	int passableTypes[] = { 0, 2, 3, 4, 12 };

	if (usingPixelCoords)
		TranslateToNodeCoordinates(x, y);

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