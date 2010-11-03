/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef OBJECT_H_GV2YM86M
#define OBJECT_H_GV2YM86M

#include "ref.h"

class World;
class ObjectList;
class Object;

class ObjectEventListener {
public:
   virtual ~ObjectEventListener() {}

   virtual void onObjectDeath(const Object & object) =0;
};

class Object {
public:
   Object();
   virtual ~Object() {}

   void addedOwner(ObjectList * owner);
   void kill();
   void setEventListener(const Ref<ObjectEventListener> & newListener);

   virtual void activate();
   virtual void deactivate();
   
private:
   Ref<ObjectEventListener> eventListener;
   ObjectList * owner;
};


#endif /* end of include guard: OBJECT_H_GV2YM86M */
