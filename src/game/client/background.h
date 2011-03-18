#ifndef GAME_BACKGROUND_H
#define GAME_BACKGROUND_H

#include <game/common/system.h>

class Background : public System {
public:
  static SystemContext::SystemId id() {
    return SystemContext::SYSTEM_BACKGROUND;
  }
  
  void init(const class Portal &interfaces);
  void render();

private:
  class Graphics *graphics;
  class Texture *tex;
};

#endif // !GAME_BACKGROUND_H
