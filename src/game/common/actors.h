#ifndef GAME_ACTORS_H
#define GAME_ACTORS_H

#include <game/common/system.h>
#include <game/common/replicated_system.h>
#include <game/common/tank.h>

#include <utils/vec.h>
#include <vector>

class Actors : public ReplicatedSystem {
public:
  static SystemContext::SystemId id() {
    return SystemContext::SYSTEM_ACTORS;
  }
  
  Actors();
  ~Actors();

  // System stuff
  void init(const class Portal &interfaces);
  void start();
  void render();

  // Replicated system
  void onTick(class Client *client);
  void onReceive(NetPacketType type, const class Packet &packet);
  
  // Misc  
  Tank *tank(ActorId id) const;
  Tank *intersectingTank(const vec2 &start, const vec2 &end, float radius, ActorId ignore, vec2 &hitpos);
  Tank *createActor();
  void remove(ActorId id);
  
private:
  typedef std::vector<Tank *> TankVector;

  Tank *createTank(const NetTankSnapshot &net_snapshot);
  TankState rebaseHistory(double time, const TankState &newState, Tank *tank);
  
  double gameTime;
  class Graphics *graphics;
  class WindowManager *wm;
  class Texture *tankBase;
  class Texture *tankTurret;

  TankVector tanks;
  double lastUpdate;
  ActorId lastId;
};

#endif // !GAME_ACTORS_H
