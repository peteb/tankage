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
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

class Object;
 
class World {
public:
   template<typename T>
   boost::weak_ptr<T> insert(const boost::shared_ptr<T> & object) {
      object->submitComponents(*this);
      liveObjects.add(object);   

      return object;      
   }
   
   Graphics::Subsystem graphics;
   Physics::Subsystem physics;
   ObjectList liveObjects;
};


#endif /* end of include guard: WORLD_H_1FEL58C0 */
