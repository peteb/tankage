#ifndef GAME_TEXTURE_LOADER_H
#define GAME_TEXTURE_LOADER_H

#include <game/common/system.h>
#include <string>

class TextureLoader : public System {
public:
  static SystemContext::SystemId id() {
    return SystemContext::SYSTEM_TEXTURE_LOADER;
  }
  
  void init(const class Portal &interfaces);
  class Texture *texture(const std::string &filename);

private:
  class ImageLoader *imgLoader;
  class Graphics *gfx;
};

#endif // !GAME_TEXTURE_LOADER_H
