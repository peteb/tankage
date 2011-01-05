/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "rocket_ammo.h"
#include "projectile.h"
#include "snail.h"
#include "player_entity.h"

void RocketAmmo::leftView() {
   this->kill();
}

void RocketAmmo::collided(const Ref<Geom>::SharedPtr & with) {
  if (Ref<Object>::SharedPtr lockedOwner = with->getOwner().lock()) {
    if (Ref<Projectile>::SharedPtr lockedProjectile = Cast<Projectile>(lockedOwner)) {
      lockedProjectile->kill();
      this->kill();
      
      if (Ref<Snail>::SharedPtr lockedSnail = lockedProjectile->shooter.lock()) {
        lockedSnail->logic->giveItems("rockets", 2); 
      }
    }
  }
}
