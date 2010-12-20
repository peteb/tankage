/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef PROJECTILE_WEAPON_H
#define PROJECTILE_WEAPON_H

// TODO: move target into Projectile

class ProjectileWeapon {
public:
   void startShooting();
   void stopShooting();
   void update(float dt);
};

#endif // !PROJECTILE_WEAPON_H
