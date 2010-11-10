/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "graphics/gfx_subsystem.h"
#include "graphics/sprite.h"
#include "graphics/vertex.h"
#include "graphics/texture.h"
#include "graphics/render_list.h"

#include <OpenGL/OpenGL.h>

 
Graphics::Subsystem::Subsystem() 
   : viewport(vec2::Zero)
{
}

#include <iostream>
void Graphics::Subsystem::render(float dt) {
   glClearColor(0.957, 0.917, 0.682, 1.0f);
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
   
   glEnable(GL_TEXTURE_2D);
   
   int rendered = 0;
   
   std::vector<Graphics::BoundedSprite *>::iterator iter = sprites.begin();
   
   while (iter != sprites.end()) {
      BoundedSprite * bs = *iter;
      
      if (Ref<Graphics::Sprite>::SharedPtr sprite = bs->sprite.lock()) {
         if (rect::intersect(bs->boundingArea, viewport)) {
            if (!bs->visibleLastFrame || bs->firstFrame) {
               sprite->enteredView();
               bs->visibleLastFrame = true;
            }
            
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, sprite->getTexture()->getTexId());
            glBegin(GL_QUADS);
            glColor3f(1.0f, 1.0f, 1.0f);
            rendered++;
            std::vector<Vertex2T2> vertices; // = sprite->constructVertices();
            
            for (unsigned i = 0; i < vertices.size(); ++i) {
               const vec2 pos = vertices[i].pos + sprite->getPosition();
               glTexCoord2f(vertices[i].tc.x, vertices[i].tc.y);
               glVertex2f(pos.x, pos.y);
            }
      
            glEnd();
            rendered++;
            
            if (showBoundingAreas) {
               glColor4f(0.0f, 0.0f, 1.0f, 0.5f);
               glDisable(GL_TEXTURE_2D);
               glBegin(GL_QUADS);
               vec2 min, max;
               bs->boundingArea.getCoords(min, max);
               glVertex2f(min.x, min.y);
               glVertex2f(max.x, min.y);
               glVertex2f(max.x, max.y);
               glVertex2f(min.x, max.y);
               glEnd();
            }

            ++iter;
         }
         else {
            if (bs->visibleLastFrame || bs->firstFrame) {
               if (Ref<Graphics::Sprite>::SharedPtr sprite = bs->sprite.lock()) {
                  sprite->leftView();
               }
            }
            bs->visibleLastFrame = false;
            ++iter;
         }         
         
         bs->firstFrame = false;
      }
      else {
         delete bs;
         iter = sprites.erase(iter);
      
      }
   }
   
  // std::cout << "Rendered: " << rendered << std::endl;
}

void Graphics::Subsystem::enqueueVisibleSprites(const Ref<Graphics::RenderList>::SharedPtr & renderList) {
   std::vector<Graphics::BoundedSprite *>::iterator iter = sprites.begin();
   
   while (iter != sprites.end()) {
      BoundedSprite * bs = *iter;
      
      if (Ref<Graphics::Sprite>::SharedPtr sprite = bs->sprite.lock()) {
         if (rect::intersect(bs->boundingArea, viewport)) {
            if (!bs->visibleLastFrame || bs->firstFrame) {
               sprite->enteredView();
               bs->visibleLastFrame = true;
            }
            
			sprite->enqueueRender(renderList);
            ++iter;
         }
         else {
            if (bs->visibleLastFrame || bs->firstFrame) {
               if (Ref<Graphics::Sprite>::SharedPtr sprite = bs->sprite.lock()) {
                  sprite->leftView();
               }
            }
            bs->visibleLastFrame = false;
            ++iter;
         }         
         
         bs->firstFrame = false;
      }
      else {
         delete bs;
         iter = sprites.erase(iter);
      
      }
   }
}

void Graphics::Subsystem::render(const Ref<Graphics::Texture>::SharedPtr & texture, const std::vector<Vertex2T2> & vertices) {
   glEnable(GL_TEXTURE_2D);
   if (texture.get())
	  glBindTexture(GL_TEXTURE_2D, texture->getTexId());
   else
	  glBindTexture(GL_TEXTURE_2D, 0);
   
   glBegin(GL_QUADS);
   glColor3f(1.0f, 1.0f, 1.0f);
            
   for (unsigned i = 0; i < vertices.size(); ++i) {
	  const vec2 pos = vertices[i].pos;
	  glTexCoord2f(vertices[i].tc.x, vertices[i].tc.y);
	  glVertex2f(pos.x, pos.y);
   }
      
   glEnd();
}

void Graphics::Subsystem::resizeViewport(const rect & size) {
   viewport = size;
}

void Graphics::Subsystem::beginFrame() {
   glClearColor(0.957, 0.917, 0.682, 1.0f);
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

Ref<Graphics::Sprite>::SharedPtr Graphics::Subsystem::createSprite(const std::string & fragments) {
   Ref<Graphics::Texture>::SharedPtr spriteTexture = textureCache.loadTexture(fragments);
   
   BoundedSprite * node = new BoundedSprite;
   node->boundingArea = spriteTexture->getSize();
   node->boundingArea.origin = vec2::Zero;

   Ref<Graphics::Sprite>::SharedPtr newSprite(new Sprite(spriteTexture, node));   
   node->sprite = newSprite;
   
   sprites.push_back(node);
   
   return newSprite;   
}

Ref<Graphics::Texture>::SharedPtr Graphics::Subsystem::getTexture(const std::string & filename) {
   return textureCache.loadTexture(filename);
}

Ref<Graphics::RenderList>::SharedPtr Graphics::Subsystem::createRenderList() {
   Ref<Graphics::RenderList>::SharedPtr newList(new Graphics::NonSortingRenderList);

   return newList;
}
