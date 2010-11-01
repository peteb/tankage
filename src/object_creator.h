/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef OBJECT_CREATOR_H_FSSO9BSX
#define OBJECT_CREATOR_H_FSSO9BSX

#include <boost/shared_ptr.hpp>

class Bullet;
class Snail;
class World;
class Object;

class ObjectCreator {
public:
   ObjectCreator(World & world);
   
   boost::shared_ptr<Object> createObject(const std::string & type, ObjectCreator & creator);

private:
   boost::shared_ptr<Bullet> createBullet();
   boost::shared_ptr<Snail> createSnail(int team, ObjectCreator & creator);

   World & world;
};

#endif /* end of include guard: OBJECT_CREATOR_H_FSSO9BSX */
