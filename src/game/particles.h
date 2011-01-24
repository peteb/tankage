#ifndef GAME_PARTICLES_H
#define GAME_PARTICLES_H

#include <game/system.h>

class Particles : public System {
public:
  void init(const class Portal &modules);
  void render();

private:
  class Graphics *gfx;
};

#endif // !GAME_PARTICLES_H
