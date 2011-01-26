#ifndef GAME_PARTICLES_H
#define GAME_PARTICLES_H

#include <game/system.h>
#include <utils/vec.h>
#include <vector>

class ParticleGroup {
public:
  ParticleGroup(class Texture *texture);

  void render(double dt, class Graphics *gfx);
  
private:
  class Texture *texture;
};

class Particles : public System {
public:
  void init(const class Portal &modules);
  void render();
  ParticleGroup *group(class Texture *texture);
  
private:
  typedef std::vector<std::pair<Texture *, ParticleGroup *> > GroupList;
  
  class Graphics *gfx;
  class WindowManager *wm;
  GroupList groups;
  double lastUpdate;
};

#endif // !GAME_PARTICLES_H
