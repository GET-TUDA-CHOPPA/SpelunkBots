#pragma once
#include "IBot.h"

class Indie : public IBot
{
public:
	Indie() { }
	~Indie() { }

	void Update() override;
};