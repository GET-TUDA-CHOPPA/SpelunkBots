#pragma once
#include "IBot.h"

class BasicBotExampleThree : public IBot
{
public:
	BasicBotExampleThree() { }
	~BasicBotExampleThree() { }

	void Update() override;
};