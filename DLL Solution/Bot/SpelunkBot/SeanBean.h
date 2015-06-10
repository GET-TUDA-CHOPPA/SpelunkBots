#pragma once
#include "IBot.h"

class SeanBean : public IBot
{
public:
	SeanBean() { }
	~SeanBean() { }

	void Update() override;
};