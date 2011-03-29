#ifndef GAME_SERVER_MAP_H
#define GAME_SERVER_MAP_H

class Map {
public:
  Map();
  void snap(class Packer &msg, class ClientSession *client);
  
  bool intersectSolid(const class vec2 &start, const class vec2 &end, float radius, vec2 &point);
  
private:
  char _data[32 * 32];
};

#endif // !GAME_SERVER_MAP_H
