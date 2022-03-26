#pragma once

#include "stdafx.h"
#include "ingame_base.h"

using namespace gameEngine;
using namespace gameUtils;


namespace ingame::title
{
	class TitleManager : public LuaActor
	{
		Sprite* mLogoSpr;
	public:
		TitleManager();
	private:
		void startLogo();
	};
}

