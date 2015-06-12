// SpelunkBot.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Bot.h"
#include "IBot.h"
#include "BasicBotExampleOne.h"
#include "BasicBotExampleTwo.h"
#include "BasicBotExampleThree.h"
#include "SeanBean.h"
#include "GoldDigger.h"
#include "DiscoveryDan.h"
#include "NotSoSolidSnake.h"
#include "Indie.h"
#include "FishBot.h"
#include <memory>

std::unique_ptr<IBot> bot;

double ConvertBoolToDouble(bool valToConvert)
{
	if (valToConvert)
	{
		return 1;
	}
	return 0;
}
char* ConvertBoolToChar(bool valToConvert)
{
	if (valToConvert)
	{
		return "1";
	}
	return "0";
}

// Add additional bots here
void CreateBot(double botSelector)
{
	switch ((int)botSelector)
	{
		case 1:
			bot = std::make_unique<BasicBotExampleOne>();
			break;
		case 2:
			bot = std::make_unique<BasicBotExampleTwo>();
			break;
		case 3:
			bot = std::make_unique<BasicBotExampleThree>();
			break;
		case 4:
			bot = std::make_unique<SeanBean>();
			break;
		case 5:
			bot = std::make_unique<GoldDigger>();
			break;
		case 6:
			bot = std::make_unique<DiscoveryDan>();
			break;
		case 7:
			bot = std::make_unique<NotSoSolidSnake>();
			break;
		case 8:
			bot = std::make_unique<Indie>();
			break;
		case 9:
			bot = std::make_unique<FishBot>();
			break;
		default:
			break;
	}
}

#pragma region Setup

SPELUNKBOT_API double Initialise(void)
{
	if (bot)
	{
		// Reset ALL variables when entering a new room.
		bot->InitialiseVariables();
	}

	return 1;
}

#pragma endregion

#pragma region Get functions for GM

SPELUNKBOT_API double GetHasGoal(void)
{
	return bot->GetHasGoal();
}
SPELUNKBOT_API double GetHoldingItem(void)
{
	return bot->GetHoldingItem();
}
SPELUNKBOT_API double GetIsInAir(void)
{
	return bot->GetHasGoal();
}
SPELUNKBOT_API double GetIsJetpacking(void)
{
	return bot->GetIsJetpacking();
}
SPELUNKBOT_API double GetIsHanging(void)
{
	return bot->GetIsHanging();
}
SPELUNKBOT_API double GetItemGoal(void)
{
	return bot->GetItemGoal();
}
SPELUNKBOT_API double GetPathCount(void)
{
	return bot->GetPathCount();
}
SPELUNKBOT_API double GetTempID(void)
{
	return bot->GetTempID();
}
SPELUNKBOT_API double GetFogGoal(void)
{
	return bot->GetFogGoal();
}
SPELUNKBOT_API double GetEndGoal(void)
{
	return bot->GetEndGoal();
}
SPELUNKBOT_API double GetWaitTimer(void)
{
	return bot->GetWaitTimer();
}
SPELUNKBOT_API double GetHeadingRight(void)
{
	return bot->GetHeadingRight();
}
SPELUNKBOT_API double GetHeadingLeft(void)
{
	return bot->GetHeadingLeft();
}
SPELUNKBOT_API double GetGoRight(void)
{
	return bot->GetGoRight();
}
SPELUNKBOT_API double GetGoLeft(void)
{
	return bot->GetGoLeft();
}
SPELUNKBOT_API double GetJump(void)
{
	return bot->GetJump();
}
SPELUNKBOT_API double GetTargetX(void)
{
	return bot->GetTargetX();
}
SPELUNKBOT_API double GetTargetY(void)
{
	return bot->GetTargetY();
}
SPELUNKBOT_API double GetAttack(void)
{
	return bot->GetAttack();
}
SPELUNKBOT_API double GetLookUp(void)
{
	return bot->GetLookUp();
}
SPELUNKBOT_API double GetDuck(void)
{
	return bot->GetDuck();
}

#pragma endregion

#pragma region Bot Logic

SPELUNKBOT_API double Update(double botSelector, double botXPos, double botYPos)
{
	if (!bot)
	{
		CreateBot(botSelector);
	}
	else
	{
		// Use Update to select a bot to run
		bot->Reset();
		bot->UpdateBotPosition(botXPos, botYPos);
		bot->Update();
	}

	return 1;
}

#pragma endregion