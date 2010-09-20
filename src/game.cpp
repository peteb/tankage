/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "game.h"
#include "graphics/sprite.h"
#include <iostream>


Game::Game() {
   playerSprite = graphics.createSprite();
   playerSprite->position = vec2(10.0f, 10.0f);
   playerController.setTarget(playerSprite);
   
}

Game::~Game() {
   
}

void Game::tick(float dt) {
   playerController.update(dt);
   physics.update(dt);
   graphics.render(dt);
   // const float step_size = 1.0f / 40.0f;
   // accum_time += dt;
   // 
   // for (int i = 0; i < accum_time / step_size; ++i) {
   //    world.quickStep(step_size);
   // }   
}

void Game::windowChangedSize(int width, int height) {
   std::cout << "Window changed size: " << width << ", " << height << std::endl;
   graphics.resizeViewport(vec2(width, height));
   physics.resizeArea(width, height);
}

// shouldn't really be here. would be better if glfw talked directly
// to the controller
void Game::keyStateChanged(const std::string & key, int state) {
   std::cout << key << ": " << state << std::endl;
   
   int dir = -1;
   
   if (key == "up_arrow")
      dir = 0;
   else if (key == "right_arrow")
      dir = 1;
   else if (key == "down_arrow")
      dir = 2;
   else if (key == "left_arrow")
      dir = 3;
      
   if (dir > -1) {
      if (state)
         playerController.startDirection(dir);
      else
         playerController.stopDirection(dir);
   }
}
