#include "main_scene_effect.h"
#include "start.h"

namespace ingame::main::effect
{
	CloudController::CloudController() : Actor()
	{
		for (int i = 0; i < 200; ++i)
		{
			new Cloud(this);
		}
	}

	void CloudController::update()
	{
		mSpr->SetXY(ScrollManager::Sole->GetX(), 0);
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

		mOriginY = Rand->Get(64);
		if (Rand->Get(2) == 0) mOriginY = GRID_HEIGHT - mOriginY;

		mX = Rand->Get(GRID_WIDTH);
		mY = mOriginY;

		mVelX = 5 + Rand->Get(45);
		mVelX *= Rand->Get(2) == 0 ? 1 : -1;
			
		mSpr->SetFlip(Rand->Get(2)==0 ? false : true);
		mSpr->SetBlendPal(224);

		mSpr->SetZ(ZIndex::CLOUD);
	}


	void Cloud::update()
	{
		int x, y;
		mSpr->GetScreenXY(&x, &y);
		if (mVelX<0 && x < mSprOriginX * 2)
		{
			mX += -x - mSprOriginX * 2 + GRID_WIDTH + (mSprOriginX*-1);
		}
			
		if (mVelX > 0 && x > GRID_WIDTH)
		{
			std::cout << mX << " " << -x << " " << -mSprOriginX << "\n";
			mX += -x - (mSprOriginX*2 * -1);
			std::cout << mX << "\n";

		}

		mX += mVelX * Time::DeltaSec();
		mSpr->SetXY(mX+mSprOriginX, mY+mSprOriginY);

		Actor::update();
	}
}

