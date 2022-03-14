#pragma once

#include <iostream>
#include <string.h>
#include <map>
#include <vector>

#include "game_utils.h"
#include "game_engine.h"

#include "ingame_manager.h"
#include "ingame_base.h"


namespace ingame::main::effect
{
	class CloudController : public Actor
	{
	public:
		CloudController();
	protected:
		void update() override;
	public:
		static const int THICKNESS = 32;
	};

	class Cloud : public Actor
	{
		int mSprOriginX{};
		int mSprOriginY{};
		double mX;
		double mY, mOriginY;
		int mAmplitude;
		double mVelX;
		EventTimer mUpdateTimer;
	public:
		Cloud(CloudController* parent);
	protected:
		void update() override;
		void updateCloud();
	};


}


