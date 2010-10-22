/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "texture.h"
#include <iostream>

Texture::Texture(GLuint texId, const rect & size)
   : texId(texId)
   , size(size)
{
   std::cout << "created texId " << texId << std::endl;
}

Texture::~Texture() {
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &texId);
}

rect Texture::getSize() const {
   return size;
}

GLuint Texture::getTexId() const {
   return texId;
}