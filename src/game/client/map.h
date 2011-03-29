#ifndef GAME_CLIENT_MAP_H
#define GAME_CLIENT_MAP_H

class ClientMap {
public:
  ClientMap(class Portal &services);
  
  void addChunk(class Unpacker &msg);
  void render();
  
private:
  char _data[32*32];
  
  class Graphics *_gfx;
};

#endif // !GAME_CLIENT_MAP_H
