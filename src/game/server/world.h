#ifndef GAME_SERVER_WORLD_H
#define GAME_SERVER_WORLD_H

#ifdef __linux__
#include "Box2D/Box2D.h"
#else
#include "Box2d/Box2d.h"
#endif

class World {
public:
  World(class GameServer *server);
    
  b2Body *createTankBody();
  
  void tick();
  
private:
  class GameServer *_server;
  b2World _phys_world;
};


#endif // !GAME_SERVER_WORLD_H
