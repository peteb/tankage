#ifndef GAME_PARTICLES_H
#define GAME_PARTICLES_H

#include <game/system.h>

class ParticleGroup {
public:
  
};

class Particles : public System {
public:
  void init(const class Portal &modules);
  void render();
  ParticleGroup *group(class Texture *texture);
  
private:
  class Graphics *gfx;
};

#endif // !GAME_PARTICLES_H
