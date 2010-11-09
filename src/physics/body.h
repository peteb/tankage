/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef BODY_H_JI764YY5
#define BODY_H_JI764YY5

#include "reference_frame2.h"
#include "ref.h"

class Object;

namespace Physics {
   class Subsystem;
   
   /*
    * A physical body that can be acted upon through forces
    */
   class Body : public ReferenceFrame2 {
   private:
      Body(Physics::Subsystem & subsystem); 
      
   public:
      
      friend class Physics::Subsystem;

      void update(float dt);
      void setDelegate(const Ref<ReferenceFrame2> & newTarget);

	  void addImpulse(const vec2 & vel);
	  //void setVelocity(const vec2 & vel);
	  vec2 getVelocity() const;

	  void setOwner(const Ref<Object>::WeakPtr & owner);
	  Ref<Object>::WeakPtr getOwner() const;
	  
      // Referenceframe2 ---------------------------------------
      void setPosition(const vec2 & pos);
      vec2 getPosition() const;
      void setOrientation(const mat2 & orient);
      mat2 getOrientation() const;
      // -------------------------------------------------------
      
   private:
      Physics::Subsystem & subsystem;
      Ref<ReferenceFrame2> delegate;
	  Ref<Object>::WeakPtr owner;
      vec2 position, velocity;
	  mat2 orientation;
   };
   
}

#endif /* end of include guard: BODY_H_JI764YY5 */
