#pragma once
#include "IBot.h"

class GoldDigger : public IBot
{
public:
	GoldDigger() { }
	~GoldDigger() { }

	void Update() override;
};