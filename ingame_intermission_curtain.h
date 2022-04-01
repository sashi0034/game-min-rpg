#pragma once

#include "stdafx.h"
#include "ingame_base.h"

using namespace gameEngine;
using namespace gameUtils;


namespace ingame
{
	class IntermissionCurtain : public Actor
	{
		bool mIsOpenCurtain = false;
		std::function<void()> mCallback;

		bool mIsCompleted = false;

		void startStrips();

		void onComplete();

		IntermissionCurtain(bool isOpen, std::function<void()> callback);
	protected:
		void update() override;
	public:
		
		static IntermissionCurtain* CreateOpen(std::function<void()> callback);
		static IntermissionCurtain* CreateClose(std::function<void()> callback);
	};
}


