#include "main_scene_effect.h"
#include "start.h"

namespace ingame::main::effect
{
	CloudController::CloudController() : Actor()
	{
		for (int i = 0; i < 64; ++i)
		{
			new Cloud(this);
		}
	}

	void CloudController::update()
	{
		double rateY = (ScrollManager::Sole->GetY()- ScrollManager::Sole->GetMaxXY().Y) / (ScrollManager::Sole->GetMinXY().Y - ScrollManager::Sole->GetMaxXY().Y);

		mSpr->SetXY(ScrollManager::Sole->GetX(), THICKNESS *(-1+2*rateY));
	}

	Cloud::Cloud(CloudController* parent) : Actor()
	{
		mSpr->SetLinkActive(parent->GetSpr());
		mSpr->SetLinkXY(parent->GetSpr());
		
		int r = Rand->Get(3);
		if (r == 0)
		{
			mSpr->SetImage(Images->Cloud[0], 0, 0, 32, 16);
			mSprOriginX = -16; mSprOriginY = -8;
		}
		else if (r == 1)
		{
			mSpr->SetImage(Images->Cloud[1], 0, 0, 64, 32);
			mSprOriginX = -32; mSprOriginY = -16;
		}
		else if (r == 2)
		{
			mSpr->SetImage(Images->Cloud[1], 0, 0, 128, 64);
			mSprOriginX = -64; mSprOriginY = -32;
		}

		mOriginY = CloudController::THICKNESS / 4 - Rand->Get(CloudController::THICKNESS);
		if (Rand->Get(2) == 0) mOriginY = GRID_HEIGHT - mOriginY;

		mX = Rand->Get(GRID_WIDTH);
		mY = mOriginY;

		mVelX = 5 + Rand->Get(45);
		mVelX *= Rand->Get(2) == 0 ? 1 : -1;
		mAmplitude = 2+Rand->Get(16);
			
		mSpr->SetFlip(Rand->Get(2)==0 ? false : true);
		mSpr->SetBlendPal(224);

		mSpr->SetZ(ZIndex::CLOUD);

		mUpdateTimer = EventTimer([&]() {updateCloud(); return true; }, 20);
	}


	void Cloud::update()
	{
		mUpdateTimer.Update();

		Actor::update();
	}

	void Cloud::updateCloud()
	{
		double deltaSec = 20.0 / 1000;
		int x, y;
		mSpr->GetScreenXY(&x, &y);
		if (mVelX < 0 && x < mSprOriginX * 2)
		{
			mX += -x - mSprOriginX * 2 + GRID_WIDTH + (mSprOriginX * -1);
		}

		if (mVelX > 0 && x > GRID_WIDTH)
		{
			mX += -x - (mSprOriginX * 2 * -1);
		}

		mY = mOriginY + mAmplitude * std::sin((mTime / mAmplitude) / 180.0 * M_PI);

		mX += mVelX * deltaSec;
		mSpr->SetXY(mX + mSprOriginX, mY + mSprOriginY);
	}
}

