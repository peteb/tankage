/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef GRAPHICS_TEXTURE_FX_H
#define GRAPHICS_TEXTURE_FX_H

#include "graphics/renderer.h"
#include "ref.h"

namespace Graphics
{
   class Texture;
   
   class TextureFx : public Graphics::Renderer
   {
   public:
	  void render(const Ref<Graphics::Mesh>::SharedPtr & mesh, RenderContext & rc);
	  void setTexture(const Ref<Graphics::Texture> & texture);

   private:
	  Ref<Graphics::Texture> & texture;
   };

}

#endif // !GRAPHICS_TEXTURE_FX_H
