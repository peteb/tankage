#ifndef GAME_CLIENT_TANK_RENDERER_H
#define GAME_CLIENT_TANK_RENDERER_H

#include <game/client/snapshot.h>
#include <game/server/tank.h>

class TankRenderer {
public:
  TankRenderer(class GameClient *client, class Portal &services);

  void render();  
  void addSnapshot(const Snapshot<Tank::State> &snapshot);
  
private:
  typedef Snapshot<Tank::State> TankSnapshot;
  
  void renderTank(const Tank::State &state);
  
  class GameClient *_client;
  class Graphics *_gfx;
  class Texture *_tank_base;
  class Texture *_tank_turret;
  
  TankSnapshot prev;
  TankSnapshot current;
};

#endif // !GAME_CLIENT_TANK_RENDERER_H
