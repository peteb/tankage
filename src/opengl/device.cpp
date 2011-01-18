/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "device.h"
#include "texture.h"
#include <utils/rect.h>
#include "opengl.h"
#include "render_context.h"
#include "graphics/vertex.h"
#include "graphics/mesh.h"
#include "graphics/color.h"

OpenGL::RenderContext *OpenGL::Device::createContext() {
  return new OpenGL::RenderContext;
}


OpenGL::Texture *OpenGL::Device::createTexture(const void * data, int width, int height, int bpp, int format) {
   GLuint newTexture;
   glGenTextures(1, &newTexture);
   glBindTexture(GL_TEXTURE_2D, newTexture);
   glTexImage2D(
      GL_TEXTURE_2D, 0, bpp, width, height, 0, format,
      GL_UNSIGNED_BYTE, data
   );
   
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   
   return new OpenGL::Texture(newTexture, rect(width, height));
}


void OpenGL::Device::drawMesh(const Ref<Graphics::Mesh>::SharedPtr & mesh) {
  glBegin(GL_QUADS);
  for (size_t i = 0; i < mesh->vertices.size(); ++i) {
    const Vertex2T2 &vertex = mesh->vertices[i];
    
    glTexCoord2f(vertex.tc.x, vertex.tc.y);
    glColor4f(vertex.color.r, vertex.color.g, vertex.color.b, vertex.color.a);
    glVertex2f(vertex.pos.x, vertex.pos.y);
    
  }
  glEnd();
}


void OpenGL::Device::setOrtho(const rect & viewport) {
  vec2 min, max;
  viewport.getCoords(min, max);
  glViewport(int(min.x), int(min.y), int(max.x - min.x), int(max.y - min.y));
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  const float scaleX = 1.0f / viewport.halfSize.x;
  const float scaleY = 1.0f / viewport.halfSize.y;
  
  
  glTranslatef(-1.0f, 1.0f, 0.0f);
  glScalef(scaleX, -scaleY, 1.0f);
  
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void OpenGL::Device::clearColor(const Graphics::Color & color) {
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT);
}
