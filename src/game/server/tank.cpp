#include "tank.h"
#include <utils/packer.h>
#include <game/server/gameserver.h>
#include <cmath>

/* <--- begin tank state ---> */
Tank::State &Tank::State::write(Packer &msg) {
  msg.writeInt(id);
  msg.writeShort(pos.x * 10.0f);
  msg.writeShort(pos.y * 10.0f);
  msg.writeShort(lin_vel.x * 100.0f);
  msg.writeShort(lin_vel.y * 100.0f);
  msg.writeShort(base_dir / 360.0f * 65536.0f);
  msg.writeShort(rot_vel / 360.0f * 65536.0f);
  msg.writeShort(turret_dir / 360.0f * 65536.0f);
  return *this;
}

Tank::State &Tank::State::read(class Unpacker &msg) {
  id = msg.readInt();
  pos.x = static_cast<float>(msg.readShort()) / 10.0f;
  pos.y = static_cast<float>(msg.readShort()) / 10.0f;
  lin_vel.x = static_cast<float>(msg.readShort()) / 100.0f;
  lin_vel.y = static_cast<float>(msg.readShort()) / 100.0f;
  base_dir = static_cast<float>(msg.readShort()) / 65536.0f * 360.0f;
  rot_vel = static_cast<float>(msg.readShort()) / 65536.0f * 360.0f;
  turret_dir = static_cast<float>(msg.readShort()) / 65536.0f * 360.0f;
  return *this;
}

void Tank::State::advance(const Control::Input &input, double duration) {
  const double DELTA_TIME = 1.0/40.0; // FIXME: this can be tweaked, a lot here can
  
  for (; duration >= DELTA_TIME; duration -= DELTA_TIME) {
    integrate(input, DELTA_TIME);
  }
  
  // integrate any leftovers
  integrate(input, duration);
}

void Tank::State::integrate(const Control::Input &input, double dt) {
  vec2 forward = vec2::FromDegrees(base_dir);
  
  // forward and backward
  if (input.buttons & Control::Input::FORWARD)
    lin_vel += forward * dt * 300.0;
  else if (input.buttons & Control::Input::BACKWARD)
    lin_vel += forward * dt * -100.0;
  else
    lin_vel *= 0.8;
  
  double mag = length(lin_vel);
  lin_vel = normalized(lin_vel);
  lin_vel *= clamp(mag, 0.0, 100.0); // maximum speed
  
  // base rotation
  double multiplier = length(lin_vel) / 100.0;
  if (input.buttons & Control::Input::TURN_RIGHT) {
    rot_vel += dt * (300.0 - (clamp(300.0 * multiplier - 50.0, 0.0, 300.0)));
    lin_vel *= 0.95;
  }
  else if (input.buttons & Control::Input::TURN_LEFT) {
    rot_vel += dt * -(300.0 - (clamp(300.0 * multiplier - 50.0, 0.0, 300.0)));
    lin_vel *= 0.95;
  }
  else
    rot_vel *= 0.8;
  
  rot_vel = clamp(rot_vel, -150.0f, 150.0f);
  
  // turret rotation
  vec2 aim_diff = vec2(input.aim_x, input.aim_y) - pos;
  double aim_dir = atan2(aim_diff.y, aim_diff.x) / M_PI * 180.0;
  double aim_dir_shortest = wrap(aim_dir - turret_dir, -180.0, 180.0);
  double min_diff = std::min(dt * 200.0, fabs(aim_dir_shortest));
  
  if (aim_dir_shortest > 0.1)
    turret_dir += min_diff;
  else if (aim_dir_shortest < -0.1)
    turret_dir -= min_diff;
  else
    turret_dir = aim_dir;
  
  turret_dir += rot_vel * dt;
  pos += lin_vel * dt;
  base_dir += rot_vel * dt;
}
/* <--- end tank state ---> */

Tank::Tank(class GameServer *gameserver)
  : Entity(16.0f)
  , _gameserver(gameserver)
{
  _reload_time = 0.0;
}

const Tank::State &Tank::state() const {
  return _state;
}

void Tank::assign(const Tank::State &state) {
  _state = state;
}


void Tank::snap(Packer &msg, const class ClientSession *client) {
  msg.writeShort(1); // 1 = tank
  _state.write(msg);
}

void Tank::tick() {
  double input_dt = _gameserver->tickDuration() / std::max<int>(_lastinput.size(), 1);
  for (size_t i = 0; i < _lastinput.size(); ++i) {
    if (_reload_time >= 0.0)
      _reload_time -= input_dt;
    else if (_lastinput[i].buttons & Control::Input::SHOOT)
      shoot();
    
    _state.advance(_lastinput[i], input_dt);    
  }
  
  if (!_lastinput.empty()) {
    _lastinput.erase(_lastinput.begin(), _lastinput.begin() + (_lastinput.size() - 1));    
  }
}

void Tank::recvInput(const Control::Input &input) {
  _lastinput.push_back(input);
}

void Tank::shoot() {
  _gameserver->spawnBullet(_state.pos + vec2::FromDegrees(_state.turret_dir) * 16.0f, 
                           _state.turret_dir, id());
  _reload_time = 0.05;
}

void Tank::takeDamage(const vec2 &at, int amount) {
  vec2 diff = normalized(_state.pos - at) * radius();
  _state.lin_vel += diff * 10.0f;
}