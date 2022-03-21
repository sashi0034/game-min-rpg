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
	void StartEffect();

	class CloudController : public Actor
	{
	public:
		CloudController();
	protected:
		void update() override;
	public:
		static const int THICKNESS = 16;
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

	class SpiritController : public Actor
	{
	public:
		SpiritController();
	protected:
		void update() override;
	};

	class Spirit : public Actor
	{
		static const useful::Vec2<int> imageSize;
		useful::Vec2<int> mSprOriginPt;
		useful::Vec2<double> mPt;
		useful::Vec2<double> mVel;
		int mAnimTIme{};
	public:
		Spirit(SpiritController* parent);
	protected:
		void update() override;
	private:
		void updateSpirit(int deltaMilli);
	};


}


