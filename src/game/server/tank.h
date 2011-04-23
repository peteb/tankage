#ifndef GAME_SERVER_TANK_H
#define GAME_SERVER_TANK_H

#include <game/server/entity.h>
#include <game/common/control.h>
#include <utils/vec.h>
#include <utils/algorithm.h>
#include <vector>
#include <map>

class Tank : public Entity {
public:
  /* <--- begin shared state code ---> */
  struct State {
    int id;
    vec2 pos;
    vec2 lin_vel;
    float base_dir;
    float rot_vel;
    float turret_dir;
    
    void advance(const Control::Input &input, double duration);

    State &write(class Packer &msg);
    State &read(class Unpacker &msg);
    
  private:
    void integrate(const Control::Input &input, double dt);
  };
  /* <--- end shared state code ---> */

  typedef std::map<char, char> Inventory;

  
  Tank(class GameServer *gameserver, class b2Body *body);
  
  void snap(class Packer &msg, const class ClientSession *client);
  void tick();
  void postTick();
  void recvInput(const Control::Input &input);
  void takeDamage(const vec2 &at, int amount);
  void takeItem(char type, int amount);
  vec2 position() const {return _state.pos; }
  
  const State &state() const;
  void assign(const State &state);
  int id() const {return _state.id; }
  const Inventory &inventory() const {return _inventory; }
  
private:
  void shoot();
  
  double _reload_time;
  int _health;
  
  class GameServer *_gameserver;
  class b2Body *_body;
  State _state;
  Inventory _inventory;
  std::vector<Control::Input> _lastinput;
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
  
  smallest_angle = wrap(end.turret_dir - begin.turret_dir, -180.0, 180.0);
  ret.turret_dir = ::lerp(begin.turret_dir, begin.turret_dir + smallest_angle, amount);
  
  return ret;
}




#endif // !GAME_SERVER_TANK_H
