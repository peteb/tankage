#ifndef GAME_BACKGROUND_H
#define GAME_BACKGROUND_H

#include <game/system.h>

class Background : public System {
public:
  Background(class Portal &interfaces, SystemContext *ctx);
  
  void render();

private:
  class Graphics *graphics;
  class Texture *tex;
};

#endif // !GAME_BACKGROUND_H
