#ifndef GAME_CLIENT_TANK_RENDERER_H
#define GAME_CLIENT_TANK_RENDERER_H

#include <game/client/snapshot.h>
#include <game/server/tank.h>

class TankRenderer {
public:
  TankRenderer(class GameClient *client);
  void render();
  
  void init(const class Portal &interfaces, const class SystemContext *context);
  void addSnapshot(const Snapshot<Tank::State> &snapshot);
  
private:
  typedef Snapshot<Tank::State> TankSnapshot;
  
  void renderTank(const Tank::State &state);
  
  class GameClient *_client;
  const class SystemContext *_context;
  class Graphics *_gfx;

  TankSnapshot prev;
  TankSnapshot current;
};

#endif // !GAME_CLIENT_TANK_RENDERER_H
