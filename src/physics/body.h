/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef BODY_H_JI764YY5
#define BODY_H_JI764YY5

#include <boost/weak_ptr.hpp>
#include "reference_frame.h"

namespace Physics {
   class Subsystem;

   /*
    * A physical body that can be acted upon through forces
    */
   class Body : public ReferenceFrame2D {
   private:
      Body(Physics::Subsystem & subsystem); 
      
   public:
      
      friend class Physics::Subsystem;

      void update(float dt);
      void setDelegate(const boost::weak_ptr<ReferenceFrame2D> & newTarget);
 
      void setPosition(const vec2 & pos);
      vec2 getPosition() const;
      
   private:
      Physics::Subsystem & subsystem;
      boost::weak_ptr<ReferenceFrame2D> delegate;
      vec2 position, velocity;
   };
   
}

#endif /* end of include guard: BODY_H_JI764YY5 */
