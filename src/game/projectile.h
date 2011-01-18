#ifndef GAME_PROJECTILE_H
#define GAME_PROJECTILE_H

#include <utils/vec.h>

class Projectile {
public:
  Projectile(const vec2 &pos, const vec2 &vel, class Snail *shooter,
             class Texture *tex, const class SystemContext *ctx);

  bool update(double dt);
  void render(class Graphics *gfx);

private:
  class Texture *tex;
  const class SystemContext *ctx;
  vec2 pos;
  vec2 vel;
  class Snail *shooter;
};


#endif // !GAME_PROJECTILE_H

