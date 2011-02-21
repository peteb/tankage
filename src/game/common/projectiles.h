#ifndef GAME_ITEMS_H
#define GAME_ITEMS_H

#include <game/common/replicated_system.h>
#include <game/common/actors.h>

#include <utils/vec.h>
#include <vector>

class Projectiles : public ReplicatedSystem {
public:
  enum ProjectileType {
    PROJECTILE_BULLET
  };

  Projectiles();
  ~Projectiles();
  void init(const class Portal &interfaces);

  void update();
  void render();
  void spawnProjectile(ProjectileType type,
                       const vec2 &pos,
                       const vec2 &dir,
                       int shooterId);

  void onTick(class Client *client);
  void onReceive(NetPacketType type, const class Packet &packet);
  
private:  
  typedef std::vector<class Projectile *> ProjectileVector;

  class WindowManager *wm;
  class Graphics *gfx;
  class Texture *bulletTexture;
  class Texture *smoke;
  
  ProjectileVector projectiles;
  double lastUpdate;
  int projectileId;
};

#endif // !GAME_ITEMS_H
