/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "game.h"
#include "graphics/sprite.h"
#include "snail.h"

#include <iostream>
#include <boost/make_shared.hpp>

Game::Game()
   : creator(world)
{
   firstSnail = world.insert(creator.createSnail(100.0f, creator, world));
   secondSnail = world.insert(creator.createSnail(500.0f, creator, world));
   
   firstSnail.lock()->setPosition(vec2(10.0f, 10.0f));
   
   playerController.setRefFrameDelegate(firstSnail.lock()->logic);
   playerController.setActionDelegate(firstSnail.lock()->logic);
}

Game::~Game() {
   
}

void Game::tick(float dt) {
   playerController.update(dt);
//   firstSnail.lock()->logic->update(dt);
//   secondSnail.lock()->logic->update(dt);
   
   world.scheduler.update(dt);
   world.physics.update(dt);
   world.graphics.render(dt);
   
   // const float step_size = 1.0f / 40.0f;
   // accum_time += dt;
   // 
   // for (int i = 0; i < accum_time / step_size; ++i) {
   //    world.quickStep(step_size);
   // }   
}

void Game::windowChangedSize(int width, int height) {
   std::cout << "Window changed size: " << width << ", " << height << std::endl;
   world.graphics.resizeViewport(vec2(width, height));
   world.physics.resizeArea(width, height);
}

// shouldn't really be here. would be better if glfw talked directly
// to the controller
void Game::keyStateChanged(const std::string & key, int state) {
   std::cout << key << ": " << state << std::endl;
      
   playerController.toggle(key, state);
}
