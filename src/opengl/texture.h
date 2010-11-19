/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include "graphics/texture.h"
#include "math/rect.h"
#include "opengl.h"


namespace OpenGL
{
   class Device;
   
   class Texture : public Graphics::Texture
   {
   public:
	  Texture(GLuint texId, const rect & size);
	  ~Texture();
   
	  rect getSize() const;
	  GLuint getTexId() const;
	  void bind() const;
	  
   private:
	  Texture(const Texture & other);
	  
	  const rect size;
	  const GLuint texId;
   };

}

#endif // !OPENGL_TEXTURE_H
