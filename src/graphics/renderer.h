/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef GRAPHICS_RENDERER_H
#define GRAPHICS_RENDERER_H

#include "ref.h"
#include "math/rect.h"

namespace Graphics {
   class Mesh;
   class Device;
   
   class Renderer {
   public:
	  virtual ~Renderer() {}

     virtual void render(const Ref<Graphics::Mesh>::SharedPtr & mesh, Device & target) =0;
     virtual rect getNativeSize() const =0;
   };

}

#endif // !GRAPHICS_RENDERER_H
