/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef PLAYER_ENTITY_H_5TG24XG5
#define PLAYER_ENTITY_H_5TG24XG5

#include "coord_system2.h"
#include "triggerable.h"
#include "updatable.h"
#include "ref.h"
#include "snail.h"

class ObjectCreator;
class World;
class HealthMeter;
class ProjectileWeapon;
class MissileLauncher;
class Body;

// This class is the main logic for snails, it could just as well been
// in the Snail class directly
class PlayerEntity : public CoordSystem2, public Triggerable, public Updatable, public SnailEventHandler {
public:
  PlayerEntity(float x, const Ref<Snail>::WeakPtr & shooterId, ObjectCreator & creator, World & world);
  
  void setTarget(const Ref<Body> & newTarget);
  void setHealthMeter(const Ref<HealthMeter> & newMeter);
  void update(float dt);
  void setWeapon(const Ref<ProjectileWeapon> &weapon);
  void giveItems(const std::string &itemName, int quantity);
  
  // CoordSystem2 --------------------------------------
  void setTransform(const CoordSystemData2 & cs);
  CoordSystemData2 getTransform() const;
  
  // Updatable --------------------------------------------
  void trigger(const std::string & action, int state);
  
  void onHealthChange(float newHealth, float diff);
  
  vec2 weaponDir, weaponPos; // FIXME: remove these
  
private:
   
  Ref<Snail>::WeakPtr shooter;
  float timeSinceDepleted;
  float xPos;
  bool autoShoot;
  Ref<HealthMeter> healthMeter;
  Ref<Body> target;
  Ref<ProjectileWeapon> weapon, backupWeapon;
  Ref<MissileLauncher> missileLauncher;
  // FIXME: more generic solution for handling of different weapons
  
  ObjectCreator & creator;
  World & world;
};

#endif /* end of include guard: PLAYER_ENTITY_H_5TG24XG5 */
