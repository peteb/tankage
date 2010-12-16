/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "powerup.h"
#include "bullet.h"
#include "snail.h"

void PowerUp::leftView() {
   this->kill();
}

void PowerUp::collided(const Ref<Physics::Geom>::SharedPtr & with) {
   if (Ref<Object>::SharedPtr lockedOwner = with->getOwner().lock()) {
	  if (Ref<Bullet>::SharedPtr lockedBullet = Cast<Bullet>(lockedOwner)) {
		 lockedBullet->kill();
         this->kill();
         
         if (Ref<Snail>::SharedPtr lockedSnail = lockedBullet->shooter.lock()) {
            lockedSnail->increaseHealth(35);
         }
	  }
   }
}
