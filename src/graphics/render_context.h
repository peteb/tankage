/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef GRAPHICS_RENDER_CONTEXT_H
#define GRAPHICS_RENDER_CONTEXT_H

namespace Graphics {
   class RenderContext {
   public:
	  virtual ~RenderContext();

	  virtual void setColor(float r, float g, float b, float a);
   };
}

#endif // !GRAPHICS_RENDER_CONTEXT_H
