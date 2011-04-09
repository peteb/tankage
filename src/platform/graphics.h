#ifndef PLATFORM_GRAPHICS_H
#define PLATFORM_GRAPHICS_H

#include <platform/interface.h>
#include <utils/vec.h>
#include <utils/color.h>
#include <utils/rect.h>
#include <vector>

class Graphics : public Interface {
public:
  static std::string id() {
    return "graphics";
  }

  enum BlendMode {
    BLEND_NONE = 0,
    BLEND_ALPHA,
    BLEND_ADDITIVE
  };
  
  virtual void clear(const class color4 &clearColor) =0;
  virtual class Texture *createTexture(class Image *image) =0;
  virtual void drawQuad(const class rect &quad, float dir) =0;
  virtual void drawQuad(const class rect &quad, const class rect &source) =0;
  virtual void drawQuads(const std::vector<rect> &quads) =0;
  virtual void drawQuads(unsigned components, unsigned vertices, float *coord, float *tc) =0;
  virtual void drawLinestrip(const std::vector<vec2> &lines) =0;
  
  virtual void drawCircle(const class vec2 &pos, float radius, float tess) =0;
  virtual void setTransform(const class vec2 &translation) =0;
  virtual void setOrtho(const class rect &size) =0;
  virtual void setViewport(const class rect &size) =0;
  virtual void setTexture(class Texture *texture) =0;
  virtual void enableTextures() =0;
  virtual void disableTextures() =0;
  virtual void setBlend(BlendMode mode) =0;
  virtual void setColor(const color4 &col) =0;
};

#endif // !PLATFORM_GRAPHICS_H
