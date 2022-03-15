#pragma once

#include "stdafx.h"
#include <iostream>
#include <string.h>
#include <map>
#include <vector>
#include <math.h>





using namespace gameUtils;
using namespace gameEngine;

namespace ingame
{

	namespace main
	{
		enum class ETileName
		{
			none,
			water_place,
			sand_pit,
			meadows,
			rock_zone,
			grass,
			tree,
			house,
			weed,
			castle,
		};

		enum class ECharacterKind
		{
			none,
			player,
			punicat,
			slime,
			sheep,
			chick,
			chicken,
			skull,
		};
		class INonPlayerCharacter
		{
		public:
			INonPlayerCharacter(ECharacterKind characterKind, std::string uniqueName);
		protected:
			ECharacterKind mCharacterKind;
			std::string mUniqueName;
		};

		struct TileMapChip
		{
			ETileName Name;
			bool IsWall;
			bool IsBridge;
			bool IsStepUp = false;
			bool IsStepLeft = false;
			bool IsStepRight = false;
			int srcX;
			int srcY;
		};

		struct EventElement
		{
			std::vector<std::string> TouchEvents{};
			std::vector<std::string> ReachEvents{};
		};

		struct MapMatElement
		{
			std::vector<TileMapChip*> Chips{};
			std::vector<byte> HasChip{}; // std::vector<bool>は遅いらしいのでbyteで管理
			bool IsWall = false;
			bool IsBridge = false;
			bool IsStep[4] = {false, false, false, false};
			//short CharacterCount = 0;
			
			EventElement Events{};

			void update();
			~MapMatElement();
		};

		struct UniqueEventValue
		{
			int X{};
			int Y{};
		};

		class MapManager : public Singleton<MapManager>
		{
		private:
			Graph* mTilesetGraph;
			std::map<int, TileMapChip*> mTilechips{};

			int mWidth;
			int mHeight;
			std::vector<std::vector<MapMatElement*>> mMat{};
			std::map<std::string, UniqueEventValue*> mUniqueEvents;
		private:
			bool doStartChip(int x, int y, ETileName tile);
			void loadTileLayer(sol::table layer);
			void loadObjectGroup(sol::table layer);

		public:
			MapManager(int stageIndex);
			~MapManager();
			int GetWidth();
			int GetHeight();
			bool IsInRange(int x, int y);
			MapMatElement* GetMatAt(int x, int y);
			Graph* GetTilesetGraph();
			UniqueEventValue* GetUniqueEvent(std::string key);
		};



		class ScrollManager : public Actor, public ISingleton<ScrollManager>
		{
			double mX{}, mY{};
			int mMinX{}, mMinY{};
			int mMaxX{}, mMaxY{};
		public:
			ScrollManager();
			~ScrollManager();
			double GetX();
			double GetY();
			useful::Vec2<int> GetMinXY();
			useful::Vec2<int> GetMaxXY();
			void SetRange(int minX, int minY, int maxX, int maxY);
		protected:
			void update() override;
		};



		class BackGroundManager : public SelfDrawingActor
		{
		public:
			BackGroundManager();
		protected:
			void update() override;
			void drawing(int hX, int hY) override;
		};



		/// <summary>
		/// TileMapレイヤー描画の基底クラス
		/// drawingChipを実装する必要がある
		/// </summary>
		class FieldLayerBase : public SelfDrawingActor
		{
		protected:
			int mGridUnit = 16;
			double mZ;
		public:
			FieldLayerBase(double z);
		protected:
			void drawing(int hX, int hY) override;
			/// <summary>
			/// 個々のマップチップの描画
			/// </summary>
			/// <param name="matX">Matrix X</param>
			/// <param name="matY">Matrix Y</param>
			/// <param name="dpX">Display X</param>
			/// <param name="dpY">Display Y</param>
			virtual void drawingChip(int matX, int matY, int dpX, int dpY, TileMapChip* chip) = 0;

			void drawingAutoTile(int matX, int matY, int dpX, int dpY,
				Graph* srcImage, int srcX, int srcY, std::function<bool(int x, int y)> canConnect);
			bool canConnect(int x, int y, ETileName tile);
			bool canConnect(int x, int y, ETileName tile1, ETileName tile2);
			bool canConnect(int x, int y, ETileName tile1, ETileName tile2, ETileName tile3);
			bool canConnect(int x, int y, ETileName tile1, ETileName tile2, ETileName tile3, ETileName tile4);
		};

		class FloorLayer : public FieldLayerBase
		{
		public:
			FloorLayer();
		protected:
			void drawingChip(int matX, int matY, int dpX, int dpY, TileMapChip* chip) override;
		};
	}
}

