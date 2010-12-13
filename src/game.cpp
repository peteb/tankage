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
#include "graphics/render_list.h"
#include "graphics/texture_fx.h"
#include "graphics/color.h"

#include <iostream>
#include <cstdlib>
#include "ref.h"

Game::Game()
   : creator(world)
   , playerInput1("player1")
   , playerInput2("player2")
{
   Ref<Graphics::Renderer>::SharedPtr heartFx = world.graphics.getRenderer("../data/hearts.png");   

   {
 	  Ref<Snail> snail = Owning(Cast<Snail>(creator.createObject("snail1", creator)));
 	  world.insert(snail.lock());
	  snail->setTransform(CoordSystemData2(vec2(100.0f, 400.0f), mat2::Identity));
      firstSnail = Observing(snail);
      snailHealth1 = Owning(new HealthMeter);
	  snailHealth1->setValue(100.0f);
	  snailHealth1->setPosition(vec2(20.0f, 20.0f));
	  snailHealth1->setRenderer(heartFx);
	  snailHealth1->setDir(1.0f);
	  firstSnail->logic->setHealthMeter(Observing(snailHealth1));
   }

   {
      Ref<Snail> snail = Owning(Cast<Snail>(creator.createObject("snail2", creator)));
      world.insert(snail.lock());
      snail->setTransform(CoordSystemData2(vec2(700.0f, 400.0f), mat2::Identity));
      secondSnail = Observing(snail);
      snailHealth2 = Owning(new HealthMeter);
      snailHealth2->setValue(100.0f);
      snailHealth2->setPosition(vec2(800.0f - 20.0f, 20.0f));
      snailHealth2->setRenderer(heartFx);
      snailHealth2->setDir(-1.0f);
      secondSnail->logic->setHealthMeter(Observing(snailHealth2));
   }

   firstSnail->setTransform(CoordSystemData2(vec2(100.0f, 300.0f), mat2::Identity));
   secondSnail->setTransform(CoordSystemData2(vec2(700.0f, 300.0f), mat2::Identity));
   
   playerInput1.setRefFrameDelegate(Observing(firstSnail->logic));
   playerInput1.setActionDelegate(Observing(firstSnail->logic));
   playerInput2.setRefFrameDelegate(Observing(secondSnail->logic));
   playerInput2.setActionDelegate(Observing(secondSnail->logic));


   cactusGenerator = Owning(new CactusGenerator(creator, world));
   cactusGenerator->setPosition(vec2(350.0f, 630.0f));
   world.scheduler.subscribe(0.1f, cactusGenerator); // update every 1/10 of a second, should be enough

   defaultRenderer = Owning(world.graphics.getRenderer("../data/geom.png"));
   Cast<Graphics::TextureFx>(defaultRenderer.lock())->setColor(Graphics::Color(1.0f, 1.0f, 1.0f, 0.5));

   drawGeoms = (getenv("DRAW_GEOMS") != 0);
}

Game::~Game() {
   
}

void Game::tick(float dt) {
   playerInput1.update(dt);
   playerInput2.update(dt);
   
   world.scheduler.update(dt);
   world.physics.update(dt);
   world.update();

   Ref<Graphics::RenderList>::SharedPtr renderList(new Graphics::RenderList);
   renderList->setDefaultRenderer(defaultRenderer); //Owning(world.graphics.getRenderer("../data/hearts.png")));

   if (drawGeoms) {
      world.physics.enqueueGeoms(renderList);
   }
   
   world.graphics.enqueueVisibleSprites(renderList);
   snailHealth1->enqueueRender(renderList, dt);
   snailHealth2->enqueueRender(renderList, dt);
   
   world.graphics.beginFrame();
   world.graphics.render(renderList);
   
   // const float step_size = 1.0f / 40.0f;
   // accum_time += dt;
   // 
   // for (int i = 0; i < accum_time / step_size; ++i) {
   //    world.quickStep(step_size);
   // }   
}

void Game::windowChangedSize(int width, int height) {
   std::cout << "Window changed size: " << width << ", " << height << std::endl;
   world.graphics.resizeViewport(rect(vec2::Zero, static_cast<float>(width), static_cast<float>(height)));
   world.physics.resizeArea(width, height);
}

// shouldn't really be here. would be better if glfw talked directly
// to the controller
void Game::keyStateChanged(const std::string & key, int state) {
   std::cout << key << ": " << state << std::endl;
      
   playerInput1.toggle(key, state);
   playerInput2.toggle(key, state);
}
