#pragma once
#include "stdafx.h"
#include "main_scene.h"


namespace ingame::main
{
	class GameController : public LuaActor, public ISingleton<GameController>
	{
		int mGameTime;
		EventTimer mCountDownTimer;
	public:
		GameController();
		~GameController();
		std::vector<std::function<void(int minu, int sec)>> OnGameTimeChanged{};
	private:
		void startCountDown();
	protected:
		void update() override;
	public:
		static const int GAME_TIME_START = 300;
	};
}







