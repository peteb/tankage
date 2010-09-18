/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef GAME_H_RGVIQMD1
#define GAME_H_RGVIQMD1

#include "graphics/gfx_subsystem.h"
#include "physics/phys_subsystem.h"
#include "hi_controller.h"

namespace Graphics {
   class Sprite;
}

class Game {
public:
   Game();
   ~Game();
   
   void tick(float dt);
   void windowChangedSize(int width, int height);
   void keyStateChanged(const std::string & key, int state);
   
private:
   Graphics::Subsystem graphics;
   Physics::Subsystem physics;
   Graphics::Sprite * playerSprite;
   HiController playerController;

};

#endif /* end of include guard: GAME_H_RGVIQMD1 */
