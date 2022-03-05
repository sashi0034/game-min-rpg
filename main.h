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


#define DEBUG

#ifdef DEBUG

#endif // DEBUG



using namespace gameUtils;
using namespace gameEngine;









namespace ingame
{
    namespace resorce 
    {
        class Image
        {
        public:
            Graph* Test = Graph::LoadGraph(R"(.\asset\image\cloud_128x64.png)");
            Graph* Chicken = Graph::LoadGraph(R"(.\asset\image\chicken_32x32.png)");
            Graph* Tile32 = Graph::LoadGraph(R"(.\asset\image\magma_tile_black_32x32.png)");

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

    useful::Random* Rand = new useful::Random();

    int GameState;

    resorce::Image* Img;


    int Process();
    int SceneTransition();

    void LoopBasicUpdate();



    namespace main 
    {
        class Scene
        {
        public:
            Scene();
            void Loop();
        };





        class Test : public Actor
        {
        public:
            static Test* Sole;

            Sprite* OtherSp;

            sol::table SolState;

            Test();

            

        protected:
            void update() override;
        };



 



        class BackGround : public Actor
        {
        public:
            static BackGround* Sole;

            int Image;
            BackGround();
        protected:
            void update() override;
        };




    }
}


