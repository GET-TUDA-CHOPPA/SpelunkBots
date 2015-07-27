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
	}
	else if (strcmp(stat, "ATTEMPT") == 0)
	{
		_successfulAttempts++;
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
	// The reason for not putting avgScore and avgTime behind the if statements that check the ranking below (even though they may not be used depending on the ranking) 
	// is beacuse the performance impact of doing some simple calculations is less than setting up two new loop structures that will loop through data that was already looped through previously.
	if (_ranking.compare("SCORE") == 0 || _ranking.compare("TIME") == 0)
	{
		int bestScore = 0;
		int avgScore = 0;
		int avgHealth = 0;
		int avgTime = 0;
		int bestTime = 0;
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
			avgTime += _completionTimes.at(k);
			if (_completionTimes.at(k) < bestTime)
			{
				bestTime = _completionTimes.at(k);
			}
		}

		if (!_completionTimes.empty())
		{
			avgTime = avgTime / _completionTimes.size();
		}

		//Put in file saving code later.
		if (_ranking.compare("SCORE") == 0)
		{
			cout << endl << "Best Score: " << "\t\t\t" << bestScore << endl;
			cout << "Average Score: " << "\t\t\t" << avgScore << endl;
			cout << "Successful Attempts: " << "\t\t" << _successfulAttempts << endl;
			cout << "Average Remaining Health: " << "\t" << avgHealth << endl;
			cout << "Best Time: " << "\t\t\t" << bestTime << endl;
		}
		else if (_ranking.compare("TIME") == 0)
		{
			cout << endl << "Best Time: " << "\t\t\t" << bestTime << endl;
			cout << "Successful Attempts: " << "\t\t" << _successfulAttempts << endl;
			cout << "Average Time: " << "\t\t\t" << avgTime << endl;
			cout << "Average Remaining Health: " << "\t" << avgHealth << endl;
			cout << "Best Score: " << "\t\t\t" << bestScore << endl;
		}
	}
	else
	{
		cout << "Ranking not recognised. Performance Statistics will not be generated." << endl;
	}	
}

void PerformanceStats::Marathon()
{

}
