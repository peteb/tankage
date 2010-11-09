/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef OBJECT_CREATOR_H_FSSO9BSX
#define OBJECT_CREATOR_H_FSSO9BSX

#include "ref.h"

class Bullet;
class Snail;
class World;
class Object;

class ObjectCreator {
public:
   ObjectCreator(World & world);
   
   Ref<Object>::SharedPtr createObject(const std::string & type, ObjectCreator & creator);

private:
   Ref<Bullet>::SharedPtr createBullet();
   Ref<Snail>::SharedPtr createSnail(int team, ObjectCreator & creator);

   World & world;
};

#endif /* end of include guard: OBJECT_CREATOR_H_FSSO9BSX */
