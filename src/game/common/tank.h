#ifndef GAME_COMMON_TANK_H
#define GAME_COMMON_TANK_H

#include <game/common/net_protocol.h>

#include <utils/vec.h>
#include <vector>
#include <utils/algorithm.h>

typedef int ActorId;

// FIXME: this is pretty generic, and not really connected to tanks. It should
// probably be in Control.
struct PlayerInput {
  float time;
  uint8_t buttons;
  int aim_x;
  int aim_y;

  enum Buttons {
    STATE_MOVE_UP =    0x01,
    STATE_MOVE_DOWN =  0x02,
    STATE_TURN_LEFT =  0x04,
    STATE_TURN_RIGHT = 0x08,
    STATE_SHOOTING =   0x10
  };

};



// The server stores the last input from the client and updates the
// tank a couple of times per second using that data by advancing its
// TankState with that input.
class TankState {
public:
  TankState() {}
  TankState(const NetTankSnapshot &snapshot);
  operator NetTankSnapshot() const;

  // run on both server and client
  void advance(const PlayerInput &delta, double duration);  // will call integrate multiple times
  
  ActorId actor;
  vec2 pos;
  float base_dir;
  float turret_dir;
  // radius might be needed if the TankState should check for
  // collisions. then we also need to know more stuff, like context.
  // I don't like bloating this class that much.
  // But it will all fall under "advancing the state following the
  // world's rules"
  
private:
  void integrate(const PlayerInput &delta, double dt);
};

// Creating dust after tank when driving can be done in update:
// 1: check TankState velocity, no dust when standing still
// 2: calculate offsets for tracks using base_dir
// 3: if (now - lastSpawnTime > some_value), spawn new particle at
//    those offsets (context->particles())

// if (context->isServer() || context->isClient())

// Tanks need a synchronized clock (the same for same actor,
// extrapolated on the client to match the server)
// but also a local clock (probably run by update)
class Tank {
public:
  Tank(const class SystemContext *ctx);
  
  void render(class Graphics *graphics);
  void update(double dt); // render, create projectiles, crush
                          // opponents, manipulate TankState, etc.

  ActorId id() const {return _state.actor; }
  const TankState &state() const;

  // assign a new state to the tank. the tank can take actions based on this;
  // start shooting and stuff like that.
  // will not be called during rewind/replay etc.
  void assign(const TankState &new_state);

  //void takeDamage(const vec2 &pos, float damage);
  //bool takeItem(const std::string &type, int amount);
  void resetCount(double time = 0.0);
  double count() const;

  bool intersects(const vec2 &start, const vec2 &end, float radius, vec2 &hitpos) const;
  //void loadTextures(class TextureLoader *loader);
  
  void setTexture(class Texture *texture, class Texture *turret);
private:
  double _count;
  TankState _state;
  const class SystemContext *context;
  class Texture *texture, *_turret;
};


// * The state should be decoupled from logic, so state types can be
//   shared
// * Something steps a state and adds collision response
// * Something should render the current state (trigger events on
//   clientside)
// * Something should trigger events on the server (create
//   projectiles)
//
// - Two major responsibilities: advancing state, triggering events
//   They need to be totally decoupled. Advancing state shouldn't
//   cause sideeffects.
//   Also, a synchronized clock will be good for animations


inline TankState lerp(const TankState &begin, const TankState &end, double scalar) {
  TankState ret;
  ret.actor = begin.actor;
  ret.pos = ::lerp(begin.pos, end.pos, scalar);
  ret.base_dir = ::lerp(begin.base_dir, end.base_dir, scalar);
  ret.turret_dir = ::lerp(begin.turret_dir, end.turret_dir, scalar);
  
  return ret;

}


#endif // !GAME_COMMON_TANK_H
