/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef GRAPHICS_TEXTURE_FX_H
#define GRAPHICS_TEXTURE_FX_H

#include "graphics/renderer.h"
#include "ref.h"
#include "math/rect.h"

namespace Graphics
{
   class Texture;
   class Device;
   class RenderContext;
   
   class TextureFx : public Graphics::Renderer
   {
   public:
	   void render(const Ref<Graphics::Mesh>::SharedPtr & mesh, Graphics::Device & target);
	   
	   void setTexture(const Ref<Graphics::Texture> & texture);
      void setRenderContext(const Ref<Graphics::RenderContext> & context);
      rect getNativeSize() const;
      
   private:
	   Ref<Graphics::Texture> texture;
      Ref<Graphics::RenderContext> context;
   };

}

#endif // !GRAPHICS_TEXTURE_FX_H
