/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "game.h"
#include "graphics/sprite.h"
#include "snail.h"
#include "math/rect.h"
#include "object.h"

#include <iostream>
#include <boost/make_shared.hpp>

#include "ref.h"

Game::Game()
   : creator(world)
{
   firstSnail = Owning(boost::dynamic_pointer_cast<Snail>(creator.createObject("snail1", creator)));
   secondSnail = Owning(boost::dynamic_pointer_cast<Snail>(creator.createObject("snail2", creator)));
   firstSnail->setPosition(vec2(100.0f, 200.0f));
   secondSnail->setPosition(vec2(600.0f, 200.0f));
   
   playerController.setRefFrameDelegate(firstSnail->logic);
   playerController.setActionDelegate(firstSnail->logic);
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
   world.update();
   
   // const float step_size = 1.0f / 40.0f;
   // accum_time += dt;
   // 
   // for (int i = 0; i < accum_time / step_size; ++i) {
   //    world.quickStep(step_size);
   // }   
}

void Game::windowChangedSize(int width, int height) {
   std::cout << "Window changed size: " << width << ", " << height << std::endl;
   world.graphics.resizeViewport(rect(vec2::Zero, width, height));
   world.physics.resizeArea(width, height);
}

// shouldn't really be here. would be better if glfw talked directly
// to the controller
void Game::keyStateChanged(const std::string & key, int state) {
   std::cout << key << ": " << state << std::endl;
      
   playerController.toggle(key, state);
}
