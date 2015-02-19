#pragma once
#include <map>
#include <string>
#include <iostream>

// Class for holding messages
class SpelunkbotsConsoleOutput
{
public:
	SpelunkbotsConsoleOutput();

	void UpdateVariable(std::string name, std::string value);
	void DisplayVariables();

private:
	std::map<std::string, std::string> variables;

	bool updated;
};