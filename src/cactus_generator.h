/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef CACTUS_GENERATOR_H
#define CACTUS_GENERATOR_H

#include "updatable.h"
#include <utils/vec.h>

class ObjectCreator;
class World;

class CactusGenerator : public Updatable {
public:
   CactusGenerator(ObjectCreator & creator, World & world);
   
   void update(float dt);
   void setPosition(const vec2 & pos);

private:
   void spawnCactus();
   
   vec2 origin;
   float timeSinceCactus;
   ObjectCreator & creator;
   World & world;
};

#endif /* end of include guard: CACTUS_GENERATOR_H */		  
