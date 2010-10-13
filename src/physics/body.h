/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef BODY_H_JI764YY5
#define BODY_H_JI764YY5


namespace Physics {
   class Subsystem;

   /*
    * A physical body that can be acted upon through forces
    */
   class Body {
   private:
      Body(Physics::Subsystem & subsystem);
      
   public:
      
      friend class Physics::Subsystem;
      
   private:
      Physics::Subsystem & subsystem;

   };
   
}

#endif /* end of include guard: BODY_H_JI764YY5 */
