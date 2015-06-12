#pragma once
#include "IBot.h"

class FishBot : public IBot
{
public:
	FishBot() { }
	~FishBot() { }

	void Update() override;

private:
	int facing;
	int numberOfCreatures;
};