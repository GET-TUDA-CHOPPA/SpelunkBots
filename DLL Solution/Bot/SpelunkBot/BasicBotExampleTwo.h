#pragma once
#include "IBot.h"

class BasicBotExampleTwo : public IBot
{
public:
	BasicBotExampleTwo() { };
	virtual ~BasicBotExampleTwo() { };

	void Update() override;
};

