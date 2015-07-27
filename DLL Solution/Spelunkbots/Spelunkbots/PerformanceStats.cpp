#include "stdafx.h"
#include "PerformanceStats.h"


PerformanceStats::PerformanceStats()
{
	string testType = "";
	string ranking = "";
}


PerformanceStats::~PerformanceStats()
{
	_gold.clear();
	_successfulAttempts.clear();
	_healthLeft.clear();
	_completionTimes.clear();
}

void PerformanceStats::SetTestType(char* testType)
{
	string str = testType;
	transform(str.begin(), str.end(), str.begin(), ::toupper);
	_testType = str;
	cout << "Test Type: " << _testType << endl;
}

void PerformanceStats::SetRanking(char* ranking)
{
	string str = ranking;
	transform(str.begin(), str.end(), str.begin(), ::toupper);
	_ranking = str;
	cout << "Ranking: " << _ranking << endl;
}

void PerformanceStats::Assigner(double val, char* stat)
{
	if (strcmp(stat, "GOLD")==0)
	{
		_gold.push_back(val);
	}
	else if (strcmp(stat, "ATTEMPT") == 0)
	{
		_successfulAttempts.push_back(val);
	}
	else if (strcmp(stat, "HEALTH") == 0)
	{
		_healthLeft.push_back(val);
	}
	else if (strcmp(stat, "TIME") == 0)
	{
		_completionTimes.push_back(val);
	}
}

void PerformanceStats::CalculatePerformance()
{

	if (_testType.compare("TESTMAPS")==0)
	{
		//go to test map calculation
	}
	else if (_testType.compare("MARATHON")==0)
	{
		//go to marathon calculation
	}
}

void PerformanceStats::Clear()
{
	_gold.clear();
	_successfulAttempts.clear();
	_healthLeft.clear();
	_completionTimes.clear();
}
