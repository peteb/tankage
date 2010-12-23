/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "player_entity.h"
#include "projectile.h"
#include "object_creator.h"
#include "world.h"
#include "graphics/sprite.h"
#include "physics/body.h"
#include "health_meter.h"
#include "missile.h"
#include "projectile_weapon.h"
#include "coordsystem_transformer.h"
#include "missile_launcher.h"
#include "bullet_weapon.h"

#include <algorithm>
#include <iostream>

PlayerEntity::PlayerEntity(float x, const Ref<Snail>::WeakPtr &shooter, ObjectCreator & creator, World & world) 
  : creator(creator)
  , world(world)
  , shooter(shooter)
{
  xPos = x;
  weapon = Owning(new BulletWeapon(creator, world, shooter));
  
   // Set the origin of the weapon. Ugly, it's using the shooter!
  weapon->setCoordSystem(
    Owning(
      new CoordSystemTransformer<CoordSystem2>(
        Observing(shooter.lock()),
        CoordSystem2::data_type(vec2::Zero, mat2::Identity)
        )
      )
    );
}

// FIXME: rename setTarget. target = the snail we're controlling
// FIXME: there's no reason _not_ to work directly on snails...
//        ie, this abstraction is useless

void PlayerEntity::setTarget(const Ref<Physics::Body> & newTarget) {
  this->target = newTarget;
}

// TODO: setPosition+getPosition on body to restrict

void PlayerEntity::update(float dt) {
   if (Ref<ProjectileWeapon>::SharedPtr lockedWeapon = weapon.lock()) {
      // Update the current weapon
      lockedWeapon->update(dt);
   }
   
   
   if (Ref<Physics::Body>::SharedPtr lockedTarget = target.lock()) {
      // Restrain the snail's body to the screen
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
   if (Ref<Physics::Body>::SharedPtr lockedBody = target.lock())
      lockedBody->setTransform(cs);
}

CoordSystemData2 PlayerEntity::getTransform() const {
   if (Ref<Physics::Body>::SharedPtr lockedTarget = target.lock())
      return lockedTarget->getTransform();

   return CoordSystemData2::Identity;
}

// 1 = on, 0 = off, -1 = one-shot
void PlayerEntity::trigger(const std::string &action, int state) {
   std::cout << "received event: " << action << std::endl;

   if (Ref<ProjectileWeapon>::SharedPtr lockedWeapon = weapon.lock()) {
      if (state == 1) {
         lockedWeapon->startShooting();
      }
      else if (state == 0) {
         lockedWeapon->stopShooting();
      }
   }
}


void PlayerEntity::onHealthChange(float newHealth, float diff) {
   std::cout << "new health: " << newHealth << std::endl;
   if (Ref<HealthMeter>::SharedPtr lockedMeter = healthMeter.lock()) {
      float speed = 200.0f;
      if (diff > 0.0f) {
         speed = 90.0f;
      }
      
      lockedMeter->setValue(newHealth, speed);
   }
}

void PlayerEntity::setHealthMeter(const Ref<HealthMeter> &newMeter) {
   healthMeter = newMeter;
}


