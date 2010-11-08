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

#include "math/vec2.h"
#include "math/rect.h"

namespace Physics {
   class Body;
   class Geom;
   
   class Subsystem {
   public:
      Subsystem();
      
      void update(float dt);
      void resizeArea(int width, int height);
      void addBody(const boost::weak_ptr<Body> & body);
	  void drawGeoms();
	  
      boost::shared_ptr<Body> createBody();
      boost::shared_ptr<Geom> createRectGeom(const rect & size);
      
   private:
      void checkCollisions();
      
      // vec2 maxArea;
      // rect leftArea, rightArea;
      std::vector<boost::weak_ptr<Body> > bodies;
      std::vector<boost::weak_ptr<Geom> > geoms;
   };
   
   
}

// might be able to remove HiController
// HiController -> PlayerLogic -> PhysicsThingie ->        Sprite
// (udlr, etc)     (restrain)    (restrain, collision)    (render)

// AiController -> PlayerLogic -> PhysicsThingie ->        Sprite

#endif /* end of include guard: PHYS_SUBSYSTEM_H_YCDJQ9A8 */
