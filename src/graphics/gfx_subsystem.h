/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef GFX_SUBSYSTEM_H_EAH7WN3F
#define GFX_SUBSYSTEM_H_EAH7WN3F

#include <vector>
#include "math/vec2.h"
#include "math/rect.h"
#include "texture_loader.h"
#include "ref.h"
#include "vertex.h"
#include "sprite_culler.h"

namespace Graphics {
   class Sprite;
   class Texture;
   class RenderList;
   class RenderContext;
   
   class Subsystem {
   public:
      Subsystem();
      
      void render(float dt);
      void resizeViewport(const rect & size);
      Ref<Graphics::Sprite>::SharedPtr createSprite(const std::string & fragments);
      Ref<Graphics::Texture>::SharedPtr getTexture(const std::string & filename);
      void enqueueVisibleSprites(const Ref<Graphics::RenderList>::SharedPtr & renderList);
      Ref<Graphics::RenderContext>::SharedPtr getRenderContext() const;
      
      void beginFrame();
      void render(const Ref<Graphics::RenderList>::SharedPtr & renderList);
      
   private:
      SpriteCuller screen;
      TextureLoader textureCache;
      rect viewport;
      Ref<Graphics::Device> renderDevice;
      Ref<Graphics::RenderContext> renderContext;
   };
}

#endif /* end of include guard: GFX_SUBSYSTEM_H_EAH7WN3F */

