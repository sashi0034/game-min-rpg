#pragma once

#include "stdafx.h"
#include "game_utils_template.h"



namespace gameUtils
{
	class Graph
	{
	private:
		int mHandler;
	public:
		Graph(int handler);
		~Graph();
		int GetHandler();
		void DrawGraph(int x, int y, int u, int v, int width, int height, double scale);
		void DrawGraph(int x, int y, int u, int v, int width, int height, double scale, double rotationRad);
		void DrawGraph(int x, int y, int u, int v, int width, int height, double scale, double rotationRad, bool isFlip);

		bool operator ==(Graph* r);

		static Graph* LoadGraph(const char* path);
		static const Graph* NONE;
	};

	class Sound
	{
	private:
		int mHandler;
		int mVolume = 255;
		bool mIsLoop = false;;
	public:
		Sound(int handler);
		~Sound();
		int GetHandler();
		void SetVolume(int vol);
		int GetVolume();
		void SetLoop(bool isLoop);
		void Play();
		void Stop();

		static Sound* LoadSound(const char* path);
		static const Sound* NONE;
	};



	class Input : public Singleton<Input>
	{
	private:

	public:
		Input();
		bool GetKeyDown(int key);
	};

	namespace useful
	{

		class Random
		{
			std::random_device device;
			std::default_random_engine engine;
			std::uniform_int_distribution<int> distribution;

		public:
			Random(void);
			Random(int seed);
			int Get(int max);
			int Get(int min, int max);
		};

		bool Between(double n, double min, double max);
		double Distance(double x, double y);
		std::string GetDateTimeStr();
		void WideStrToNarrowStr(const std::wstring& src, std::string& dest);
		void NarrowStrToWideStr(const std::string& src, std::wstring& dest);
	}

	



}


