/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef OBJECT_CREATOR_H_FSSO9BSX
#define OBJECT_CREATOR_H_FSSO9BSX

#include "ref.h"

class Projectile;
class Snail;
class World;
class Object;
class Helmet;

class ObjectCreator {
public:
   ObjectCreator(World & world);
   
   Ref<Object>::SharedPtr createObject(const std::string & type, ObjectCreator & creator);

private:
   Ref<Projectile>::SharedPtr createProjectile();
   Ref<Snail>::SharedPtr createSnail(int team, ObjectCreator & creator);
   Ref<Helmet>::SharedPtr createHelmet(int dir);
   
   World & world;
};

#endif /* end of include guard: OBJECT_CREATOR_H_FSSO9BSX */
