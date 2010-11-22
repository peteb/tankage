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

Graphics::Subsystem::Subsystem() 
   : viewport(vec2::Zero)
{
   renderDevice = new OpenGL::Device;
   renderContext = Owning(renderDevice->createContext());
   textureCache.setDevice(renderDevice);
}

void Graphics::Subsystem::enqueueVisibleSprites(const Ref<Graphics::RenderList>::SharedPtr & renderList) {
   screen.enqueueVisibleSprites(renderList, viewport);
}

void Graphics::Subsystem::resizeViewport(const rect & size) {
   viewport = size;
}

void Graphics::Subsystem::beginFrame() { // TODO: move into opengl
   glClearColor(0.957f, 0.917f, 0.682f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT);

   vec2 min, max;
   viewport.getCoords(min, max);
   glViewport(int(min.x), int(min.y), int(max.x - min.x), int(max.y - min.y));
   
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   
   const float scaleX = 1.0f / viewport.halfSize.x;
   const float scaleY = 1.0f / viewport.halfSize.y;

   bool showBoundingAreas = false;
   
   glTranslatef(-1.0f, 1.0f, 0.0f);
   glScalef(scaleX, -scaleY, 1.0f);
   
   glMatrixMode(GL_TEXTURE);
   glLoadIdentity();

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void Graphics::Subsystem::render(const Ref<Graphics::RenderList>::SharedPtr & renderList) {
   renderList->render(*renderDevice);
}

Ref<Graphics::Sprite>::SharedPtr Graphics::Subsystem::createSprite(const std::string & fragments) {
   Ref<Graphics::Texture>::SharedPtr spriteTexture = textureCache.loadTexture(fragments);
   Ref<Graphics::TextureFx>::SharedPtr renderer(new TextureFx);
   renderer->setTexture(Owning(spriteTexture));
   renderer->setRenderContext(renderContext);
   
   Ref<Graphics::Sprite>::SharedPtr newSprite(new Sprite(renderer, renderer->getNativeSize()));
   screen.insert(newSprite);
   
   return newSprite;   
}

Ref<Graphics::Texture>::SharedPtr Graphics::Subsystem::getTexture(const std::string & filename) {
   return textureCache.loadTexture(filename);
}

Ref<Graphics::RenderContext>::SharedPtr Graphics::Subsystem::getRenderContext() const {
   return renderContext.lock();
}