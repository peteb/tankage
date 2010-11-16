/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef TEXTURE_H_61TB0IDZ
#define TEXTURE_H_61TB0IDZ

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <windows.h>
#include <GL/GL.h>
#endif

#include "math/rect.h"

namespace Graphics {
   class Texture {
   public:
	  ~Texture();
   
	  rect getSize() const;
   
	  GLuint getTexId() const;
   
   private:
	  Texture(GLuint texId, const rect & size);
   
	  friend class TextureLoader;

	  rect size;
	  GLuint texId;
   };
}

#endif /* end of include guard: TEXTURE_H_61TB0IDZ */
