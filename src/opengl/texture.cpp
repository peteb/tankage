/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "texture.h"
#include "opengl.h"

OpenGL::Texture::Texture(GLuint texId, const rect & size)
   : texId(texId)
   , size(size)
{
}

OpenGL::Texture::~Texture() {
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &texId);
}

rect OpenGL::Texture::getSize() const {
   return size;
}

GLuint OpenGL::Texture::getTexId() const {
   return texId;
}

void OpenGL::Texture::bind() const {
   glBindTexture(GL_TEXTURE_2D, texId);
}
