#ifndef ENGINE_OPENGL_GRAPHICS_H
#define ENGINE_OPENGL_GRAPHICS_H

#include <engine/graphics.h>
#include <utils/singleton.h>

namespace OpenGl {
class Graphics : public ::Graphics, public Singleton<Graphics> {
public:
  void clear(const color4 &clearColor);
  
};

}

#endif // !ENGINE_OPENGL_GRAPHICS_H
