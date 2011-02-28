#ifndef GAME_COMMON_TANK_H
#define GAME_COMMON_TANK_H

#include <game/common/net_protocol.h>

#include <utils/vec.h>
#include <vector>

typedef int ActorId;

/*
 * Tanks can be updated using absolute data (snapshot)
 * Delta changes can be applied (input) for a certain amount of time (dt)
 */
class Tank {
public:
  struct Input {
    uint8_t buttons;
    int aim_x;
    int aim_y;
  };

  struct State {
    State() {}
    State(const NetTankSnapshot &snapshot);
    operator NetTankSnapshot() const;
    
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

  double secondsSinceFire;
  int health;
  bool _snapshotted;
};




#endif // !GAME_COMMON_TANK_H
