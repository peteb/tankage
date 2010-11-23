/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef GRAPHICS_RENDER_CONTEXT_H
#define GRAPHICS_RENDER_CONTEXT_H

#include "ref.h"

namespace Graphics {
   class Texture;
   
   class RenderContext {
   public:
	  virtual ~RenderContext() {}

	  virtual void bind() const =0;
	  virtual void setBlend(bool useBlending) =0;
	  
	  //virtual void setTexture(const Ref<Texture>::SharedPtr & texture) =0;
   };
}

#endif // !GRAPHICS_RENDER_CONTEXT_H
