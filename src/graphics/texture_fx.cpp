/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "texture_fx.h"
#include "graphics/texture.h"
#include "graphics/render_context.h"
#include "graphics/device.h"

void Graphics::TextureFx::render(const Ref<Graphics::Mesh>::SharedPtr & mesh, Graphics::Device & target)
{
   texture->bind();
   context->bind();
   target.drawMesh(mesh);
}

void Graphics::TextureFx::setTexture(const Ref<Graphics::Texture> & texture)
{
   this->texture = texture;
}

void Graphics::TextureFx::setRenderContext(const Ref<Graphics::RenderContext> & context) 
{
   this->context = context;
}

rect Graphics::TextureFx::getNativeSize() const 
{
   return texture->getSize();
}