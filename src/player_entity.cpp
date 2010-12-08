/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "player_entity.h"
#include "bullet.h"
#include "object_creator.h"
#include "world.h"
#include "graphics/sprite.h"
#include "physics/body.h"
#include "health_meter.h"
#include <algorithm>

PlayerEntity::PlayerEntity(float x, void * shooterId, ObjectCreator & creator, World & world) 
   : creator(creator)
   , world(world)
{
   xPos = x;
   shooting = 0;
   tshot = 0.0f;
   this->shooterId = shooterId;
}

void PlayerEntity::setTarget(const Ref<Physics::Body> & newTarget) {
   this->target = newTarget;
}

void PlayerEntity::shoot() {
   const vec2 forward = weaponDir;
   
   Ref<Bullet>::SharedPtr bullet = Cast<Bullet>(creator.createObject("bullet", creator));
   bullet->setTransform(CoordSystemData2(getTransform().position + weaponPos, getTransform().orientation));
   bullet->body->addImpulse(vec2(1400.0f, 0.0f) * forward);
   bullet->body->setTransform(CoordSystemData2(bullet->body->getTransform().position, mat2(weaponDir, vec2(0.0f, 1.0f))));
   
   if (Ref<Physics::Body>::SharedPtr lockedTarget = target.lock())
	  lockedTarget->addImpulse(vec2(-100.0f, 0.0f) * forward);

   bullet->shooter = shooterId;
   world.insert(bullet);
   //world.add(bullet);
}

// TODO: setPosition+getPosition on body to restrict

void PlayerEntity::update(float dt) {
   tshot += dt;
   
   
   if (shooting == 1 || shooting == 3) {
      tshot = 0.0f;
      shoot();
      
      if (shooting == 1)
         shooting = 2;
      else
         shooting = 0;
   }
   else if (shooting == 2) {
      if (tshot >= 0.4) {
         tshot = 0.0f;
         shoot();
      }
   }
   
   
   /*  if (Ref<Physics::Body>::SharedPtr acquiredTarget = target.lock()) {
	  } */

   if (Ref<Physics::Body>::SharedPtr lockedTarget = target.lock()) {
      vec2 pos = lockedTarget->getTransform().position;
      pos.x = std::max(pos.x, 32.0f);
      pos.x = std::min(pos.x, 800.0f - 32.0f);
      pos.y = std::max(pos.y, 32.0f);
      pos.y = std::min(pos.y, 600.0f - 32.0f);

      vec2 delta = vec2(xPos, pos.y) - pos;

      if (delta.getMagnitude() > 50.0f) {
         delta.normalize();
         delta = delta * 50.0f;
	  }

	  lockedTarget->addImpulse(-lockedTarget->getVelocity() * 0.005f);
	  lockedTarget->setTransform(CoordSystemData2(pos + delta * 5.0f * dt, lockedTarget->getTransform().orientation));
   }
}

void PlayerEntity::setTransform(const CoordSystemData2 & cs) {
   target->setTransform(cs);
}

CoordSystemData2 PlayerEntity::getTransform() const {
   return target->getTransform();
}

#include <iostream>
// 1 = on, 0 = off, -1 = one-shot
void PlayerEntity::trigger(const std::string & action, int state) {
   std::cout << "received event: " << action << std::endl;
   
   if (shooting == 2 && state == 0) // event triggered already, remove
      shooting = 0;
   else if (shooting == 0 && state == 1) // not been set for shooting yet
      shooting = 1;
   else if (shooting == 1 && state == 0)
      shooting = 3;
}


void PlayerEntity::onHealthChange(float newHealth) {
   std::cout << "new health: " << newHealth << std::endl;
   if (Ref<HealthMeter>::SharedPtr lockedMeter = healthMeter.lock())
	  lockedMeter->setValue(newHealth);
}

void PlayerEntity::setHealthMeter(const Ref<HealthMeter> & newMeter) {
   healthMeter = newMeter;
}


