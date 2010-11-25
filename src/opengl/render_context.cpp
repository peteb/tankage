/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "render_context.h"
#include "texture.h"
#include "opengl.h"

OpenGL::RenderContext::RenderContext()
   : color(1.0f, 1.0f, 1.0f, 1.0f)
{
   useBlending = false;
}

void OpenGL::RenderContext::bind() const
{
   if (useBlending) {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   }
   else {
      glDisable(GL_BLEND);
   }

   glColor4f(color.r, color.g, color.b, color.a);
}


void OpenGL::RenderContext::setBlend(bool useBlending)
{
   this->useBlending = useBlending;
}


void OpenGL::RenderContext::setColor(const Graphics::Color & color)
{
   // TODO: this sucks.
   this->color = color;
}
