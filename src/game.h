/*
 * $Id$
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef GAME_H_RGVIQMD1
#define GAME_H_RGVIQMD1
 
#include "graphics/gfx_subsystem.h"

class Game {
public:
   Game();
   ~Game();
   
   void tick(float dt);
   
private:
   Graphics::Subsystem graphics;
};

#endif /* end of include guard: GAME_H_RGVIQMD1 */
