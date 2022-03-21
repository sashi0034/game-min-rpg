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


#define DEBUG

#ifdef DEBUG

#endif // DEBUG



using namespace gameUtils;
using namespace gameEngine;









namespace ingame
{
    extern const char* GAME_TITLE_NAME;

    namespace resorce 
    {
        class Image
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

            Graph* UiWindowBlack = loadPng("ui/window_black");
            Graph* UiArrow = loadPng("ui/arrow_16x16");

            Graph* Font18Edged = new Graph(DxLib::CreateFontToHandle("", 18, 1, DX_FONTTYPE_EDGE));
#if 0
            int Templa = LoadGraph(R"(.png)");
#endif // 0

            Image()
            {
            }
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

    extern resorce::Image* Images;


    int doProcess();
    int SceneTransition();

    void LoopBasicUpdate();


    class SceneBase
    {
    protected:
        void loop();
    public:
        SceneBase();
    };

    namespace main 
    {
        class MainScene : public SceneBase
        {
        public:
            MainScene();
        };









 
    }
}


