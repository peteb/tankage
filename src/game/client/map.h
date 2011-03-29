#ifndef GAME_CLIENT_MAP_H
#define GAME_CLIENT_MAP_H

#include <utility>

class ClientMap {
public:
  ClientMap(class Portal &services);
  
  void addChunk(class Unpacker &msg);
  void setTile(const std::pair<int, int> &tile, char state);
  void render();
  
private:
  char _data[32*32];
  
  class Graphics *_gfx;
};

#endif // !GAME_CLIENT_MAP_H
