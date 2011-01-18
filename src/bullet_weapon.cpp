/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "bullet_weapon.h"
#include "object_creator.h"
#include "world.h"
#include "physics/body.h"
#include "snail.h"
#include "projectile.h"

BulletWeapon::BulletWeapon(ObjectCreator &creator, World &world,
                           const Ref<Snail>::WeakPtr &shooter)
  : ProjectileWeapon(0.5f)
  , creator(creator)
  , world(world)
  , shooter(shooter)
{
}

void BulletWeapon::shoot() {
  if (Ref<CoordSystem2>::SharedPtr lockedOrigin = origin.lock()) {
    Ref<Projectile>::SharedPtr bullet = Cast<Projectile>(creator.createObject("bullet", creator));

    CoordSystem2::data_type cs = lockedOrigin->getTransform();
    vec2 forward = cs.orientation.x();

    if (invertForward)
      forward = -forward;
    
    bullet->setTransform(cs);
    bullet->body->addImpulse(vec2(2500.0f, 0.0f) * forward);
    bullet->body->setTransform(cs);

    if (Ref<Snail>::SharedPtr lockedSnail = shooter.lock()) {
      bullet->shooter = shooter; // FIXME: shooter should not be here, but target can
      
      shooter.lock()->physBody->addImpulse(vec2(-130.0f, 0.0f) * forward);
    }
   
    world.insert(bullet);
  }
  
}

bool BulletWeapon::isDepleted() const {
  return false; // Infinite amount of ammunition
}
