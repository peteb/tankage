#ifndef GAME_SERVER_MAP_H
#define GAME_SERVER_MAP_H

#include <utility>

typedef std::pair<int, int> TileCoord;

class Map {
public:
  Map(class GameServer *gameserver, class b2World &world); 
  // needed for spawning events
  void snap(class Packer &msg, class ClientSession *client);
  
  bool intersectSolid(const class vec2 &start, const class vec2 &end, float radius, vec2 &point, TileCoord &hit_tile);
  void damageTile(const TileCoord &tile);
  char at(const TileCoord &tile) const;
  char &at(const TileCoord &tile);
  
private:
  char _data[32 * 32];
  class GameServer *_gameserver;
  
  class b2World &_world;
};

#endif // !GAME_SERVER_MAP_H
