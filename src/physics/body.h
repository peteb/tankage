/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef BODY_H_JI764YY5
#define BODY_H_JI764YY5

class rect;
 
namespace Physics {
   class Subsystem;
   
   class Body {
   private:
      Body(Physics::Subsystem & subsystem, rect * area);
      
   public:
      
      friend class Physics::Subsystem;
      
   private:
      Physics::Subsystem & subsystem;
      rect * area;
   };
   
}

#endif /* end of include guard: BODY_H_JI764YY5 */
