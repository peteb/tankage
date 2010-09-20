/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef PHYS_SUBSYSTEM_H_YCDJQ9A8
#define PHYS_SUBSYSTEM_H_YCDJQ9A8

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "vec2.h"
#include "rect.h"

namespace Physics {
   class Body;
   
   class Subsystem {
   public:
      Subsystem();
      
      void update(float dt);
      void resizeArea(int width, int height);
      boost::shared_ptr<Body> createBody(const std::string & area);
      
   private:
      vec2 maxArea;
      rect leftArea, rightArea;
      std::vector<boost::weak_ptr<Body> > bodies;
   };
   
}

// might be able to remove HiController
// HiController -> PlayerLogic -> PhysicsThingie ->        Sprite
// (udlr, etc)     (restrain)    (restrain, collision)    (render)

// AiController -> PlayerLogic -> PhysicsThingie ->        Sprite

#endif /* end of include guard: PHYS_SUBSYSTEM_H_YCDJQ9A8 */
