/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef SPRITE_H_CD2WG4HZ
#define SPRITE_H_CD2WG4HZ

#include <vector>
#include "vec2.h"
#include "reference_frame.h"

namespace Graphics {
   class Sprite : public ReferenceFrame2D {
   public:
      Sprite();
      
      std::vector<vec2> constructVertices() const;
      
      void setPosition(const vec2 & newPos);
      vec2 getPosition() const;

   private:
      vec2 position;
   };
   
}

#endif /* end of include guard: SPRITE_H_CD2WG4HZ */
