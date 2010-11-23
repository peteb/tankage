/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "render_context.h"
#include "texture.h"
#include "opengl.h"

OpenGL::RenderContext::RenderContext()
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
}


void OpenGL::RenderContext::setBlend(bool useBlending)
{
   this->useBlending = useBlending;
}

