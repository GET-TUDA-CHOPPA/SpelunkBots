#pragma once
#include "IBot.h"

class BasicBotExampleTwo : public IBot
{
private:
	bool					_canGoRight;
	bool					_canGoLeft;
	bool					_canJumpRight;
	bool					_canJumpLeft;
	bool					_canJumpGrabRight;
	bool					_canJumpGrabLeft;

public:
	BasicBotExampleTwo() { }
	virtual ~BasicBotExampleTwo() { }

	void					Update() override;
};

