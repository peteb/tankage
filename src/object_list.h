/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef OBJECT_LIST_H_YW53G2SY
#define OBJECT_LIST_H_YW53G2SY

#include <vector>
#include "ref.h"

class Object;

class ObjectList {
public:
   void add(const Ref<Object>::SharedPtr & object);
   void enqueueRemoval(Object * obj);
   void eraseEnqueuedRemovals();
   
   // object should receive a method call when added to an object list
   // object should have a "kill" method, that removes the object from those lists
   
private:
   std::vector<Ref<Object>::SharedPtr> objects;
   std::vector<Object *> enqueuedItems;
};


#endif /* end of include guard: OBJECT_LIST_H_YW53G2SY */
