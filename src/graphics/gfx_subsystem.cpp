/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "gfx_subsystem.h"
#include "sprite.h"
#include "vertex.h"
#include "texture.h"
#include "render_list.h"
#include "texture_fx.h"
#include "opengl/device.h"
#include "color.h"
#include "texture_fx.h"

GfxSubsystem::GfxSubsystem() 
  : viewport(vec2::Zero())
{
  renderDevice = Owning(new OpenGL::Device);
  renderContext = Owning(renderDevice->createContext());
  textureCache.setDevice(renderDevice.lock());
}


void GfxSubsystem::enqueueVisibleSprites(const Ref<Graphics::RenderList>::SharedPtr & renderList) {
  screen.enqueueVisibleSprites(renderList, viewport);
}


void GfxSubsystem::resizeViewport(const rect & size) {
  viewport = size;
}


void GfxSubsystem::beginFrame() {
  renderDevice->setOrtho(viewport);
  renderDevice->clearColor(Graphics::Color(0.957f, 0.917f, 0.682f, 1.0f));
}


void GfxSubsystem::render(const Ref<Graphics::RenderList>::SharedPtr & renderList) {
  renderList->render(*(*renderDevice).get());
}


Ref<Sprite>::SharedPtr GfxSubsystem::createSprite(const std::string & fragments) {
  Ref<Graphics::Texture>::SharedPtr spriteTexture = textureCache.loadTexture(fragments);
  Ref<Graphics::TextureFx>::SharedPtr renderer(new Graphics::TextureFx);
  renderer->setTexture(Owning(spriteTexture));
  renderer->setRenderContext(renderContext);
   
  Ref<Sprite>::SharedPtr newSprite(new Sprite(renderer, renderer->getNativeSize()));
  screen.insert(newSprite);
   
  return newSprite;   
}


Ref<Graphics::Texture>::SharedPtr GfxSubsystem::getTexture(const std::string & filename) {
  return textureCache.loadTexture(filename);
}


Ref<Graphics::Renderer>::SharedPtr GfxSubsystem::getRenderer(const std::string & name) {
  Ref<Graphics::TextureFx>::SharedPtr newRenderer(new Graphics::TextureFx);
  newRenderer->setTexture(Owning(getTexture(name)));
  newRenderer->setRenderContext(renderContext);
   
  return newRenderer;
}

Ref<Graphics::RenderContext>::SharedPtr GfxSubsystem::getRenderContext() const {
  return renderContext.lock();
}
