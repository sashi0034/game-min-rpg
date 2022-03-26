#include "stdafx.h"
#include "title_scene.h"
#include "start.h"

namespace ingame::title
{
	TitleManager::TitleManager() : LuaActor("TitleManagerLuaData", true)
	{
		startLogo();
	}
	void TitleManager::startLogo()
	{
		mLogoSpr = new Sprite(Images->UiTitleLogo);
		mLogoSpr->SetDrawingMethod(Sprite::DrawingKind::DotByDot);

		int w, h;
		DxLib::GetGraphSize(Images->UiTitleLogo->GetHandler(), &w, &h);
		mLogoSpr->SetXY(GRID_WIDTH/2 - w/2/PX_PER_GRID, GRID_HEIGHT*3/8 - h/2/PX_PER_GRID);
	}
}

