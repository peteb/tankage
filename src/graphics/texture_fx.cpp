/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "texture_fx.h"
#include "graphics/texture.h"
#include "graphics/render_context.h"
#include "graphics/device.h"

Graphics::TextureFx::TextureFx()
   : color(1.0f, 1.0f, 1.0f, 1.0f)
{
}

void Graphics::TextureFx::render(const Ref<Graphics::Mesh>::SharedPtr & mesh, Graphics::Device & target)
{
   context->setBlend(true);
   context->setColor(color);
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


void Graphics::TextureFx::setColor(const Graphics::Color & color)
{
   this->color = color;
}


rect Graphics::TextureFx::getNativeSize() const 
{
   return texture->getSize();
}
