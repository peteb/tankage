#include <engine/opengl/graphics.h>
#include <engine/opengl/opengl.h>

void OpenGl::Graphics::clear(const color4 &clearColor) {
  glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
  glClear(GL_COLOR_BUFFER_BIT);
}
