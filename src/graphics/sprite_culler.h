/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */


#ifndef GRAPHICS_SPRITE_CULLER_H
#define GRAPHICS_SPRITE_CULLER_H

#include "ref.h"
#include <vector>

class rect;
class Sprite;

namespace Graphics {
   class RenderList;
   
   class SpriteCuller {
   public:
	  void insert(const Ref<Sprite>::WeakPtr sprite);
	  void enqueueVisibleSprites(const Ref<Graphics::RenderList>::SharedPtr & renderList, const rect & viewport);

   private:
	  class SpriteMetadata {
	  public:
		 SpriteMetadata(const Ref<Sprite>::WeakPtr & sprite);

		 bool visibleLastFrame;
		 bool firstFrame;
		 Ref<Sprite>::WeakPtr sprite;
	  };
	  
	  std::vector<SpriteMetadata> sprites;
   };

}

#endif // !GRAPHICS_SPRITE_CULLER_H
