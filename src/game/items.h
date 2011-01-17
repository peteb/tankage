#ifndef GAME_ITEMS_H
#define GAME_ITEMS_H

#include <game/system.h>
#include <utils/vec.h>
#include <vector>

class Item {
public:
  virtual ~Item() {}

  Item(const vec2 &pos, float radius);
  
  virtual void render(class Graphics *gfx) =0;
  virtual bool update(double dt) {}
  bool intersects(const vec2 &start, const vec2 &end, float radius, vec2 &hitpos);
  virtual bool takeDamage(const vec2 &pos, float damage) =0;
  
protected:
  vec2 pos;
  float radius;
};

class Powerup : public Item {
public:
  Powerup(const vec2 &pos, class Texture *tex, const std::string &type, int amount);

  void render(class Graphics *gfx);
  bool update(double dt);
  bool takeDamage(const vec2 &pos, float damage);

private:
  class Texture *tex;
  std::string type;
  int amount;
  int taken;
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
  Item *intersectingItem(const vec2 &start, const vec2 &end, float radius, vec2 &hitpos);
  
private:
  class WindowManager *wm;
  class Graphics *gfx;
  class Texture *cactusTexture;
  class Texture *bulletTexture;
  class Texture *healthPowerup;
  
  double lastGentime;
  double lastUpdate;
  
  typedef std::vector<Item *> ItemVector;
  typedef std::vector<Projectile *> ProjectileVector;
  
  ItemVector items;
  ProjectileVector projectiles;
};

#endif // !GAME_ITEMS_H
