#pragma once
#include "IBot.h"

class DiscoveryDan : public IBot
{
public:
	DiscoveryDan() { }
	~DiscoveryDan() { }

	void Update() override;
};