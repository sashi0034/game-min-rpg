#include "stdafx.h"
#include "main_scene_controller.h"


namespace ingame::main
{
	GameController::GameController() : LuaActor("GameControllerLuaData", true)
	{
		Sole = this;

		mGameTime = GAME_TIME_START;


		startCountDown();
	}
	GameController::~GameController()
	{
		Sole = nullptr;
	}
	void GameController::startCountDown()
	{
		mCountDownTimer = EventTimer([&]()->bool {
			mGameTime--;
			if (mGameTime < 0)
			{
				std::cout OUT_LOG "Enter game over.";
				return false;
			}
			for (auto fn : OnGameTimeChanged) fn(mGameTime/60, mGameTime%60);
			return true;
		}, 1000);
	}
	void GameController::update()
	{
		Actor::update();
		mCountDownTimer.Loop();
	}
}






