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
#include "render_context.h"

namespace Graphics {
   class Sprite;
   class BoundedSprite;
   class Texture;
   class RenderList;
   
   class Subsystem {
   public:
      Subsystem();
      
      void render(float dt);
      void resizeViewport(const rect & size);
      Ref<Graphics::Sprite>::SharedPtr createSprite(const std::string & fragments);
	  Ref<Graphics::Texture>::SharedPtr getTexture(const std::string & filename);
	  //  Ref<Graphics::RenderList>::SharedPtr createRenderList();
	  void enqueueVisibleSprites(const Ref<Graphics::RenderList>::SharedPtr & renderList);

	  void beginFrame();
//	  void render(const Ref<Graphics::Texture>::SharedPtr & texture, const std::vector<Vertex2T2> & vertices);
	  Graphics::RenderContext & getContext();
	  
   private:
      TextureLoader textureCache;
      std::vector<Graphics::BoundedSprite *> sprites;
      rect viewport;
	  Graphics::RenderContext renderCtx;
   };

   class BoundedSprite {
   private:
      BoundedSprite() {visibleLastFrame = false; firstFrame = true; }
      
      Ref<Graphics::Sprite>::WeakPtr sprite;

   public:
      rect boundingArea;
      bool visibleLastFrame, firstFrame;
      
      friend class Graphics::Subsystem;
   };

}

#endif /* end of include guard: GFX_SUBSYSTEM_H_EAH7WN3F */

