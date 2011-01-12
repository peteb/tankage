#ifndef GAME_ITEMS_H
#define GAME_ITEMS_H

#include <game/system.h>
#include <utils/vec.h>
#include <vector>

class Cactus {
public:
  Cactus(const vec2 &pos, class Texture *tex);
  
  void render(class Graphics *gfx);
  bool update(double dt);
  
private:
  class Texture *tex;
  vec2 pos;
};


class Projectile {
public:
  Projectile(const vec2 &pos, const vec2 &vel, class Snail *shooter, class Texture *tex, const SystemContext *ctx);

  bool update(double dt);
  void render(class Graphics *gfx);

private:
  class Texture *tex;
  const SystemContext *ctx;
  vec2 pos;
  vec2 vel;
  class Snail *shooter;
};


class Items : public System {
public:
  enum ProjectileType {
    PROJECTILE_BULLET
  };
  
  
  Items(const class Portal &interfaces, SystemContext *ctx);

  void update();
  void render();
  void spawnProjectile(ProjectileType type,
                       const vec2 &pos,
                       const vec2 &dir,
                       class Snail *shooter);
  
private:
  class WindowManager *wm;
  class Graphics *gfx;
  class Texture *cactusTexture;
  class Texture *bulletTexture;
  
  double lastGentime;
  double lastUpdate;
  
  typedef std::vector<Cactus *> CactusVector;
  typedef std::vector<Projectile *> ProjectileVector;
  
  CactusVector cactii;
  ProjectileVector projectiles;
};

#endif // !GAME_ITEMS_H
