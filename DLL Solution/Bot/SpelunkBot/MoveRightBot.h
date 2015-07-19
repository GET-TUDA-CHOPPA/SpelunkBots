#pragma once

#include "IBot.h"

class MoveRightBot : public IBot
{
public:
	MoveRightBot() { };
	virtual ~MoveRightBot() { };

	void Update() override;
};
