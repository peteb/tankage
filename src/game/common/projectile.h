#ifndef GAME_PROJECTILE_H
#define GAME_PROJECTILE_H

#include <utils/vec.h>
#include <game/common/net_protocol.h>

class Projectile {
public:
  Projectile(class ParticleGroup *partGroup, int shooterId,
             class Texture *tex, const class SystemContext *ctx, const vec2 &pos,
             int id);

  bool update(double dt);
  void render(class Graphics *gfx);
  void setVel(const vec2 &vel);
  NetProjectileSnapshot snapshot() const;
  
private:
  class ParticleGroup *partGroup;
  class Texture *tex;
  const class SystemContext *ctx;
  
  vec2 pos;
  vec2 vel;
  double sinceEmit;
  int shooterId;
  int _id;
};


#endif // !GAME_PROJECTILE_H

