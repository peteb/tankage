/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef MISSILE_LAUNCHER_H
#define MISSILE_LAUNCHER_H

#include "projectile_weapon.h"

class MissileLauncher : public ProjectileWeapon {
public:
  MissileLauncher();
  
  void shoot();
};


#endif // !MISSILE_LAUNCHER_H
