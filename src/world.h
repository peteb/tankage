/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef WORLD_H_1FEL58C0
#define WORLD_H_1FEL58C0

#include "graphics/gfx_subsystem.h"
#include "physics/phys_subsystem.h"
#include "object_list.h"
#include "scheduler.h"
#include "ref.h"

class Object;
 
class World {
public:
   template<typename T>
   typename Ref<typename T::element_type>::WeakPtr insert(const T & object) {
      liveObjects.add(object);   
      object->addedOwner(&liveObjects);

      return object;
   }
   
   void update() {
      liveObjects.eraseEnqueuedRemovals();
   }
   
   GfxSubsystem graphics;
   PhysSubsystem physics;
   ObjectList liveObjects;
   Scheduler scheduler;
};


#endif /* end of include guard: WORLD_H_1FEL58C0 */
