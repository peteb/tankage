/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef OPENGL_DEVICE_H
#define OPENGL_DEVICE_H

#include "graphics/device.h"
#include "render_context.h"
#include "texture.h"

namespace OpenGL {
   
   class Device : public Graphics::Device {
   public:
	  OpenGL::RenderContext * createContext();
	  OpenGL::Texture * createTexture(const void * data, int width, int height, int bpp, int format);
   };
}

#endif // !OPENGL_DEVICE_H
