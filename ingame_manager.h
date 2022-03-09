#pragma once
#define NOMINMAX 

#include <iostream>
#include <string.h>
#include <map>
#include <vector>
#include <math.h>


#include "magic_enum.h"

#include "game_utils.h"
#include "game_engine.h"
#include "lua_manager.h"


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
			grass,
			tree,
			house,
			weed,
		};

		enum class ECharacterKind
		{
			none,
			player,
			punicat,
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
			std::vector<byte> HasChip{}; // std::vector<bool>�͒x���炵���̂�byte�ŊǗ�
			bool IsWall = false;
			bool IsBridge = false;
			bool IsStep[4] = {false, false, false, false};
			
			EventElement Events{};

			void Update();
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
			void installCharacter(double x, double y, std::string character, std::string name);

		public:
			MapManager(int stageIndex);
			~MapManager();
			int GetWidth();
			int GetHeight();
			bool IsInRange(int x, int y);
			MapMatElement* GetMatAt(int x, int y);
			Graph* GetTilesetGraph();
		};



		class ScrollManager : public Actor, public ISingleton<ScrollManager>
		{
			double mX{}, mY{};
		public:
			ScrollManager();
			~ScrollManager();
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
		/// TileMap���C���[�`��̊��N���X
		/// drawingChip����������K�v������
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
			/// �X�̃}�b�v�`�b�v�̕`��
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

