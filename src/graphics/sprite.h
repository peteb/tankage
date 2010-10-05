/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef SPRITE_H_CD2WG4HZ
#define SPRITE_H_CD2WG4HZ

#include <vector>
#include <boost/shared_ptr.hpp>
#include "vec2.h"
#include "reference_frame.h"
#include "graphics/vertex.h"

class Texture;

namespace Graphics {
   class Sprite : public ReferenceFrame2D {
   public:
      Sprite(const boost::shared_ptr<Texture> & texture);
      
      std::vector<Vertex2T2> constructVertices() const;
      
      void setPosition(const vec2 & newPos);
      vec2 getPosition() const;

   private:
      boost::shared_ptr<Texture> texture;
      vec2 position;
   };
   
}

#endif /* end of include guard: SPRITE_H_CD2WG4HZ */
