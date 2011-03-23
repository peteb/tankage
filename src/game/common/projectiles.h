#ifndef GAME_ITEMS_H
#define GAME_ITEMS_H

#include <game/common/replicated_system.h>
#include <game/server/actors.h>

#include <utils/vec.h>
#include <vector>

typedef uint32_t ProjectileId;

class Projectiles : public ReplicatedSystem {
public:
  static SystemContext::SystemId id() {
    return SystemContext::SYSTEM_PROJECTILES;
  }
  
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
                       float dir,
                       ActorId shooterId);

  void onTick();
  void onReceive(NetPacketType type, const class Packet &packet);
  
private:  
  typedef std::vector<class Projectile *> ProjectileVector;

  class WindowManager *wm;
  class Graphics *gfx;
  class Texture *bulletTexture;
  class Texture *smoke;
  
  ProjectileVector projectiles;
  double lastUpdate;
  uint32_t projectileId;

  Projectile *projectile(ProjectileId id) const; 
  Projectile *createProjectile(ProjectileId id);
};

#endif // !GAME_ITEMS_H
