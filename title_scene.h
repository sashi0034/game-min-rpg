#pragma once

#include "stdafx.h"
#include "ingame_base.h"

using namespace gameEngine;
using namespace gameUtils;


namespace ingame::title
{
	class TitleManager : public SelfDrawingActor
	{
		Sprite* mBackSpr;
		Sprite* mLogoSpr;
	public:
		TitleManager();
	protected:
		void update() override;
		void drawing(int hX, int hY) override;
	private:
		void startLogo();
		void startBack();
		void startDecoration();
	};

	class Walker : public Actor
	{
		int mId{};
		double mX, mY;
	public:
		Walker(int id);
	protected:
		void update() override;
	};
}

