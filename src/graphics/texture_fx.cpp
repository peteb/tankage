/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "texture_fx.h"
#include "graphics/texture.h"

void Graphics::TextureFx::render(const Ref<Graphics::Mesh>::SharedPtr & mesh, RenderContext & rc)
{
   texture->bind();
//   mesh.draw();
}

void Graphics::TextureFx::setTexture(const Ref<Graphics::Texture> & texture)
{
   this->texture = texture;
}

