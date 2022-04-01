#include "stdafx.h"
#include "start.h"
#include "ingame_intermission_curtain.h"


namespace ingame
{

	IntermissionCurtain::IntermissionCurtain(bool isOpen, std::function<void()> callback)
	{
		mIsOpenCurtain = isOpen;
		mCallback = callback;

		startStrips();

	}

	void IntermissionCurtain::startStrips()
	{
		// •’Ê‚Ì•ûŒü‚Ö
		for (int y = 0; y < GRID_HEIGHT; y += 32 * 2)
		{
			Sprite* spr = new Sprite();
			spr->SetLinkAlive(mSpr);
			spr->SetZ(double(ZIndex::CURTAIN));

			auto x = std::shared_ptr<double>(new double(0));

			auto updateFn = [spr, x, y, this]() {
				const int scrollSpeed = 8;
				const int width = 1280;
				const int xMax = GRID_WIDTH + 32;

				if (*x == 0)
				{
					//spr->SetImage(Images->UiCurtainStrip, 0, mIsOpenCurtain ? 32 : 0, width, 32);
					spr->SetImage(Images->UiCurtainStrip, 0, 0, width, 32);
				}
				*x = *x + scrollSpeed;

				if (mIsOpenCurtain)
				{
					spr->SetXY(GRID_WIDTH - (xMax - *x), y);
				}
				else
				{
					spr->SetXY(GRID_WIDTH - *x, y);
				}

				if (*x > xMax)
				{
					onComplete();
					return false;
				}

				return true;
			};
			updateFn();
			new EventTimerAsActor(updateFn, int(FPS60_MILLI));
		}

		// ‹t‚Ì•ûŒü‚Ö
		for (int y = 32; y < GRID_HEIGHT; y += 32 * 2)
		{
			Sprite* spr = new Sprite();
			spr->SetLinkAlive(mSpr);
			spr->SetZ(double(ZIndex::CURTAIN));

			auto x = std::shared_ptr<double>(new double(0));

			auto updateFn = [spr, x, y, this]() {
				const int scrollSpeed = 8;
				const int width = 1280;
				const int xMax = GRID_WIDTH + 32;

				if (*x == 0)
				{
					spr->SetImage(Images->UiCurtainStrip, 0, 0, width, 32);
					spr->SetFlip(true);
				}
				*x = *x + scrollSpeed;

				if (mIsOpenCurtain)
				{
					spr->SetXY(-width + (xMax - *x), y);
				}
				else
				{
					spr->SetXY(-width + *x, y);
				}

				if (*x > GRID_WIDTH + 32)
				{
					onComplete();
					return false;
				}

				return true;
			};
			updateFn();
			new EventTimerAsActor(updateFn, int(FPS60_MILLI));
		}
	}

	void IntermissionCurtain::onComplete()
	{
		if (!mIsCompleted)
		{
			mCallback();
			mIsCompleted = true;
			new EventTimerAsActor([this]() { Sprite::Destroy(mSpr); return false; }, 100);
		}
	}

	void IntermissionCurtain::update()
	{
	}

	IntermissionCurtain* IntermissionCurtain::CreateOpen(std::function<void()> callback)
	{
		return new IntermissionCurtain(true, callback);
	}
	IntermissionCurtain* IntermissionCurtain::CreateClose(std::function<void()> callback)
	{
		return new IntermissionCurtain(false, callback);
	}
}






