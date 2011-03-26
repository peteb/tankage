#ifndef GAME_SERVER_TANK_H
#define GAME_SERVER_TANK_H

#include <game/server/entity.h>
#include <game/common/control.h>
#include <utils/vec.h>
#include <utils/algorithm.h>

inline double wrap(double value, double lower, double upper) { // FIXME: util.
  double distance = upper - lower;
  double times = floor((value - lower) / distance);
  return value - (times * distance);
}

class Tank : public Entity {
public:
  /* <--- begin shared state code ---> */
  struct State {
    int id;
    vec2 pos;
    vec2 lin_vel;
    float base_dir;
    float rot_vel;
    
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
  void recvInput(const Control::Input &input);
  
  const State &state() const;
  void assign(const State &state);
  int id() const {return _state.id; }
  
private:
  class GameServer *_gameserver;
  State _state;
  Control::Input _lastinput;
};



inline Tank::State lerp(const Tank::State &begin, 
                        const Tank::State &end, 
                        double amount) {
  Tank::State ret;
  ret.id = begin.id;
  ret.pos = ::lerp(begin.pos, end.pos, amount);
  float smallest_angle = wrap(end.base_dir - begin.base_dir, -180.0, 180.0);
  ret.base_dir = ::lerp(begin.base_dir, begin.base_dir + smallest_angle, amount);
  ret.lin_vel = end.lin_vel; // just snap derivates
  
  return ret;
}




#endif // !GAME_SERVER_TANK_H
