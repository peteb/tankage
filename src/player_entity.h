/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef PLAYER_ENTITY_H_5TG24XG5
#define PLAYER_ENTITY_H_5TG24XG5

#include "coord_system2.h"
#include "triggerable.h"
#include "updatable.h"
#include "ref.h"
#include "snail.h"

class ObjectCreator;
class World;
class HealthMeter;

namespace Physics {class Body; }

class PlayerEntity : public CoordSystem2, public Triggerable, public Updatable, public SnailEventHandler {
public:
   PlayerEntity(float x, void * shooterId, ObjectCreator & creator, World & world);
   
   void setTarget(const Ref<Physics::Body> & newTarget);
   void setHealthMeter(const Ref<HealthMeter> & newMeter);
   void update(float dt);
   
   // CoordSystem2 --------------------------------------
   void setTransform(const CoordSystemData2 & cs);
   CoordSystemData2 getTransform() const;
   
   // Updatable --------------------------------------------
   void trigger(const std::string & action, int state);

   void onHealthChange(float newHealth);

   vec2 weaponDir, weaponPos;
   
private:
   void shoot();
   
   int shooting;
   float tshot;
   void * shooterId;
   
   float xPos;
   Ref<HealthMeter> healthMeter;
   Ref<Physics::Body> target;
   ObjectCreator & creator;
   World & world;
};

#endif /* end of include guard: PLAYER_ENTITY_H_5TG24XG5 */
