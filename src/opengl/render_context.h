/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef OPENGL_RENDER_CONTEXT_H
#define OPENGL_RENDER_CONTEXT_H

#include "graphics/render_context.h"

namespace OpenGL {
   class RenderContext : public Graphics::RenderContext {
   public:
	  // void setTexture(const Ref<Graphics::Texture>::SharedPtr & texture);
	  void bind() const;
   };

}

#endif // !OPENGL_RENDER_CONTEXT_H
