#pragma once
#include "IBot.h"

class JordanBot : public IBot
{
private:
	bool					_canGoRight;
	bool					_canGoLeft;
	bool					_canJumpRight;
	bool					_canJumpLeft;
	bool					_canJumpGrabRight;
	bool					_canJumpGrabLeft;

public:
	JordanBot() { }
	virtual ~JordanBot() { }

	void					Update() override;
};

