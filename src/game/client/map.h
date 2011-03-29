#ifndef GAME_CLIENT_MAP_H
#define GAME_CLIENT_MAP_H

class ClientMap {
public:
  void addChunk(class Unpacker &msg);
};

#endif // !GAME_CLIENT_MAP_H
