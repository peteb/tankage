#ifndef GAME_SERVER_TANK_H
#define GAME_SERVER_TANK_H

#include <game/server/entity.h>
#include <game/common/control.h>
#include <utils/vec.h>
#include <utils/algorithm.h>

class Tank : public Entity {
public:
  /* <--- begin shared state code ---> */
  struct State {
    int id;
    vec2 pos;
    float base_dir;

    void advance(const Control::Input &input, double duration);

    State &write(class Packer &msg);
    State &read(class Unpacker &msg);
    
  private:
    void integrate(const Control::Input &input, double dt);
  };
  /* <--- end shared state code ---> */
  
  Tank(class GameServer *gameserver);
  
  void snap(class Packer &msg, const class ClientSession *client);
  void tick();
  
  const State &state() const;
  void assign(const State &state);
  
private:
  class GameServer *_gameserver;
  State _state;
};



inline Tank::State lerp(const Tank::State &begin, 
                        const Tank::State &end, 
                        float amount) {
  Tank::State ret;
  ret.id = begin.id;
  ret.pos = ::lerp(begin.pos, end.pos, amount);
  ret.base_dir = ::lerp(begin.base_dir, end.base_dir, amount);

  return ret;
}




#endif // !GAME_SERVER_TANK_H
