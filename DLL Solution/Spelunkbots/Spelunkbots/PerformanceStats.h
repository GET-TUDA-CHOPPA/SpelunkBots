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
	void PerformanceStats::TestMaps();
	void PerformanceStats::Marathon();

private:
	vector<int> _scores;
	vector<int> _healthLeft;
	vector<int> _times;
	vector<int> _attempts;
	string _testType;
	string _ranking;
};

