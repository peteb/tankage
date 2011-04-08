#ifndef GAME_SERVER_MAP_H
#define GAME_SERVER_MAP_H

#include <utility>

class Map {
public:
  Map(class GameServer *gameserver); // needed for spawning events
  void snap(class Packer &msg, class ClientSession *client);
  
  bool intersectSolid(const class vec2 &start, const class vec2 &end, float radius, vec2 &point, std::pair<int, int> &hit_tile);
  void damageTile(const std::pair<int, int> &tile);
  char at(int x, int y) const;
  
private:
  char _data[32 * 32];
  class GameServer *_gameserver;
};

#endif // !GAME_SERVER_MAP_H
