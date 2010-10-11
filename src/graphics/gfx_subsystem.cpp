/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "graphics/gfx_subsystem.h"
#include "graphics/sprite.h"
#include "graphics/vertex.h"
#include <OpenGL/OpenGL.h>
#include <boost/make_shared.hpp>

using namespace Graphics;
 
Subsystem::Subsystem() 
   : viewport(vec2::Zero)
{
}


void Subsystem::render(float dt) {
   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT);

   glViewport(0, 0, int(viewport.x), int(viewport.y));
   
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   
   const float scaleX = 2.0f / viewport.x;
   const float scaleY = 2.0f / viewport.y;
   glTranslatef(-1.0f, 1.0f, 0.0f);
   glScalef(scaleX, -scaleY, 1.0f);
   
   glMatrixMode(GL_TEXTURE);
   glLoadIdentity();

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   
   glEnable(GL_TEXTURE_2D);
   
   for (unsigned i = 0; i < sprites.size(); ++i) {
      if (boost::shared_ptr<Sprite> sprite = sprites[i].lock()) {
         glBegin(GL_QUADS);
         // glColor3f(1.0f, 0.0f, 0.0f);

         std::vector<Vertex2T2> vertices = sprite->constructVertices();
         for (unsigned i = 0; i < vertices.size(); ++i) {
            const vec2 pos = vertices[i].pos + sprite->getPosition();
            glTexCoord2f(vertices[i].tc.x, vertices[i].tc.y);
            glVertex2f(pos.x, pos.y);
         }
      
         glEnd();
      }
      else {
         sprites.erase(sprites.begin() + i);
      }
   }
}

void Subsystem::resizeViewport(const vec2 & size) {
   viewport = size;
}

boost::shared_ptr<Sprite> Subsystem::createSprite(const std::string & fragments) {
   boost::shared_ptr<Texture> spriteTexture = textureCache.loadTexture(fragments);
   boost::shared_ptr<Sprite> newSprite = boost::make_shared<Sprite>(spriteTexture);   
   
   return newSprite;   
}

void Subsystem::addSpriteToScene(const boost::shared_ptr<Sprite> & sprite) {
   sprites.push_back(sprite);   
}
