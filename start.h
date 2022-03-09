#pragma once

#define _USE_MATH_DEFINES


#include <random>
#include <any>
#include <map>
#include <math.h>
#include <iostream>

#include "DxLib.h" // 3.22
#include "magic_enum.h" // 0.7.3
#include "lua.hpp" // 5.4
#include "sol.hpp" // 3.5.0

#include "sprite.h"
#include "game_utils.h"
#include "game_engine.h"
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
            Graph* Cloud = loadPng("cloud_128x64");
            Graph* Chicken = loadPng("chicken_32x32");
            Graph* NaturalTile = loadPng("natural_playground_16x16");
            Graph* Weed = loadPng("weed_16x16");
            Graph* Tree = loadPng("tree_16x16");
            Graph* Kisaragi = loadPng("kisaragi_24x24");
            Graph* Punicat = loadPng("punicat_24x24");
            Graph* UiWindows = loadPng("ui/window_black");


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
    
    const int ROUGH_WIDTH = 416, ROUGH_HEIGHT = 240;
    const int ROUGH_SCALE = 3;
    const int FULL_WIDTH = ROUGH_WIDTH * ROUGH_SCALE;
    const int FULL_HEIGHT = ROUGH_HEIGHT * ROUGH_SCALE;

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


        enum ZIndex
        {
            FLOOR = 1000,
            CHARACTER = 0,
        };








 
    }
}


