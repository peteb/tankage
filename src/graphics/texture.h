/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef TEXTURE_H_61TB0IDZ
#define TEXTURE_H_61TB0IDZ

#include <OpenGL/OpenGL.h>

class Texture {
public:
   
private:
   Texture(GLuint texId);
   
   friend class TextureLoader;

   GLuint texId;
};


#endif /* end of include guard: TEXTURE_H_61TB0IDZ */
