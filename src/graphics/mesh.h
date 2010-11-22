/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */


#ifndef GRAPHICS_MESH_H
#define GRAPHICS_MESH_H

#include <vector>
#include "vertex.h"

namespace Graphics {
   class Mesh {
   public:
      std::vector<Vertex2T2> vertices;
   };

}

#endif // !GRAPHICS_MESH_H
