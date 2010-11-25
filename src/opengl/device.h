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

namespace Graphics {
   class Mesh;
}

namespace OpenGL {
   
   class Device : public Graphics::Device {
   public:	  
	  OpenGL::RenderContext * createContext();
	  OpenGL::Texture * createTexture(const void * data, int width, int height, int bpp, int format);
	  void drawMesh(const Ref<Graphics::Mesh>::SharedPtr & mesh);
	  void setOrtho(const rect & viewport);
	  void clearColor(const Graphics::Color & color);
   };
}

#endif // !OPENGL_DEVICE_H
