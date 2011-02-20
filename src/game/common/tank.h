#ifndef GAME_COMMON_TANK_H
#define GAME_COMMON_TANK_H

#include <game/common/net_protocol.h>

#include <utils/vec.h>

class Snail {
public:
  enum SnailState {
    STATE_MOVE_UP,
    STATE_MOVE_DOWN,
    STATE_TURN_LEFT,
    STATE_TURN_RIGHT,
    STATE_TURN_TURRET_LEFT,
    STATE_TURN_TURRET_RIGHT,
    STATE_SHOOT,
    STATE_MAX
  };

  Snail(const vec2 &initialPos, int id, const class SystemContext *ctx);
  ~Snail();
  
  void startState(SnailState state);
  void stopState(SnailState state);
  void setTexture(class Texture *texture, class Texture *turret);
  
  void render(class Graphics *graphics);
  bool update(double dt);
  void takeDamage(const vec2 &pos, float damage);
  bool takeItem(const std::string &type, int amount);
  
  bool intersects(const vec2 &start, const vec2 &end, float radius, vec2 &hitpos);
  void onSnap(const NetTankSnapshot &snapshot);
  NetTankSnapshot snapshot() const;
  
  void setCursor(const vec2 & pos);
  
  const vec2 &position() const;
  
private:
  class Texture *texture, *_turret;
  vec2 _position, originalPos;
  float _dir, _speed, _rotSpeed, _turretDir;
  NetTankSnapshot snapshots[2];
  double sinceSnap;
  
  int id;
  const class SystemContext *context;
  float radius;
  vec2 vel, cursorPos;
  bool _state[STATE_MAX];

  double secondsSinceFire;
  bool takingControl;
  int health;
};




#endif // !GAME_COMMON_TANK_H
