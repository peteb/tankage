#include "bullet.h"

#include <utils/packer.h>
#include <game/server/gameserver.h>
#include <cmath>

/* <--- begin bullet state ---> */
Bullet::State &Bullet::State::write(Packer &msg) {
  msg.writeInt(id);
  msg.writeShort(start_pos.x * 10.0f);
  msg.writeShort(start_pos.y * 10.0f);
  msg.writeInt(start_tick);
  msg.writeShort(dir / 360.0f * 65536.0f);
  
  return *this;
}

Bullet::State &Bullet::State::read(class Unpacker &msg) {
  id = msg.readInt();
  start_pos.x = static_cast<float>(msg.readShort()) / 10.0f;
  start_pos.y = static_cast<float>(msg.readShort()) / 10.0f;
  start_tick = msg.readInt();
  dir = static_cast<float>(msg.readShort()) / 65536.0f * 360.0f;

  return *this;
}

vec2 Bullet::State::positionAt(double time_ofs) {
  return vec2(0.0f, 0.0f);
}
/* <--- end bullet state ---> */

Bullet::Bullet(class GameServer *gameserver, int shooter) 
  : _gameserver(gameserver)
  , _shooter(shooter)
{
  _alive_time = 1.5;
}

void Bullet::snap(class Packer &msg, const class ClientSession *client) {
  msg.writeShort(2); // 2 = bullet
  _state.write(msg);
}

void Bullet::tick() {
  _alive_time -= _gameserver->tickDuration();
  
  if (_alive_time <= 0.0) {
    _gameserver->destroyEntity(id());
  }
}
