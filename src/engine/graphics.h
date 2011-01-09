#ifndef ENGINE_GRAPHICS_H
#define ENGINE_GRAPHICS_H

#include <engine/interface.h>

class Graphics : public Interface {
public:
  static std::string id() {
    return "graphics";
  }
  
  virtual void clear(const class color4 &clearColor) =0;
  virtual class Texture *createTexture(class Image *image) =0;
  virtual void drawQuad(const class rect &quad) =0;
  virtual void setOrtho(const class rect &size) =0;
  virtual void setViewport(const class rect &size) =0;
  virtual void enableTextures() =0;
  virtual void disableTextures() =0;

};

#endif // !ENGINE_GRAPHICS_H
