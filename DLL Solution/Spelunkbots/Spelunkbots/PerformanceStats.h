#pragma once
#include <vector>
#include <iostream>
#include <string>
#include<algorithm>
using namespace std;

class PerformanceStats
{
public:
	PerformanceStats();
	~PerformanceStats();
	void SetTestType(char* testType);
	void SetRanking(char* ranking);
	void Assigner(double val, char* stat);
	void CalculatePerformance();
	void Clear();

private:
	vector<int> _gold;
	vector<int> _successfulAttempts;
	vector<int> _healthLeft;
	vector<int> _completionTimes;
	string _testType;
	string _ranking;
};

