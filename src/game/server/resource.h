#ifndef GAME_SERVER_RESOURCE_H
#define GAME_SERVER_RESOURCE_H

#include <game/server/entity.h>
#include <utils/vec.h>

class Resource : public Entity {
public:
  /* <--- begin shared state code ---> */
  struct State {
    int id;
    vec2 pos;
    unsigned int start_tick;
    char type;
    
    State &write(class Packer &msg);
    State &read(class Unpacker &msg);
  };
  /* <--- end shared state code ---> */
  
  Resource(class GameServer *gameserver);
  
  void snap(class Packer &msg, const class ClientSession *client);
  void tick();
  
  const State &state() const {return _state; }
  void assign(const State &state) {_state = state; }
  int id() const {return _state.id; }
  
private:
  State _state;
  
  class GameServer *_gameserver;
};

#endif // !GAME_SERVER_RESOURCE_H
