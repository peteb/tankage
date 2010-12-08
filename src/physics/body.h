/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef BODY_H_JI764YY5
#define BODY_H_JI764YY5

#include "coord_system2.h"
#include "ref.h"

class Object;

namespace Physics {
   class Subsystem;
   
   /*
    * A physical body that can be acted upon through forces
    */
   class Body : public CoordSystem2 {
   private:
      Body(Physics::Subsystem & subsystem); 
      
   public:
      
      friend class Physics::Subsystem;

      void update(float dt);
      void setDelegate(const Ref<CoordSystem2> & newTarget);

	  void addImpulse(const vec2 & vel);
	  //void setVelocity(const vec2 & vel);
	  vec2 getVelocity() const;

	  void setOwner(const Ref<Object>::WeakPtr & owner);
	  Ref<Object>::WeakPtr getOwner() const;
	  
      // CoordSystem2 ---------------------------------------
      void setTransform(const CoordSystemData2 & cs);
      CoordSystemData2 getTransform() const;
      
   private:
      Physics::Subsystem & subsystem;
      Ref<CoordSystem2> delegate;
	  Ref<Object>::WeakPtr owner;
      vec2 position, velocity;
	  mat2 orientation;
   };
   
}

#endif /* end of include guard: BODY_H_JI764YY5 */
