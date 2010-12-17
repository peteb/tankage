/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "cactus.h"
#include "world.h"
#include "physics/body.h"
#include "projectile.h"
#include "missile.h"

Cactus::Cactus()
   : health(100.0f)
{
}

void Cactus::setTransform(const CoordSystemData2 & cs) {
   body->setTransform(cs);
}

CoordSystemData2 Cactus::getTransform() const {
   return body->getTransform();
}


void Cactus::leftView() {
   kill();
}

void Cactus::collided(const Ref<Physics::Geom>::SharedPtr & with) {
   if (Ref<Object>::SharedPtr lockedOwner = with->getOwner().lock()) {
	  if (Ref<Projectile>::SharedPtr bulletOwner = Cast<Projectile>(lockedOwner)) {
		 bulletOwner->kill();
         increaseHealth(-45.0f);
	  }
	  if (Ref<Missile>::SharedPtr lockedMissile = Cast<Missile>(lockedOwner)) {
		 lockedMissile->kill();
         increaseHealth(-100.0f);
	  }

   }
}

void Cactus::increaseHealth(float by) {
   health += by;

   if (health <= 0.0f) {
      sprite->setCell(3, 0);
      geom = Ref<Physics::Geom>::Null();
   }
   else if (health <= 50.0f) {
      sprite->setCell(2, 0);
   }
   else if (health <= 75.0f) {
      sprite->setCell(1, 0);
   }
}
