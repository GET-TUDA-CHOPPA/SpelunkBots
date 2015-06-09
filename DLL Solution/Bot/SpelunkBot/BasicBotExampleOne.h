#pragma once

#include "IBot.h"

class BasicBotExampleOne : public IBot
{
public:
	BasicBotExampleOne() { };
	virtual ~BasicBotExampleOne() { };

	void Update() override;
};