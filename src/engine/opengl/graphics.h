#ifndef ENGINE_OPENGL_GRAPHICS_H
#define ENGINE_OPENGL_GRAPHICS_H

#include <engine/graphics.h>
#include <utils/singleton.h>

namespace OpenGl {
class Graphics : public ::Graphics, public Singleton<Graphics> {
public:
  class Texture *createTexture(class Image *image);

  void clear(const class color4 &clearColor);
  void drawQuad(const class rect &quad);

  void setOrtho(const class rect &size);
  void setViewport(const class rect &size);

  void enableTextures();
  void disableTextures();
};

}

#endif // !ENGINE_OPENGL_GRAPHICS_H
