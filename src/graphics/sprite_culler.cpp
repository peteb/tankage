/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "sprite_culler.h"
#include "sprite.h"

Graphics::SpriteCuller::SpriteMetadata::SpriteMetadata(const Ref<Sprite>::WeakPtr & sprite)
   : sprite(sprite)
{
   visibleLastFrame = false;
   firstFrame = true;
}

void Graphics::SpriteCuller::insert(const Ref<Sprite>::WeakPtr sprite) {
   Graphics::SpriteCuller::SpriteMetadata spriteMd(sprite);
   sprites.push_back(spriteMd);
}

void Graphics::SpriteCuller::enqueueVisibleSprites(const Ref<Graphics::RenderList>::SharedPtr & renderList, const rect & viewport) {
   std::vector<SpriteMetadata>::iterator iter = sprites.begin();
   
   while (iter != sprites.end()) {
      SpriteMetadata & metadata = *iter;
      
      if (Ref<Sprite>::SharedPtr sprite = metadata.sprite.lock()) {
		 // If the sprite still exists
		 
         if (rect::intersect(sprite->getBoundingBox(), viewport)) {
			// If the sprite intersects the viewport
			
			if (!metadata.visibleLastFrame || metadata.firstFrame) {
               sprite->enteredView();
               metadata.visibleLastFrame = true;
            }
            
			sprite->enqueueRender(renderList);
         }
         else {
			// If it's completely outside the viewport
			if (metadata.visibleLastFrame && !metadata.firstFrame) {
               if (Ref<Sprite>::SharedPtr sprite = metadata.sprite.lock()) {
                  sprite->leftView();
               }
            }

			metadata.visibleLastFrame = false;
         }         
         
         metadata.firstFrame = false;
		 ++iter;
      }
      else {
		 // Failed to lock sprite ptr, remove from list
         iter = sprites.erase(iter);
      }
   }
}
