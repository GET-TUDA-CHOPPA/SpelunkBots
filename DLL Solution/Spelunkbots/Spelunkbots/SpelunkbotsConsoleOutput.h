#pragma once
#include <map>
#include <string>
#include <iostream>

#define GMEXPORT extern "C" __declspec (dllexport)

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

/*
	UpdatePlayerVariables adds or updates the variables in the SpelunkbotsConsoleOutput class that are displayed to the console.

	name: name of the variable
	value: value of the variable
	type: type of the variable

	GM script: UpdatePlayerVariables
*/
GMEXPORT double UpdatePlayerVariables(char *name, char *value, double type);

/*
	DisplayMessages calls the DisplayVariables in the SpelunkbotsConsoleOutput class to print the stored variables to the console.

	This variables will only be printed if one of them has changed.

	GM script: DisplayMessages
*/
GMEXPORT double DisplayMessages();