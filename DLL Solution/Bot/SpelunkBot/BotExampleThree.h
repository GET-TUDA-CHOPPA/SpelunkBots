#pragma once
#include "IBot.h"

class BotExampleThree : public IBot
{
public:
	BotExampleThree() { };
	virtual ~BotExampleThree() { };

	void Update() override;
};

