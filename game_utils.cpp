
#include "stdafx.h"
#include "game_utils.h"

namespace gameUtils
{
	Graph::Graph(int handler)
	{
		mHandler = handler;
	}

	Graph::~Graph()
	{
		if (mHandler!=-1) DxLib::DeleteGraph(mHandler);
	}

	bool Graph::operator==(Graph* r)
	{
		return this->mHandler == r->mHandler;
	}


	int Graph::GetHandler()
	{
		return mHandler;
	}

	void Graph::DrawGraph(int x, int y, int u, int v, int width, int height, double scale)
	{
		this->DrawGraph(x, y, u, v, width, height, scale, 0.0);
	}

	void Graph::DrawGraph(int x, int y, int u, int v, int width, int height, double scale, double rotationRad)
	{
		this->DrawGraph(x, y, u, v, width, height, scale, rotationRad, false);
	}

	void Graph::DrawGraph(int x, int y, int u, int v, int width, int height, double scale, double rotationRad, bool isFlip)
	{
		int image = DxLib::DerivationGraph(u, v, width, height, mHandler);

		if (rotationRad == 0)
		{
			DxLib::DrawRotaGraph3(x, y, 0, 0, scale, scale, 0, image, 1, isFlip);
		}
		else
		{
			int x1 = width / 2, y1 = height / 2;
			double cos = std::cos(rotationRad), sin = std::sin(rotationRad);

			double x2 = x1 * cos - y1 * sin, y2 = x1 * sin + y1 * cos;

			double dx = x1 - x2, dy = y1 - y2;

			DxLib::DrawRotaGraph3(x + int(dx * scale), y + int(dy * scale),
				0, 0, // 回転中心
				scale, scale,
				rotationRad, image,
				1, isFlip);
		}
		DxLib::DeleteGraph(image);
	}

	Graph* Graph::LoadGraph(const char* path)
	{
		int handler = DxLib::LoadGraph(path);
		return new Graph(handler);
	}

	const Graph* Graph::NONE = new Graph(-1);

	
	namespace useful
	{

		Random::Random() : engine(device()), distribution(0, INT_MAX - 1)
		{
		}
		Random::Random(int seed) : engine(seed), distribution(0, INT_MAX - 1)
		{
		}
		int Random::Get(int max)
		{
			return distribution(engine) % max;
		}
		int Random::Get(int min, int max)
		{
			return min + (distribution(engine) % (max - min));
		}


		bool Between(double n, double min, double max)
		{
			return (min <= n && n <= max);
		}

		std::string GetDateTimeStr()
		{
			// 参考: https://replication.hatenablog.com/entry/2014/07/28/121117

			SYSTEMTIME st;
			GetSystemTime(&st);
			char szTime[25] = { 0 };

			// wHourを9時間足して、日本時間にする
			wsprintf(szTime, "%04d/%02d/%02d %02d:%02d:%02d %03d",
				st.wYear, st.wMonth, st.wDay,
				st.wHour + 9, st.wMinute, st.wSecond, st.wMilliseconds);
			return szTime;
		}

		/// <summary>
		/// ワイド文字列からマルチバイト文字列へ
		/// 参考: https://kryozahiro.hateblo.jp/entry/20080809/1218295912
		/// setlocale(LC_CTYPE, ""); する必要あり
		/// </summary>
		/// <param name="src"></param>
		/// <param name="dest"></param>
		void WideStrToNarrowStr(const std::wstring& src, std::string& dest) 
		{
			char* mbs = new char[src.length() * MB_CUR_MAX + 1];
			wcstombs(mbs, src.c_str(), src.length() * MB_CUR_MAX + 1);
			dest = mbs;
			delete[] mbs;
		}
		/// <summary>
		/// マルチバイト文字列からワイド文字列
		/// </summary>
		/// <param name="src"></param>
		/// <param name="dest"></param>
		void NarrowStrToWideStr(const std::string& src, std::wstring& dest) 
		{
			wchar_t* wcs = new wchar_t[src.length() + 1];
			mbstowcs(wcs, src.c_str(), src.length() + 1);
			dest = wcs;
			delete[] wcs;
		}

	}


	Input::Input() : Singleton<Input>()
	{}

	bool Input::GetKeyDown(int key)
	{
		return DxLib::CheckHitKey(key);
	}

}

