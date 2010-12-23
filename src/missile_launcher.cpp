/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "missile_launcher.h"
#include "missile.h"
#include "object_creator.h"
#include "world.h"
#include "physics/body.h"
#include "snail.h"

MissileLauncher::MissileLauncher(ObjectCreator &creator, World &world,
                                 const Ref<Snail>::WeakPtr &shooter)
  : ProjectileWeapon(0.5f)
  , creator(creator)
  , world(world)
  , shooter(shooter)
{
}

void MissileLauncher::shoot() {
  // TODO: ProjectileWeapon should have isDepleted

  if (Ref<CoordSystem2>::SharedPtr lockedOrigin = origin.lock()) {
    // Create the new missile
    Ref<Missile>::SharedPtr missile = Cast<Missile>(
      creator.createObject("missile", creator)
      );

    CoordSystem2::data_type cs = lockedOrigin->getTransform();
    
    missile->setTransform(cs);
    missile->body->addImpulse(vec2(500.0f, 0.0f) * cs.orientation.getX());
    missile->body->setTransform(cs);

    missile->target = shooter.lock()->enemy;
    missile->shooter = shooter; // FIXME: shooter should not be here, but target can
    
    //if (Ref<Physics::Body>::SharedPtr lockedTarget = target.lock()) {
//      lockedTarget->addImpulse(vec2(-800.0f, 0.0f) * forward);
      // FIXME: do a callback on the logic with a recoil
    // }
    
    world.insert(missile);
  }
  
}

