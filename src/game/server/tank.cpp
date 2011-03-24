#include "tank.h"
#include <utils/packer.h>
#include <game/server/gameserver.h>

/* <--- begin tank state ---> */
Tank::State &Tank::State::write(Packer &msg) {
  msg.writeInt(id);
  msg.writeShort(pos.x * 10.0f);
  msg.writeShort(pos.y * 10.0f);
  msg.writeShort(base_dir / 360.0f * 65536.0f);
  return *this;
}

Tank::State &Tank::State::read(class Unpacker &msg) {
  id = msg.readInt();
  pos.x = static_cast<float>(msg.readShort()) / 10.0f;
  pos.y = static_cast<float>(msg.readShort()) / 10.0f;
  base_dir = static_cast<float>(msg.readShort()) / 65536.0f * 360.0f;
  return *this;
}

void Tank::State::advance(const Control::Input &input, double duration) {
  const double DELTA_TIME = 1.0/40.0;
  
  for (; duration >= DELTA_TIME; duration -= DELTA_TIME) {
    integrate(input, DELTA_TIME);
  }
  
  // integrate any leftovers
  integrate(input, duration);
}

void Tank::State::integrate(const Control::Input &input, double dt) {
  vec2 forward = vec2::FromDegrees(base_dir);
  
  if (input.buttons & Control::Input::FORWARD)
    pos += forward * dt * 120.0;
  if (input.buttons & Control::Input::BACKWARD)
    pos += forward * dt * -120.0;
  if (input.buttons & Control::Input::TURN_RIGHT)
    base_dir += dt * 45.0;
  if (input.buttons & Control::Input::TURN_LEFT)
    base_dir += dt * -45.0;

}
/* <--- end tank state ---> */

Tank::Tank(class GameServer *gameserver)
  : _gameserver(gameserver)
{
  
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
  _state.advance(_lastinput, _gameserver->tickDuration());
}

void Tank::recvInput(const Control::Input &input) {
  _lastinput = input;
}