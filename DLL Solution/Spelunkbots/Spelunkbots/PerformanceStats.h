#pragma once
#include <vector>
#include <iostream>
#include <string>
#include<algorithm>
#include <math.h>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime> 
#include <iomanip>
using namespace std;

class PerformanceStats
{
public:
	PerformanceStats();
	~PerformanceStats();
	void SetTestType(char* testType);
	void SetRanking(char* ranking);
	void SetBotID(char* ID);
	string GetTestType();
	void Assigner(double val, char* stat);
	void CalculatePerformance();
	int CalcDistanceTraveled();
	void Clear();
	void PerformanceStats::TestMaps();
	void PerformanceStats::Marathon();
	bool CheckFolderState();

private:
	vector<int> _scores;
	vector<int> _healthLeft;
	vector<double> _times;
	vector<int> _attempts;
	string _testType;
	string _ranking;
	string _botID;
	int _entranceX;
	int _entranceY;
	int _exitX;
	int _exitY;
	int _botX;
	int _botY;
	int _testNumber;
	bool _unrecognisedData;
};

