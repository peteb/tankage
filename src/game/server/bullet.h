#ifndef GAME_SERVER_BULLET_H
#define GAME_SERVER_BULLET_H

#include <game/server/entity.h>
#include <utils/vec.h>

// IDEA: Entity<State>

class Bullet : public Entity {
public:
  /* <--- begin shared state code ---> */
  struct State {
    int id;
    vec2 start_pos;
    int start_tick;
    float dir;
    
    vec2 positionAt(double time_ofs) const;
    
    State &write(class Packer &msg);
    State &read(class Unpacker &msg);
  };
  /* <--- end shared state code ---> */
  
  Bullet(class GameServer *gameserver, int shooter);
  
  void snap(class Packer &msg, const class ClientSession *client);
  void tick();
  
  const State &state() const {return _state; }
  void assign(const State &state) {_state = state; }
  int id() const {return _state.id; }
  
private:
  State _state;
  
  class GameServer *_gameserver;
  int _shooter;  
  double _alive_time;
};

#endif // !GAME_SERVER_BULLET_H
