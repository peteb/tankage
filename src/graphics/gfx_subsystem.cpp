/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "graphics/gfx_subsystem.h"
#include "graphics/sprite.h"

#include <OpenGL/OpenGL.h>

using Graphics::Subsystem;
 
Subsystem::Subsystem() {
   Sprite * s1 = new Sprite;
   s1->x = 0;
   s1->y = 0;
   
   sprites.push_back(s1);
}

void Subsystem::render(float dt) {
   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   
   const float scaleX = 1.0f / 400.0f;
   const float scaleY = 1.0f / 300.0f;
   glTranslatef(-1.0f, 1.0f, 0.0f);
   glScalef(scaleX, -scaleY, 1.0f);
   
   glMatrixMode(GL_MODELVIEW);
   
   for (int i = 0; i < sprites.size(); ++i) {
      const Sprite * sprite = sprites[i];
      sprites[i]->y += 0.01f;
      sprites[i]->x += 0.01f;
      glLoadIdentity();
      glTranslatef(sprite->x, sprite->y, 0.0f);

      glBegin(GL_QUADS);

      glColor3f(1.0f, 0.0f, 0.0f);
      
      const float halfWidth = 32.0f / 2.0f;
      const float halfHeight = 32.0f / 2.0f;

      glVertex2f(-halfWidth, -halfHeight);
      glVertex2f( halfWidth, -halfHeight);
      glVertex2f( halfWidth,  halfHeight);
      glVertex2f(-halfWidth,  halfHeight);
      glEnd();
   }
}

