/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef GFX_SUBSYSTEM_H_EAH7WN3F
#define GFX_SUBSYSTEM_H_EAH7WN3F

#include <vector>
#include <utils/vec.h>
#include <utils/rect.h>
#include "texture_loader.h"
#include "ref.h"
#include "vertex.h"
#include "sprite_culler.h"

class Sprite;

namespace Graphics {
class Texture;
class RenderList;
class RenderContext;
class Renderer;
}

class GfxSubsystem {
public:
  GfxSubsystem();
  
  void resizeViewport(const rect & size);
  Ref<Sprite>::SharedPtr createSprite(const std::string & fragments);
  Ref<Graphics::Renderer>::SharedPtr getRenderer(const std::string & name);
	  
  void enqueueVisibleSprites(const Ref<Graphics::RenderList>::SharedPtr & renderList);
  Ref<Graphics::RenderContext>::SharedPtr getRenderContext() const;
  
  void beginFrame();
  void render(const Ref<Graphics::RenderList>::SharedPtr & renderList);
  
private:
  Ref<Graphics::Texture>::SharedPtr getTexture(const std::string & filename);
  
  Graphics::SpriteCuller screen;
  Graphics::TextureLoader textureCache;
  rect viewport;
  Ref<Graphics::Device> renderDevice;
  Ref<Graphics::RenderContext> renderContext;
};

#endif /* end of include guard: GFX_SUBSYSTEM_H_EAH7WN3F */

