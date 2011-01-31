#ifndef GAME_BACKGROUND_H
#define GAME_BACKGROUND_H

#include <game/common/system.h>

class Background : public System {
public:
  void init(const class Portal &interfaces);
  void render();

private:
  class Graphics *graphics;
  class Texture *tex;
};

#endif // !GAME_BACKGROUND_H
