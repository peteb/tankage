/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "graphics/gfx_subsystem.h"
#include "graphics/sprite.h"
#include "graphics/vertex.h"
#include "graphics/texture.h"
#include <OpenGL/OpenGL.h>
#include <boost/make_shared.hpp>

using namespace Graphics;
 
Subsystem::Subsystem() 
   : viewport(vec2::Zero)
{
}

#include <iostream>
void Subsystem::render(float dt) {
   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT);

   vec2 min, max;
   viewport.getCoords(min, max);
   glViewport(int(min.x), int(min.y), int(max.x - min.x), int(max.y - min.y));
   
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   
   const float scaleX = 1.0f / viewport.halfSize.x;
   const float scaleY = 1.0f / viewport.halfSize.y;

   bool showBoundingAreas = true;
   
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
   
   std::vector<BoundedSprite *>::iterator iter = sprites.begin();
   
   while (iter != sprites.end()) {
      BoundedSprite * bs = *iter;
      
      if (boost::shared_ptr<Sprite> sprite = bs->sprite.lock()) {
         if (rect::intersect(bs->boundingArea, viewport)) {
            if (!bs->visibleLastFrame) {
               sprite->enteredView();
               bs->visibleLastFrame = true;
            }
            
            glEnable(GL_TEXTURE_2D);
            glBegin(GL_QUADS);
            glColor3f(1.0f, 1.0f, 1.0f);
            rendered++;
            std::vector<Vertex2T2> vertices = sprite->constructVertices();
            
            for (unsigned i = 0; i < vertices.size(); ++i) {
               const vec2 pos = vertices[i].pos + sprite->getPosition();
               glTexCoord2f(vertices[i].tc.x, vertices[i].tc.y);
               glVertex2f(pos.x, pos.y);
            }
      
            glEnd();

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
            if (bs->visibleLastFrame) {
               if (boost::shared_ptr<Sprite> sprite = bs->sprite.lock()) {
                  sprite->leftView();
                  bs->visibleLastFrame = false;
               }
            }
            ++iter;
         }         
      }
      else {
         delete bs;
         iter = sprites.erase(iter);
      
      }
   }
   
  // std::cout << "Rendered: " << rendered << std::endl;
}

void Subsystem::resizeViewport(const rect & size) {
   viewport = size;
}

boost::shared_ptr<Sprite> Subsystem::createSprite(const std::string & fragments) {
   boost::shared_ptr<Texture> spriteTexture = textureCache.loadTexture(fragments);
   
   BoundedSprite * node = new BoundedSprite;
   node->boundingArea = spriteTexture->getSize();
   node->boundingArea.origin = vec2::Zero;

   boost::shared_ptr<Sprite> newSprite = boost::shared_ptr<Sprite>(
      new Sprite(spriteTexture, node)
   );   
   node->sprite = newSprite;
   
   sprites.push_back(node);
   
   return newSprite;   
}

void Subsystem::addSpriteToScene(const boost::shared_ptr<Sprite> & sprite) {
   // BoundedSprite node;
   // node.sprite = sprite;
   // node.boundingArea = sprite->getSize();
   // sprites.push_back(node);   
}
