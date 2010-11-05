/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef GFX_SUBSYSTEM_H_EAH7WN3F
#define GFX_SUBSYSTEM_H_EAH7WN3F

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include "math/vec2.h"
#include "math/rect.h"
#include "texture_loader.h"

namespace Graphics {
   class Sprite;
   class BoundedSprite;
   class Texture;
   
   class Subsystem {
   public:
      Subsystem();
      
      void render(float dt);
      void resizeViewport(const rect & size);
      boost::shared_ptr<Graphics::Sprite> createSprite(const std::string & fragments);
	  boost::shared_ptr<Graphics::Texture> getTexture(const std::string & filename);

   private:
      TextureLoader textureCache;
      std::vector<Graphics::BoundedSprite *> sprites;
      rect viewport;
   };

   class BoundedSprite {
   private:
      BoundedSprite() {visibleLastFrame = false; firstFrame = true; }
      
      boost::weak_ptr<Graphics::Sprite> sprite;

   public:
      rect boundingArea;
      bool visibleLastFrame, firstFrame;
      
      friend class Graphics::Subsystem;
   };

}

#endif /* end of include guard: GFX_SUBSYSTEM_H_EAH7WN3F */

