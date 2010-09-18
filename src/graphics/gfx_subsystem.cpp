/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "graphics/gfx_subsystem.h"
#include "graphics/sprite.h"

#include <OpenGL/OpenGL.h>

using Graphics::Subsystem;
 
Subsystem::Subsystem() 
   : viewport(vec2::Zero)
{
   Sprite * s1 = new Sprite;
   s1->position = vec2(0.0f, 0.0f);
   
   sprites.push_back(s1);
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
   
   glMatrixMode(GL_MODELVIEW);
   
   for (unsigned i = 0; i < sprites.size(); ++i) {
      const Sprite * sprite = sprites[i];
      sprites[i]->position += vec2(0.01f, 0.1f);
      
      glBegin(GL_QUADS);
      glColor3f(1.0f, 0.0f, 0.0f);

      std::vector<vec2> vertices = sprite->constructVertices();
      for (unsigned i = 0; i < vertices.size(); ++i) {
         const vec2 pos = vertices[i] + sprite->position;
         glVertex2f(pos.x, pos.y);
      }
         
      glEnd();
   }
}

void Subsystem::resizeViewport(const vec2 & size) {
   viewport = size;
}
