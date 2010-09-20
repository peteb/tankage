/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef GFX_SUBSYSTEM_H_EAH7WN3F
#define GFX_SUBSYSTEM_H_EAH7WN3F

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include "vec2.h"

namespace Graphics {
   class Sprite;
   
   class Subsystem {
   public:
      Subsystem();
      
      void render(float dt);
      void resizeViewport(const vec2 & size);
      boost::shared_ptr<Sprite> createSprite();
      
   private:
      std::vector<boost::weak_ptr<Graphics::Sprite> > sprites;
      vec2 viewport;
   };
}

#endif /* end of include guard: GFX_SUBSYSTEM_H_EAH7WN3F */

