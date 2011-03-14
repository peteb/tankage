#ifndef GAME_COMMON_TANK_H
#define GAME_COMMON_TANK_H

#include <game/common/net_protocol.h>

#include <utils/vec.h>
#include <vector>
#include <utils/algorithm.h>

typedef int ActorId;



/*
 * C: Control::update, creates a new move with time T1
 * -- some time later --
 * C: Control::onTick, sends input state from time T1
 * -- some time later --
 * Server receives input state T1, sends it to the tank
 * -- some time later --
 * Server updates the world
 * -- some time later --
 * Server gets snapshot from tank, sends it to player with T1
 * -- some time later --
 * Client receives snapshot for T1
  
  
 */

/*
 * Tanks can be updated using absolute data (snapshot)
 * Delta changes can be applied (input) for a certain amount of time (dt)
 */
class Tank {
public:
  struct Input {
    float time; // FIXME: probably overlapping with move::time in control
    uint8_t buttons;
    int aim_x;
    int aim_y;
  };

  struct State {
    State() {}
    State(const NetTankSnapshot &snapshot);
    operator NetTankSnapshot() const;

    /*   State operator +(const State &other) const {
      State ret;
      ret.actor = actor;
      ret.pos = pos + other.pos;
      ret.base_dir = base_dir + other.base_dir;

      return ret;
    }

    State operator -(const State &other) const {
      State ret;
      ret.actor = actor;
      ret.pos = pos - other.pos;
      ret.base_dir = base_dir - other.base_dir;

      return ret;
    }
    
    State operator *(double scalar) const {
      State ret;
      ret.actor = actor;
      ret.pos = pos * scalar;
      ret.base_dir = base_dir * scalar;

      return ret;
    }*/

    ActorId actor;
    vec2 pos;
    float base_dir;
    float turret_dir;
  };
  
  enum Buttons {
    STATE_MOVE_UP =    0x01,
    STATE_MOVE_DOWN =  0x02,
    STATE_TURN_LEFT =  0x04,
    STATE_TURN_RIGHT = 0x08,
    STATE_SHOOT =      0x10
  };

  Tank(ActorId id, const class SystemContext *ctx);
  ~Tank();

  void assign(const State &snapshot);
  State snapshot() const;

  void resetCount(double time = 0.0);
  double count() const;
  void render(class Graphics *graphics);
  bool advance(const Input &delta, double time);
  
  void takeDamage(const vec2 &pos, float damage);
  bool takeItem(const std::string &type, int amount);
  
  bool intersects(const vec2 &start, const vec2 &end, float radius, vec2 &hitpos) const;
  
  void setCursor(const vec2 & pos);
  void setTexture(class Texture *texture, class Texture *turret);
  
  const vec2 &position() const;
  const ActorId id() const;
  
private:
  ActorId _id;
  const class SystemContext *context;
  class Texture *texture, *_turret;
  vec2 _position;
  float _dir, _speed, _rotSpeed, _turretDir;

  float radius;
  vec2 vel, cursorPos;

  double _count;
  double secondsSinceFire;
  int health;
};

inline Tank::State lerp(const Tank::State &begin, const Tank::State &end, double scalar) {
  Tank::State ret;
  ret.actor = begin.actor;
  ret.pos = ::lerp(begin.pos, end.pos, scalar);
  ret.base_dir = ::lerp(begin.base_dir, end.base_dir, scalar);
  return ret;

}


#endif // !GAME_COMMON_TANK_H
