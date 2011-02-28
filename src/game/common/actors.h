#ifndef GAME_ACTORS_H
#define GAME_ACTORS_H

#include <game/common/system.h>
#include <game/common/replicated_system.h>
#include <game/common/tank.h>

#include <utils/vec.h>
#include <vector>

class Actors : public ReplicatedSystem {
public:
  Actors();
  ~Actors();

  // System stuff
  void init(const class Portal &interfaces);
  void render();

  // Replicated system
  void onTick(class Client *client);
  void onReceive(NetPacketType type, const class Packet &packet);
  
  // Misc  
  Tank *tank(ActorId id) const;
  Tank *intersectingTank(const vec2 &start, const vec2 &end, float radius, ActorId ignore, vec2 &hitpos);
  Tank *createActor();
  
private:
  typedef std::vector<Tank *> TankVector;

  void createTank(const NetTankSnapshot &net_snapshot);
  
  class Graphics *graphics;
  class WindowManager *wm;
  class Texture *tankBase;
  class Texture *tankTurret;
  
  TankVector tanks;
  double lastUpdate;
  ActorId lastId;
};

#endif // !GAME_ACTORS_H
