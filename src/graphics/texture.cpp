/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "texture.h"
#include <iostream>

Texture::Texture(GLuint texId)
   : texId(texId)
{
   std::cout << "created texId " << texId << std::endl;
}