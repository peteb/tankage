/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef PROJECTILE_WEAPON_H
#define PROJECTILE_WEAPON_H

// TODO: move target into Projectile

enum {
   SHOT_SHOOTING = 0x0001,
   SHOT_FIRST = 0x0002,
   SHOT_ENDING = 0x0004,
};
   
class ProjectileWeapon {
public:
   ProjectileWeapon(float interval);
   
   void startShooting();
   void stopShooting();
   void update(float dt);

   virtual void shoot();

private:
   float interval;
   float secondsSinceShot;
   unsigned shooting; // state of the gun
};

#endif // !PROJECTILE_WEAPON_H
