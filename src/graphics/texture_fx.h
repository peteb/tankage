/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef GRAPHICS_TEXTURE_FX_H
#define GRAPHICS_TEXTURE_FX_H

#include "graphics/renderer.h"
#include "graphics/color.h"
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
      TextureFx();
      
      void render(const Ref<Graphics::Mesh>::SharedPtr & mesh, Graphics::Device & target);
	   
      void setTexture(const Ref<Graphics::Texture> & texture);
      void setColor(const Graphics::Color & color);
      void setRenderContext(const Ref<Graphics::RenderContext> & context);
      rect getNativeSize() const;
      
   private:
      Ref<Graphics::Texture> texture;
      Ref<Graphics::RenderContext> context;
      Graphics::Color color;
   };

}

#endif // !GRAPHICS_TEXTURE_FX_H
