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
#include "cactus_generator.h"

#include <iostream>
#include <boost/make_shared.hpp>

#include "ref.h"

Game::Game()
   : creator(world)
   , playerInput1("player1")
   , playerInput2("player2")
{
   {
	  Ref<Snail> snail = Owning(boost::dynamic_pointer_cast<Snail>(creator.createObject("snail1", creator)));
	  world.insert(snail.lock());
	  snail->setPosition(vec2(100.0f, 400.0f));
	  firstSnail = Observing(snail);
	  snailHealth1.setValue(75.0f);
	  snailHealth1.setPosition(vec2(20.0f, 20.0f));
	  snailHealth1.setTexture(world.graphics.getTexture("../data/hearts.png"));
	  snailHealth1.setDir(1.0f);
	  // TODO: ska man kunna få texture från gfx_subsystem? yes, kör på det tills vidare. sen refactor
   }

   {
	  Ref<Snail> snail = Owning(boost::dynamic_pointer_cast<Snail>(creator.createObject("snail2", creator)));
	  world.insert(snail.lock());
	  snail->setPosition(vec2(700.0f, 400.0f));
	  secondSnail = Observing(snail);
	  snailHealth2.setValue(100.0f);
	  snailHealth2.setPosition(vec2(800.0f - 20.0f, 20.0f));
	  snailHealth2.setTexture(world.graphics.getTexture("../data/hearts.png"));
	  snailHealth2.setValue(40.0f);
	  snailHealth2.setDir(-1.0f);
   }

   firstSnail->setPosition(vec2(100.0f, 300.0f));
   secondSnail->setPosition(vec2(700.0f, 300.0f));
   
   playerInput1.setRefFrameDelegate(Observing(firstSnail->logic));
   playerInput1.setActionDelegate(Observing(firstSnail->logic));
   playerInput2.setRefFrameDelegate(Observing(secondSnail->logic));
   playerInput2.setActionDelegate(Observing(secondSnail->logic));


   cactusGenerator = Owning(boost::shared_ptr<CactusGenerator>(new CactusGenerator(creator, world)));
   cactusGenerator->setPosition(vec2(350.0f, 630.0f));
   world.scheduler.subscribe(0.1f, cactusGenerator); // update each 1/10 of a second, should be enough
   
}

Game::~Game() {
   
}

void Game::tick(float dt) {
   playerInput1.update(dt);
   playerInput2.update(dt);
   
//   firstSnail.lock()->logic->update(dt);
//   secondSnail.lock()->logic->update(dt);
   
   world.scheduler.update(dt);
   world.physics.update(dt);
   world.graphics.render(dt);
   snailHealth1.draw();
   snailHealth2.draw();
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
      
   playerInput1.toggle(key, state);
   playerInput2.toggle(key, state);
}
