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
  if (input.buttons & Control::Input::FORWARD) 
    pos.x += dt * 20.0;
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
  // this is where we need the input
  double time = _gameserver->gameTick() * _gameserver->tickDuration();
  _state.pos = vec2(400.0f, 300.0f) + vec2::FromDegrees(time * 90.0) * 200.0f;
}

void Tank::recvInput(const Control::Input &input) {

}