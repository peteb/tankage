/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "cactus_generator.h"
#include <iostream>
#include "cactus.h"
#include "object_creator.h"
#include "world.h"
#include "physics/body.h"

CactusGenerator::CactusGenerator(ObjectCreator & creator, World & world)
   : creator(creator)
   , world(world)
{
   timeSinceCactus = 0.0f;
}

void CactusGenerator::setPosition(const vec2 & pos) {
   origin = pos;
}

void CactusGenerator::update(float dt) {
   timeSinceCactus += dt;
   if (timeSinceCactus > 0.5f) {
	  timeSinceCactus = 0.0f;
	  std::cout << "new cactus" << std::endl;

      if (rand() % 10 < 5)
         spawnCactus();
   }
   
}

void CactusGenerator::spawnCactus() {
   Ref<Cactus>::SharedPtr cactus = Cast<Cactus>(creator.createObject("cactus", creator));

   vec2 startPos = origin;
   startPos.x += static_cast<float>(rand() % 50) * 1.5f;
   cactus->setPosition(startPos);
   
   cactus->body->addImpulse(vec2(0.0f, -140.0f));
   world.insert(cactus);
}
