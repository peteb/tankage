/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "powerup.h"
#include "projectile.h"
#include "snail.h"

void PowerUp::leftView() {
   this->kill();
}

void PowerUp::collided(const Ref<Physics::Geom>::SharedPtr & with) {
   if (Ref<Object>::SharedPtr lockedOwner = with->getOwner().lock()) {
	  if (Ref<Projectile>::SharedPtr lockedProjectile = Cast<Projectile>(lockedOwner)) {
		 lockedProjectile->kill();
         this->kill();
         
         if (Ref<Snail>::SharedPtr lockedSnail = lockedProjectile->shooter.lock()) {
            lockedSnail->increaseHealth(35);
         }
	  }
   }
}
