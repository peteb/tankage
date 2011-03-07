#ifndef GAME_PROJECTILE_H
#define GAME_PROJECTILE_H

#include <utils/vec.h>
#include <game/common/net_protocol.h>
#include <game/common/actors.h>
#include <stdint.h>

class Projectile {
public:
  Projectile(const class SystemContext *ctx, uint32_t _id);

  uint32_t id() const {return _id; }
  bool update(double dt);
  void render(class Graphics *gfx);
  
  void setVelocity(const vec2 &vel);
  void setPosition(const vec2 &pos);
  void setShooter(ActorId id) {shooterId = id; }
  void setTexture(class Texture *tex) {this->tex = tex; }
  
  NetProjectileSnapshot snapshot() const;
  void onSnap(const NetProjectileSnapshot &snapshot);
  
private:
  class Texture *tex;
  const class SystemContext *ctx;
  
  vec2 position;
  vec2 velocity;
  ActorId shooterId;
  uint32_t _id;
  bool snapshotted;
};


#endif // !GAME_PROJECTILE_H

