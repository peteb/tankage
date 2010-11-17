/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "device.h"
#include "texture.h"
#include "math/rect.h"
#include "opengl.h"
#include "render_context.h"


#include <OpenGL/OpenGL.h>

OpenGL::RenderContext * OpenGL::Device::createContext() {
   return NULL;
}

OpenGL::Texture * OpenGL::Device::createTexture(const void * data, int width, int height, int bpp, int format) {
   GLuint newTexture;
//   glEnable(GL_TEXTURE_2D);
   glGenTextures(1, &newTexture);
   glBindTexture(GL_TEXTURE_2D, newTexture);
   glTexImage2D(
      GL_TEXTURE_2D, 0, bpp, width, height, 0, format,
      GL_UNSIGNED_BYTE, data
   );
   
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   
   return new OpenGL::Texture(newTexture, rect(width, height));
}

