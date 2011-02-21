#ifndef GAME_COMMON_TANK_H
#define GAME_COMMON_TANK_H

#include <game/common/net_protocol.h>
#include <game/common/actors.h>

#include <utils/vec.h>

class Tank {
public:
  enum State {
    STATE_MOVE_UP = 0,
    STATE_MOVE_DOWN,
    STATE_TURN_LEFT,
    STATE_TURN_RIGHT,
    STATE_SHOOT,
    STATE_MAX
  };

  Tank(ActorId id, const class SystemContext *ctx);
  ~Tank();

  void onSnap(const NetTankSnapshot &snapshot);
  NetTankSnapshot snapshot() const;
  
  void startState(State state);
  void stopState(State state);
  void setTexture(class Texture *texture, class Texture *turret);
  
  void render(class Graphics *graphics);
  bool update(double dt);
  
  void takeDamage(const vec2 &pos, float damage);
  bool takeItem(const std::string &type, int amount);
  
  bool intersects(const vec2 &start, const vec2 &end, float radius, vec2 &hitpos) const;
  
  void setCursor(const vec2 & pos);
  
  const vec2 &position() const;
  const ActorId id() const;
  
private:
  class Texture *texture, *_turret;
  vec2 _position;
  float _dir, _speed, _rotSpeed, _turretDir;
  NetTankSnapshot snapshots[2];
  double sinceSnap;
  
  const class SystemContext *context;
  float radius;
  vec2 vel, cursorPos;
  bool _state[STATE_MAX];

  double secondsSinceFire;
  int health;
  ActorId _id;
  bool _snapshotted;
};




#endif // !GAME_COMMON_TANK_H
