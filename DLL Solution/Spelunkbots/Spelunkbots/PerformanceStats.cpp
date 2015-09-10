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
	_botID = "";
	_unrecognisedData = false;
	_testNumber = 0;
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
	_botID = "";
	_unrecognisedData = false;
	_testNumber = 0;
}

void PerformanceStats::SetTestType(char* testType)
{
	string str = testType;
	transform(str.begin(), str.end(), str.begin(), ::toupper);
	if (str.compare("TESTMAPS") == 0 || str.compare("MARATHON") == 0)
	{
		_testType = str;
		cout << "Test Type: " << _testType << endl;
	}
	else
	{
		_unrecognisedData = true;
		cout << "Test type not recognised." << endl;
	}
}

void PerformanceStats::SetRanking(char* ranking)
{
	string str = ranking;
	transform(str.begin(), str.end(), str.begin(), ::toupper);
	if (str.compare("SCORE") == 0 || str.compare("TIME") == 0)
	{
		_ranking = str;
		cout << "Ranking: " << _ranking << endl;
	}
	else
	{
		_unrecognisedData = true;
		cout << "Ranking not recognised." << endl;
	}
}

void PerformanceStats::SetBotID(char* ID)
{
	_botID = ID;
	cout << "BotID: " << _botID << endl;
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
		_exitX = val + 8;
	}
	else if (strcmp(stat, "EXITY") == 0)
	{
		_exitY = val + 8;
	}
	else if (strcmp(stat, "ENTRANCEX") == 0)
	{
		_entranceX = val + 8;
	}
	else if (strcmp(stat, "ENTRANCEY") == 0)
	{
		_entranceY = val + 8;
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
	if (!_unrecognisedData)
	{
		if (_botID.compare("") == 0)
		{
			auto time = std::chrono::system_clock::now();
			auto time_t = chrono::system_clock::to_time_t(time);
			stringstream dateString;

			dateString << put_time(std::localtime(&time_t), "%d-%m-%y");
			_botID = dateString.str();
		}

		bool error = false;
		_testNumber++;
		error = CheckFolderState();

		if (!error)
		{
			if (_testType.compare("TESTMAPS") == 0)
			{
				TestMaps();
			}
			else if (_testType.compare("MARATHON") == 0)
			{
				Marathon();
			}
		}
	}
	else
	{
		cout << "The test type or ranking you have chosen is unrecongnised. Please modify and restart Spelunky." << endl;
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
	ofstream fileStream;
	ostringstream concat;
	string directoryName = "";
	// The reason for not putting avgScore and avgTime behind the if statements that check the ranking below (even though they may not be used depending on the ranking) 
	// is beacuse the performance impact of doing some simple calculations is less than setting up two new loop structures that will loop through data that was already looped through previously.
	int bestScore = 0;
	int avgScore = 0;
	int avgHealth = 0;
	double avgTime = 0.0;
	int successes = 0;
	double bestTime = 0.0;
	if (_times.size() > 0)
	{
		bestTime = _times.at(0);
	}

	for (int i = 0; i < _scores.size(); i++)
	{
		avgScore += _scores.at(i);
		avgHealth += _healthLeft.at(i);
		successes += _attempts.at(i);
		if (_scores.at(i) > bestScore)
		{
			bestScore = _scores.at(i);
		}
	}

	for (int k = 0; k < _times.size(); k++)
	{
		avgTime += _times.at(k);
		if (_times.at(k) < bestTime)
		{
			bestTime = _times.at(k);
		}
	}

	avgScore = avgScore / _scores.size();
	avgHealth = avgHealth / _healthLeft.size();
	if (_times.size() > 0)
	{
		avgTime = avgTime / _times.size();
	}

	//Put data in files
	concat.str("");
	concat << "..\\..\\Performance\\" << _botID << "\\" << _testType << "\\" << _ranking << "\\" << "Result_" << _testNumber << ".txt";
	directoryName = concat.str();
	fileStream.open(directoryName.c_str(), ofstream::out);
	if (fileStream.is_open())
	{
		if (_ranking.compare("SCORE") == 0)
		{
			fileStream << "BEST SCORE,AVERAGE SCORE,SUCCESSFUL ATTEMPTS,AVERAGE REMAINING HEALTH,BEST TIME" << endl;
			fileStream << bestScore << "," << avgScore << "," << successes << "," << avgHealth << "," << bestTime;
		}
		else if (_ranking.compare("TIME") == 0)
		{
			fileStream << "BEST TIME,SUCCESSFUL ATTEMPTS,AVERAGE TIME,AVERAGE REMAINING HEALTH,BEST SCORE" << endl;
			fileStream << bestTime << "," << successes << "," << avgTime << "," << avgHealth << "," << bestScore;
		}
		cout << "Performance data written to file." << endl;
	}
	else
	{
		cout << "Unable to write performance data to file." << endl;
	}
	fileStream.close();
}

void PerformanceStats::Marathon()
{
	ofstream fileStream;
	ostringstream concat;
	string directoryName = "";

	double timeTaken = 0;
	for (int i = 0; i < _times.size(); i++)
	{
		timeTaken += _times.at(i);
	}

	//Put data in files
	concat.str("");
	concat << "..\\..\\Performance\\" << _botID << "\\" << _testType << "\\" << _ranking << "\\" << "Result_" << _testNumber << ".txt";
	directoryName = concat.str();
	fileStream.open(directoryName.c_str(), ofstream::out);
	if (fileStream.is_open())
	{
		if (_ranking.compare("SCORE") == 0)
		{
			fileStream << "SCORE,COMPLETED,TIME TAKEN,HEALTH" << endl;
			if (_attempts.at(0) != 17) // 17 is the number passed when the end room is reached which is counted as a "17th" level. 
			{
				fileStream << _scores.at(0) << "," << "NO," << timeTaken << "," << 0;
			}
			else
			{
				fileStream << _scores.at(0) << "," << "YES," << timeTaken << "," << _healthLeft.at(0);
			}
		}
		else if (_ranking.compare("TIME") == 0)
		{
			fileStream << "COMPLETED,LEVEL DIED,TIME TAKEN UNTIL DEATH ON LEVEL,TIME TAKEN OVERALL,DISTANCE TRAVELLED TO EXIT,SCORE";
			for (int i = 0; i < 16; i++)
			{
				fileStream << ",Time Taken to complete Level " << i + 1;
			}
			fileStream << endl;
			if (_attempts.at(0) != 17) //Didn't complete game
			{				
				
				fileStream << "NO," << _times.size() << "," << _times.at(_times.size() - 1) << "," << timeTaken << "," << CalcDistanceTraveled() << "," << _scores.at(0);
				for (int k = 0; k < _times.size() - 1; k++)
				{
					fileStream << "," << _times.at(k);
				}
			}
			else
			{
				fileStream << "YES," << "," << "," << timeTaken << "," << "," << _scores.at(0);
				for (int k = 0; k < _times.size(); k++)
				{
					fileStream << "," << _times.at(k);
				}
			}
		}
		cout << "Performance data written to file." << endl;
	}
	else
	{
		cout << "Unable to write performance data to file." << endl;
	}

	fileStream.close();
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

bool PerformanceStats::CheckFolderState()
{
	ofstream fileStream;
	ostringstream concat;
	string directoryName = "";
	concat.str("");
	bool error = false;

	concat << "..\\..\\Performance";
	directoryName = concat.str();
	CreateDirectoryA(directoryName.c_str(), NULL);
	if (ERROR_PATH_NOT_FOUND == GetLastError()) //if ERROR_PATH_NOT_FOUND is != GetLastError this measn that GetLastError is probably equal to either ERROR_ALREADY_EXISTS or nothing. If it is either of these two we can create inner folders 
	{
		error = true;
		cout << "Unable to Create Performance Folder." << endl;
	}

	if (!error)
	{
		concat << "..\\..\\Performance\\" << _botID;
		directoryName = concat.str();
		CreateDirectoryA(directoryName.c_str(), NULL);
		if (ERROR_PATH_NOT_FOUND == GetLastError()) //if ERROR_PATH_NOT_FOUND is != GetLastError this measn that GetLastError is probably equal to either ERROR_ALREADY_EXISTS or nothing. If it is either of these two we can create inner folders 
		{
			error = true;
			cout << "Unable to Create " << _botID << " Folder." << endl;
		}
	}

	if (!error)
	{
		concat.str("");
		concat << "..\\..\\Performance\\" << _botID << "\\" << _testType;
		directoryName = concat.str();		
		CreateDirectoryA(directoryName.c_str(), NULL);
		if (ERROR_PATH_NOT_FOUND == GetLastError())
		{
			error = true;
			cout << "Unable to Create " << _testType << " Folder." << endl;
		}
	}	

	if (!error)
	{
		concat.str("");
		concat << "..\\..\\Performance\\" << _botID << "\\" << _testType << "\\" << _ranking;
		directoryName = concat.str();
		CreateDirectoryA(directoryName.c_str(), NULL);
		if (ERROR_PATH_NOT_FOUND == GetLastError())
		{
			error = true;
			cout << "Unable to Create " << _ranking << " Folder." << endl;
		}
	}

	if (!error)
	{
		if (_testNumber == 1) // First Run
		{
			bool exists = true;
			while (exists)
			{
				concat.str("");
				concat << "..\\..\\Performance\\" << _botID << "\\" << _testType << "\\" << _ranking << "\\" << "Result_" << _testNumber << ".txt";
				directoryName = concat.str();
				fileStream.open(directoryName.c_str(), ofstream::in);
				if (!fileStream.is_open())
				{
					fileStream.close();
					break;
				}
				fileStream.close();
				_testNumber++;
			}
		}
	}
	return error;
}
