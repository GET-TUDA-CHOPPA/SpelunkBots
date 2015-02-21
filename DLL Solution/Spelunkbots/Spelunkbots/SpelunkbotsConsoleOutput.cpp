#include "stdafx.h"
#include "SpelunkbotsConsoleOutput.h"

SpelunkbotsConsoleOutput::SpelunkbotsConsoleOutput()
{
	updated = false;
}

void SpelunkbotsConsoleOutput::UpdateVariable(std::string name, std::string value)
{
	bool exists = false;

	for (auto var : variables)
	{
		if (var.first == name)
		{
			exists = true;
			break;
		}
	}

	if (exists && variables[name] != value)
	{
		variables[name] = value;

		updated = true;
	}
	else if (!exists)
	{
		variables[name] = value;

		updated = true;
	}
}

void SpelunkbotsConsoleOutput::DisplayVariables()
{
	// Only display if a variable has been updated
	if (updated)
	{
		std::cout << "================================" << std::endl;
		for (auto var : variables)
		{
			std::cout << var.first + ": " + var.second << std::endl;
		}

		updated = false;
	}
}
