#include "stdafx.h"
#include "PerformanceStats.h"


PerformanceStats::PerformanceStats()
{
	string testType = "";
	string ranking = "";
	_successfulAttempts = 0;
}


PerformanceStats::~PerformanceStats()
{
	_scores.clear();
	_successfulAttempts = 0;
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
	if (strcmp(stat, "SCORE")==0)
	{
		_scores.push_back(val);
		cout << "Score: " << val << endl;
	}
	else if (strcmp(stat, "ATTEMPT") == 0)
	{
		_successfulAttempts++;
		cout << "Successful Attemps: " << val << endl;
	}
	else if (strcmp(stat, "HEALTH") == 0)
	{
		_healthLeft.push_back(val);
		cout << "Health: " << val << endl;
	}
	else if (strcmp(stat, "TIME") == 0)
	{
		_completionTimes.push_back(val);
		cout << "Time: " << val << endl;
	}
}

void PerformanceStats::CalculatePerformance()
{

	if (_testType.compare("TESTMAPS")==0)
	{
		TestMaps();
	}
	else if (_testType.compare("MARATHON")==0)
	{
		Marathon();
	}
	else
	{
		cout << "Test Type not recognised. Performance Statistics will not be generated." << endl;
	}
}

void PerformanceStats::Clear()
{
	_scores.clear();
	_successfulAttempts = 0;
	_healthLeft.clear();
	_completionTimes.clear();
}

void PerformanceStats::TestMaps()
{
	if (_ranking.compare("SCORE")==0)
	{
		int bestScore = 0;
		int avgScore = 0;
		int avgHealth = 0;
		int bestTime = -1;
		if (!_completionTimes.empty())
		{
			bestTime = _completionTimes.at(0);
		}
		

		for (int i = 0; i < _scores.size(); i++)
		{
			avgScore += _scores.at(i);
			avgHealth += _healthLeft.at(i);

			if (_scores.at(i) > bestScore)
			{
				bestScore = _scores.at(i);
			}
		}

		avgScore = avgScore / _scores.size();
		avgHealth = avgHealth / _healthLeft.size();

		for (int k = 0; k < _completionTimes.size(); k++)
		{
			if (_completionTimes.at(k) < bestTime)
			{
				bestTime = _completionTimes.at(k);
			}
		}

		//Put in file saving code later.
		cout << endl << "Best Score: " << "\t\t\t" << bestScore << endl;
		cout << "Average Score: " << "\t\t\t" << avgScore << endl;
		cout << "Successful Attempts: " << "\t\t" << _successfulAttempts << endl;
		cout << "Average Remaining Health: " << "\t" << avgHealth << endl;
		cout << "Best Time: " << "\t\t\t" << bestTime << endl;

	}
	else if (_ranking.compare("TIME")==0)
	{

	}
	else
	{
		cout << "Ranking not recognised. Performance Statistics will not be generated." << endl;
	}
}

void PerformanceStats::Marathon()
{

}
