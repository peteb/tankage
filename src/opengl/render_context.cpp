/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "render_context.h"
#include "texture.h"

void OpenGL::RenderContext::setTexture(const Ref<Graphics::Texture>::SharedPtr & texture) {
   const GLuint texId = Cast<OpenGL::Texture>(texture)->getTexId();
   glBindTexture(GL_TEXTURE_2D, texId);
}

