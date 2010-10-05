/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "game.h"
#include "graphics/sprite.h"
#include <iostream>

#include <boost/make_shared.hpp>

Game::Game() {
   playerSprite = graphics.createSprite("../data/snail2.png");
   playerSprite->setPosition(vec2(10.0f, 10.0f));

   playerSprite2 = graphics.createSprite("../data/snail2.png");
   playerSprite2->setPosition(vec2(10.0f, 50.0f));

   playerEntity = boost::make_shared<PlayerEntity>(500.0f);
   playerController.setTarget(playerEntity);
   playerEntity->setTarget(playerSprite);

   playerEntity2 = boost::make_shared<PlayerEntity>(100.0f);
   playerEntity2->setTarget(playerSprite2);

}

Game::~Game() {
   
}

void Game::tick(float dt) {
   playerController.update(dt);
   playerEntity->update(dt);
   playerEntity2->update(dt);
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
