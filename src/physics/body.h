/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef BODY_H_JI764YY5
#define BODY_H_JI764YY5

#include <boost/weak_ptr.hpp>
#include "reference_frame2.h"
#include "ref.h"

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
 
      // ReferenceFrame2 ---------------------------------------
      void setPosition(const vec2 & pos);
      vec2 getPosition() const;
      void setOrientation(const mat2 & orient);
      mat2 getOrientation() const;
      
   private:
      Physics::Subsystem & subsystem;
      Ref<ReferenceFrame2> delegate;
      vec2 position, velocity;
   };
   
}

#endif /* end of include guard: BODY_H_JI764YY5 */
