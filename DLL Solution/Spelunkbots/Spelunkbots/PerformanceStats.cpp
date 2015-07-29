#include "stdafx.h"
#include "PerformanceStats.h"


PerformanceStats::PerformanceStats()
{
	string testType = "";
	string ranking = "";
}


PerformanceStats::~PerformanceStats()
{
	_scores.clear();
	_attempts.clear();
	_healthLeft.clear();
	_times.clear();
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
		int bestScore = 0;
		int location = 0;
		int completed = 0;
		int health = 0;
		int time = 0;
		bool bestFound = false;

		// Find a initial "best" score.
		for (int i = 0; i < _scores.size(); i++)
		{
			if (_scores.at(i) > bestScore)
			{
				bestScore = _scores.at(i);
				location = i;
			}
		}

		// Get accompanying data.
		completed = _attempts.at(location);
		health = _healthLeft.at(location);
		time = _times.at(location);
		bestScore = _scores.at(location);

		// Delete entry from vectors.
		_scores.erase(_scores.begin() + location);
		_healthLeft.erase(_healthLeft.begin() + location);
		_times.erase(_times.begin() + location);
		_attempts.erase(_attempts.begin() + location);

		// Check to see if the best score is repeated
		location = -1;
		while (!bestFound)
		{
			for (int k = 0; k < _scores.size(); k++)
			{
				if (_scores.at(k) == bestScore)
				{
					if (_attempts.at(k) > completed)
					{
						location = k;
						break;
					}
					else if (_healthLeft.at(k) > health)
					{
						location = k;
						break;
					}
					else if (_times.at(k) < time)
					{
						location = k;
						break;
					}
				}
			}

			if (location != -1)
			{
				location = -1;
				// Get accompanying data.
				completed = _attempts.at(location);
				health = _healthLeft.at(location);
				time = _times.at(location);
				bestScore = _scores.at(location);

				// Delete entry from vectors.
				_scores.erase(_scores.begin() + location);
				_healthLeft.erase(_healthLeft.begin() + location);
				_times.erase(_times.begin() + location);
				_attempts.erase(_attempts.begin() + location);
			}
			else
			{
				bestFound = true;
			}
		}

		cout << endl << "Best Score: " << "\t" << bestScore << endl;
		if (completed == 0)
		{
			cout << "Completed?: " << "\t" << "NO" << endl;
		}
		else
		{
			cout << "Completed?: " << "\t" << "YES"<< endl;
		}
		cout << "Health: " << "\t" << health << endl;
		cout << "Time Taken: " << "\t" << time << endl;

	}
	else
	{
		cout << "Ranking not recognised. Performance Statistics will not be generated." << endl;
	}
}
