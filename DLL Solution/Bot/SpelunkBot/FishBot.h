#pragma once
#include "IBot.h"

class FishBot : public IBot
{
public:
	FishBot() : _facing{ 1 }, _numberOfCreatures{ 0 } { }
	~FishBot() { }

	void Update() override;

private:
	int _facing;
	int _numberOfCreatures;
};