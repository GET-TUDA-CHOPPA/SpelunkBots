#include "stdafx.h"
#include "PerformanceStats.h"


PerformanceStats::PerformanceStats()
{
	string testType = "";
	string ranking = "";
	_entranceX = 0;
	_entranceY = 0;
	_exitX = 0;
	_exitY = 0;
	_botX = 0;
	_botY = 0;
}


PerformanceStats::~PerformanceStats()
{
	_scores.clear();
	_attempts.clear();
	_healthLeft.clear();
	_times.clear();
	_testType = "";
	_ranking = "";
	_entranceX = 0;
	_entranceY = 0;
	_exitX = 0;
	_exitY = 0;
	_botX = 0;
	_botY = 0;
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


string PerformanceStats::GetTestType()
{
	return _testType;
}


void PerformanceStats::Assigner(double val, char* stat)
{
	if (strcmp(stat, "SCORE")==0)
	{
		_scores.push_back(val);
	}
	else if (strcmp(stat, "ATTEMPT") == 0)
	{
		_attempts.push_back(val);
	}
	else if (strcmp(stat, "HEALTH") == 0)
	{
		_healthLeft.push_back(val);
	}
	else if (strcmp(stat, "TIME") == 0)
	{
		_times.push_back(val);
	}
	else if (strcmp(stat, "EXITX") == 0)
	{
		_exitX = val;
	}
	else if (strcmp(stat, "EXITY") == 0)
	{
		_exitY = val;
	}
	else if (strcmp(stat, "ENTRANCEX") == 0)
	{
		_entranceX = val;
	}
	else if (strcmp(stat, "ENTRANCEY") == 0)
	{
		_entranceY = val;
	}
	else if (strcmp(stat, "BOTX") == 0)
	{
		_botX = val;
	}
	else if (strcmp(stat, "BOTY") == 0)
	{
		_botY = val;
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
	_attempts.clear();
	_healthLeft.clear();
	_times.clear();
	_entranceX = 0;
	_entranceY = 0;
	_exitX = 0;
	_exitY = 0;
	_botX = 0;
	_botY = 0;
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
		int bestTime = _times.at(0);
		int successes = 0;


		for (int i = 0; i < _scores.size(); i++)
		{
			avgScore += _scores.at(i);
			avgHealth += _healthLeft.at(i);
			successes += _attempts.at(i);
			avgTime += _times.at(i);

			if (_scores.at(i) > bestScore)
			{
				bestScore = _scores.at(i);
			}
			if (_times.at(i) < bestTime)
			{
				bestTime = _times.at(i);
			}
		}

		avgScore = avgScore / _scores.size();
		avgHealth = avgHealth / _healthLeft.size();
		avgTime = avgTime / _times.size();

		//Put in file saving code later.
		if (_ranking.compare("SCORE") == 0)
		{
			cout << endl << "Best Score: " << "\t\t\t" << bestScore << endl;
			cout << "Average Score: " << "\t\t\t" << avgScore << endl;
			cout << "Successful Attempts: " << "\t\t" << successes << endl;
			cout << "Average Remaining Health: " << "\t" << avgHealth << endl;
			cout << "Best Time: " << "\t\t\t" << bestTime << endl;
		}
		else if (_ranking.compare("TIME") == 0)
		{
			cout << endl << "Best Time: " << "\t\t\t" << bestTime << endl;
			cout << "Successful Attempts: " << "\t\t" << successes << endl;
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
	if (_ranking.compare("SCORE") == 0 || _ranking.compare("TIME") == 0)
	{
		int timeTaken = 0;
		for (int i = 0; i < _times.size(); i++)
		{
			timeTaken += _times.at(i);
		}

		//Put data in files
		if (_ranking.compare("SCORE") == 0)
		{
			
			cout << endl << "Score: " << "\t" << _scores.at(0) << endl;
			if (_attempts.at(0) != 16)
			{
				cout << "Completed?: " << "\t" << "NO" << endl;
				cout << "Time Taken: " << "\t" << timeTaken << endl;
			}
			else
			{
				cout << "Completed?: " << "\t" << "YES" << endl;
				cout << "Time Taken: " << "\t" << timeTaken << endl;
				cout << "Health: " << "\t" << _healthLeft.at(0) << endl;
			}
		}
		else if (_ranking.compare("TIME") == 0)
		{
			cout << endl;
			if (_attempts.at(0) != 16)
			{
				cout << "Completed?: " << "\t\t\t\t" << "NO" << endl;
				for (int i = 0; i < _times.size() -1; i++)
				{
					cout << "Time Taken to complete Level " << i + 1 << ": " << "\t" << _times.at(i) << endl;
				}
				cout << "Time Taken until death on Level " << _times.size() << ": " << "\t" << _times.at(_times.size() - 1) << endl;
				cout << "Time Taken Overall: " << "\t\t\t" << timeTaken << endl;
				cout << "Distance travelled to exit: " << "\t\t" << CalcDistanceTraveled() << "%" << endl;
				cout << "Score: " << "\t\t\t\t\t" << _scores.at(0) << endl;
			}
			else
			{
				cout << "Completed?: " << "\t\t\t" << "YES" << endl;
				for (int i = 0; i < _times.size(); i++)
				{
					cout << "Time Taken to complete Level " << i + 1 << ": " << "\t" << _times.at(i) << endl;
				}
				cout << "Time Taken Overall: " << "\t\t\t" << timeTaken << endl;
			}			
		}
	}
	else
	{
		cout << "Ranking not recognised. Performance Statistics will not be generated." << endl;
	}
}

int PerformanceStats::CalcDistanceTraveled()
{
	double startDistanceX = 0;
	double startDistanceY = 0;
	double botDistanceX = 0;
	double botDistanceY = 0;
	double distanceTraveled = 0;

	startDistanceX = _exitX - _entranceX;
	startDistanceX = startDistanceX * startDistanceX;
	startDistanceX = abs(startDistanceX);

	startDistanceY = _exitY - _entranceY;
	startDistanceY = startDistanceY * startDistanceY;
	startDistanceY = abs(startDistanceY);

	double result1 = sqrt(startDistanceX + startDistanceY);

	botDistanceX = _exitX - _botX;
	botDistanceX = botDistanceX * botDistanceX;
	botDistanceX = abs(botDistanceX);

	botDistanceY = _exitY - _botY;
	botDistanceY = botDistanceY * botDistanceY;
	botDistanceY = abs(botDistanceY);

	double result2 = sqrt(botDistanceX + botDistanceY);

	result2 = result1 - result2;

	if (result2 < 0)
	{
		distanceTraveled = 0;
	}
	else
	{
		distanceTraveled = (result2 / result1) * 100;
	}

	return distanceTraveled;
}
