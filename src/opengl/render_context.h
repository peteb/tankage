/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef OPENGL_RENDER_CONTEXT_H
#define OPENGL_RENDER_CONTEXT_H

#include "graphics/render_context.h"
#include "graphics/color.h"

namespace OpenGL {
   class RenderContext : public Graphics::RenderContext {
   public:
	  RenderContext();
	  
	  // void setTexture(const Ref<Graphics::Texture>::SharedPtr & texture);
	  void bind() const;
	  void setBlend(bool useBlending);
	  void setColor(const Graphics::Color & color);

   private:
	  bool useBlending;
	  Graphics::Color color;
   };

}

#endif // !OPENGL_RENDER_CONTEXT_H