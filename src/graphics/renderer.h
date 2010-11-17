/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef GRAPHICS_RENDERER_H
#define GRAPHICS_RENDERER_H

namespace Graphics {
   class Mesh;
   class RenderContext;
   
   class Renderer {
   public:
	  virtual ~Renderer() {}

	  virtual void render(const Mesh & mesh, RenderContext & rc) {}
   };

}

#endif // !GRAPHICS_RENDERER_H
