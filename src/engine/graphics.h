#ifndef ENGINE_GRAPHICS_H
#define ENGINE_GRAPHICS_H

#include <engine/interface.h>
#include <utils/color.h>

class Graphics : public Interface {
public:
  static std::string id() {
    return "graphics";
  }
  
  virtual void clear(const color4 &clearColor) =0;
};

#endif // !ENGINE_GRAPHICS_H
