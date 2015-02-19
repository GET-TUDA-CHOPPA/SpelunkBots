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

SpelunkbotsConsoleOutput spelunkbotConsoleOutput;

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

// Convert pixel coordinates into node coordinates
void TranslateToNodeCoordinates(double &x, double &y)
{
	x /= 16;
	y /= 16;
}

void TranslateToNodeCoordinates(double &x1, double &y1, double &x2, double &y2)
{
	TranslateToNodeCoordinates(x1, y1);
	TranslateToNodeCoordinates(x2, y2);
}


GMEXPORT double UpdatePlayerVariables(char *name, char *value, double type)
{
	std::string varName = name;
	std::string varValue = value;

	if (type == 0)
	{
		varValue = (varValue == "1" ? "True" : "False");
	}

	spelunkbotConsoleOutput.UpdateVariable(varName, varValue);
	return 0;
}

GMEXPORT double DisplayMessages()
{
	spelunkbotConsoleOutput.DisplayVariables();
	return 0;
}

GMEXPORT double SetScreenXYWH(double x, double y, double w, double h)
{
	screenX = x;
	screenY = y;
	screenW = w;
	screenH = h;
	return 0;
}

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

// Call this each time a new level is loaded
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
*/
GMEXPORT double SetMapCoord(double x, double y, double state)
{
	spmap[(int)x][(int)y] = state;
	return state;
}

/// Call this each time a new square is discovered in the map
GMEXPORT double ClearFogFromSquare(double x, double y)
{
	mapFog[(int)x][(int)y] = 0;
	return 0;
}

// returns whether a node has terrain or not
// if the section has not been discovered then it returns false
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

GMEXPORT double GetFogState(double x, double y)
{
	return mapFog[(int)x][(int)y];
}


GMEXPORT bool IsClearPathToExit()
{
	return 0;
}


#pragma section "Dynamic Objects"

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

// push blocks
GMEXPORT double NodeContainsPushBlock(double x, double y)
{
	pushBlocks[(int)x][(int)y] = 1;
	return 0;
}

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

// enemies

GMEXPORT double NodeContainsBat(double x, double y)
{
	bats[(int)x][(int)y] += 1;
	return 0;
}

// Returns the number of bats for a given node.
GMEXPORT double GetNodeContainsBat(double x, double y)
{
	if (mapFog[(int)x][(int)y] == 0)
	{
		return bats[(int)x][(int)y];
	}
	return 0;
}

#pragma section "Collectables"

GMEXPORT double ResetCollectables()
{
	collectablesList.clear();
	return 0;
}

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

GMEXPORT double NumberOfCollectableTypeInNode(double type, double x, double y)
{
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

GMEXPORT double ResetEnemies()
{
	enemiesList.clear();
	return 0;
}

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

GMEXPORT double NumberOfWebsInNode(double x, double y)
{
	if (mapFog[(int)x][(int)y] == 0)
	{
		return spiderWebs[(int)x][(int)y];
	}
	return 0;
}

GMEXPORT double NumberOfEnemyTypeInNode(double type, double x, double y)
{
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

// ===== Example custom functions ===== //

// Check if there is any enemy type in a node
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

// Check if there is any collectable type in a node
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

// Check if a node is passable
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