#include "stdafx.h"
#include "title_scene.h"
#include "start.h"

namespace ingame::title
{
	TitleManager::TitleManager() : SelfDrawingActor()
	{
		startLogo();
		startBack();
		new Walker(0);
		new Walker(1);
	}
	void TitleManager::update()
	{
		double temp = 200.0;
		mBackSpr->SetXY(0, -(mTime%(int(32 * temp)))/temp);

		if (Input::Sole->GetKeyDown(KEY_INPUT_SPACE))
		{
			TitleScene::Sole->EnableExit();
		}

		SelfDrawingActor::update();
	}

	void TitleManager::drawing(int hX, int hY)
	{
		std::string str = "presented by sashi";
		int w = DxLib::GetDrawStringWidthToHandle(str.c_str(), (int)(DxLib::strlenDx(str.c_str())), Fonts->Font12Edged->GetHandler()) * 2;
		DxLib::DrawExtendStringToHandle(GRID_WIDTH*PX_PER_GRID/2-w/2, GRID_HEIGHT*PX_PER_GRID*3/4, 2, 2, str.c_str(), DxLib::GetColor(32, 32, 32),
			Fonts->Font12Edged->GetHandler(), DxLib::GetColor(255, 255, 255));
	}

	void TitleManager::startLogo()
	{
		mLogoSpr = new Sprite(Images->UiTitleLogo);
		//mLogoSpr->SetDrawingMethod(Sprite::DrawingKind::DotByDot);

		int w, h;
		DxLib::GetGraphSize(Images->UiTitleLogo->GetHandler(), &w, &h);
		mLogoSpr->SetXY(GRID_WIDTH/2 - w/2, GRID_HEIGHT*3/8 - h/2);
	}

	void TitleManager::startBack()
	{
		mBackSpr = new Sprite();
		mBackSpr->SetImage(const_cast<Graph*>(Graph::NONE));
		mBackSpr->SetZ(10);
		mBackSpr->SetDrawingMethod([](Sprite* hSpr, int hX, int hY) {
			for (int x = 0; x < GRID_WIDTH; x += 32)
			{
				for (int y = hY; y < GRID_HEIGHT; y += 32)
				{
					Images->UiTitleBack->DrawGraph(x * PX_PER_GRID, y * PX_PER_GRID, 0, 0, 32, 32, PX_PER_GRID);
				}
			}
		});
	}

	
	Walker::Walker(int id) : Actor()
	{
		mId = id;

		switch (mId)
		{
		case 0:
			mY = -32;
			mX = 20 * 16;
			break;
		case 1:
			mX = -32;
			mY = 12* 16;
			break;
		default:
			break;
		}
	}

	void Walker::update()
	{
		double vel = 40;
		switch (mId)
		{
		case 0:
			mY += vel * Time::DeltaSec();

			mSpr->SetXY(mX, mY);
			mSpr->SetImage(Images->Kisaragi, (mTime % 800) / 200 * 32, 32 * 3, 32, 32);

			if (mY > GRID_HEIGHT)
			{
				mY = -32;
			}
			break;
		case 1:
			mX += vel * Time::DeltaSec();

			mSpr->SetXY(mX, mY);
			mSpr->SetImage(Images->Kisaragi, (mTime % 1200) / 200 * 32, 32 * 4, 32, 32);

			if (mX > GRID_WIDTH)
			{
				mX = -32;
			}
			break;
		default:
			break;
		}

		Actor::update();
	}

}

