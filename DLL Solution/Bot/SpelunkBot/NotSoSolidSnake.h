#pragma once
#include "IBot.h"

class NotSoSolidSnake : public IBot
{
public:
	NotSoSolidSnake() { }
	~NotSoSolidSnake() { }

	void Update() override;
};