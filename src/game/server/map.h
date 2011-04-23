#ifndef GAME_SERVER_MAP_H
#define GAME_SERVER_MAP_H

#include <utility>
#include <vector>
#include "page.h"

typedef std::pair<int, int> TileCoord;

class Map {
public:
  Map(class GameServer *gameserver, class b2World *world); 
  // needed for spawning events
  void snap(class Packer &msg, class ClientSession *client);
  
  bool intersectSolid(const class vec2 &start, const class vec2 &end, vec2 &point, TileCoord &hit_tile);
  void damageTile(const TileCoord &tile, int amount);
  
private:
  class GameServer *_gameserver;
  class b2World *_world;

  Page _page;
};

#endif // !GAME_SERVER_MAP_H
