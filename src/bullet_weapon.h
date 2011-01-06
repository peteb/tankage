/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef BULLET_WEAPON_H
#define BULLET_WEAPON_H

#include "projectile_weapon.h"
#include "ref.h"

class ObjectCreator;
class World;
class Snail;

class BulletWeapon : public ProjectileWeapon {
public:
  BulletWeapon(ObjectCreator &creator, World &world,
               const Ref<Snail>::WeakPtr &shooter);
  
  void shoot();
  bool isDepleted() const;
  
private:
  ObjectCreator &creator;
  World &world;
  Ref<Snail>::WeakPtr shooter;
};



#endif // !BULLET_WEAPON_H
