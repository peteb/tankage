#ifndef GAME_PROJECTILE_H
#define GAME_PROJECTILE_H

#include <utils/vec.h>

class Projectile {
public:
  Projectile(class ParticleGroup *partGroup, class Snail *shooter,
             class Texture *tex, const class SystemContext *ctx, const vec2 &pos);

  bool update(double dt);
  void render(class Graphics *gfx);
  void setVel(const vec2 &vel);
  
private:
  class ParticleGroup *partGroup;
  class Snail *shooter;
  class Texture *tex;
  const class SystemContext *ctx;
  
  vec2 pos;
  vec2 vel;
  double sinceEmit;
};


#endif // !GAME_PROJECTILE_H

