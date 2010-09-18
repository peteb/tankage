/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef SPRITE_H_CD2WG4HZ
#define SPRITE_H_CD2WG4HZ

#include <vector>
#include "vec2.h"

namespace Graphics {
   class Sprite {
   public:
      Sprite();
      
      std::vector<vec2> constructVertices() const;
      
      vec2 position;
   };
   
}

#endif /* end of include guard: SPRITE_H_CD2WG4HZ */
