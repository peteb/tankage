#ifndef GAME_PROJECTILES_H
#define GAME_PROJECTILES_H

#include <game/system.h>
#include <utils/vec.h>
#include <vector>

class Bullet {
public:
  Bullet(class Texture *tex, const vec2 &pos, const vec2 &vel, int shooterId);

  bool update(double dt);
  void render(class Graphics *gfx);
  
private:
  class Texture *tex;
  vec2 pos;
  vec2 vel;
  int shooterId;
};

class Projectiles : public System {
public:
  Projectiles(const class Portal &interfaces, SystemContext *ctx);

  void spawnBullet(const vec2 &pos, const vec2 &dir, int shooterId);
  void update();
  
private:
  class Graphics *gfx;
  class Texture *bulletTexture;
  class WindowManager *wm;
  
  typedef std::vector<Bullet *> BulletVector;
  BulletVector bullets;

  double lastUpdate;
};

#endif // !GAME_PROJECTILES_H
