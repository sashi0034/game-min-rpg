#pragma once
#include "stdafx.h" 

#define _USE_MATH_DEFINES

#include <random>
#include <any>
#include <map>
#include <math.h>
#include <iostream>

#include "lua_manager.h"
#include "ingame_manager.h"
#include "main_scene.h"
#include "title_scene.h"





using namespace gameUtils;
using namespace gameEngine;









namespace ingame
{
    extern const char* GAME_TITLE_NAME;

    namespace resorce 
    {
        class ImageRes
        {
        private:
            Graph* loadPng(std::string fileName);
        public:
            Graph* Cloud[3] = { 
                loadPng("cloud_32x16"),
                loadPng("cloud_64x32"),
                loadPng("cloud_128x64") };
            Graph* NaturalTile = loadPng("natural_playground_16x16");
            Graph* Weed = loadPng("weed_16x16");
            Graph* Tree = loadPng("tree_16x16");
            Graph* Kisaragi = loadPng("kisaragi_24x24");
            Graph* Punicat = loadPng("punicat_24x24");
            Graph* Slime = loadPng("slime_24x24");
            Graph* Sheep = loadPng("sheep_32x32");
            Graph* Mouse = loadPng("mouse_24x24");
            Graph* Chicken = loadPng("chicken_32x32");
            Graph* Chick = loadPng("chick_16x16");
            Graph* Skull = loadPng("skull_24x24");
            Graph* SkullSmall = loadPng("skull_small_16x16");
            Graph* Castle = loadPng("castle_32x32");
            Graph* RockZone = loadPng("rock_zone_16x16");

            Graph* EffectSpirit = loadPng("effect/spirit_64x64");

            Graph* UiWindowBlack = loadPng("ui/window_black");
            Graph* UiArrow = loadPng("ui/arrow_16x16");
            Graph* UiWhiteRoundRect = loadPng("ui/white_rounnd_rect");
            Graph* UiTitleLogo = loadPng("ui/title_logo");
            Graph* UiTitleBack = loadPng("ui/title_back_32x32");
            Graph* UiCurtainStrip = loadPng("ui/curtain_strip");
            Graph* UiBlackScreen = loadPng("ui/black_screen");

#if 0
            int Templa = LoadGraph(R"(.png)");
#endif // 0

            ImageRes()
            {
            }
        };

        class FontRes
        {
        public:
            Graph* Font12Edged = new Graph(DxLib::CreateFontToHandle("", 12, 1, DX_FONTTYPE_EDGE));
            Graph* Font18Edged = new Graph(DxLib::CreateFontToHandle("", 18, 1, DX_FONTTYPE_EDGE));
            Graph* Font18EdgedThick = new Graph(DxLib::CreateFontToHandle("", 18, 10, DX_FONTTYPE_EDGE));
        };

        class SoundRes
        {
        private:
            Sound* loadMp3(std::string fileName);
        public:
            Sound* PlayerDamaged = loadMp3("player_damaged");
            Sound* PlayerAtack = loadMp3("player_atack");
            Sound* SelectMove = loadMp3("select_move");
            Sound* SelectOk = loadMp3("select_ok");
            Sound* WindowPush = loadMp3("window_push");
            Sound* WindowOpen = loadMp3("window_open");
            Sound* FlagOn = loadMp3("flag_on");
            Sound* SceneChange = loadMp3("scene_change");

            SoundRes() {};
        };

    }



}


namespace ingame{
    
    const int GRID_WIDTH = 416, GRID_HEIGHT = 240;
    const int PX_PER_GRID = 3;
    const int FULL_WIDTH = GRID_WIDTH * PX_PER_GRID;
    const int FULL_HEIGHT = GRID_HEIGHT * PX_PER_GRID;

    extern useful::Random* Rand;

    extern int GameState;

    extern resorce::ImageRes* Images;
    extern resorce::FontRes* Fonts;
    extern resorce::SoundRes* Sounds;

    int DoProcess();
    int SceneTransition();

    class SceneBase : public Singleton<SceneBase>
    {
        bool canExit = false;
    public:
        SceneBase();
        virtual void Loop();
        virtual ~SceneBase();
        void EnableExit();
    };

    void ExecuteScene(SceneBase* scene);

    namespace title
    {
        class TitleScene : public SceneBase
        {
        public:
            TitleScene();
            ~TitleScene();
        };
    }


    namespace main 
    {
        class MainScene : public SceneBase
        {
        public:
            MainScene();
            ~MainScene();
        };
    }
}


