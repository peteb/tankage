/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "projectile_weapon.h"

ProjectileWeapon::ProjectileWeapon(float interval)
   : interval(interval)
   , shooting(0)
   , secondsSinceShot(interval)
{
}

void ProjectileWeapon::setCoordSystem(const Ref<CoordSystem2> &newCs) {
  // Sets the origin for spawned projectiles
  this->origin = newCs;
}

void ProjectileWeapon::startShooting() {
  shooting |= (SHOT_FIRST | SHOT_SHOOTING);
}

void ProjectileWeapon::stopShooting() {
  if ((shooting & SHOT_SHOOTING) && !(shooting & SHOT_FIRST)) {
    // Stop shooting immediately if it's not the first round
    shooting = 0;
  }
  else {
    shooting |= SHOT_ENDING;
  }
}

void ProjectileWeapon::update(float dt) {
  if (shooting & SHOT_SHOOTING) {
    if (secondsSinceShot >= interval) {
      shoot();
      secondsSinceShot = 0.0f;
      shooting &= ~SHOT_FIRST;
    }
  }

  if (shooting & SHOT_ENDING) {
    shooting = 0;
  }

  secondsSinceShot += dt;
}

void ProjectileWeapon::shoot() {

}

