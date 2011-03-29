#ifndef GAME_SERVER_MAP_H
#define GAME_SERVER_MAP_H

class Map {
public:
  Map();
  void snap(class Packer &msg, class ClientSession *client);
  
private:
  char _data[32 * 32];
};

#endif // !GAME_SERVER_MAP_H
