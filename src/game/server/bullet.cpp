#include "bullet.h"

#include <game/server/client_session.h>
#include <game/server/gameserver.h>
#include <game/server/tank.h>
#include <utils/packer.h>
#include <cmath>

/* <--- begin bullet state ---> */
Bullet::State &Bullet::State::write(Packer &msg) {
  msg.writeInt(id);
  msg.writeShort(start_pos.x * 10.0f);
  msg.writeShort(start_pos.y * 10.0f);
  msg.writeShort(start_vel.x * 1000.0f); // FIXME: start_vel is not used, remove?
  msg.writeShort(start_vel.y * 1000.0f);
  msg.writeInt(start_tick);
  msg.writeShort(dir / 360.0f * 65536.0f);
  msg.writeShort(max_lerp * 1000.0f);
  
  return *this;
}

Bullet::State &Bullet::State::read(class Unpacker &msg) {
  id = msg.readInt();
  start_pos.x = static_cast<float>(msg.readShort()) / 10.0f;
  start_pos.y = static_cast<float>(msg.readShort()) / 10.0f;
  start_vel.x = static_cast<float>(msg.readShort()) / 1000.0f;
  start_vel.y = static_cast<float>(msg.readShort()) / 1000.0f;
  start_tick = msg.readInt();
  dir = static_cast<float>(msg.readShort()) / 65536.0f * 360.0f;
  max_lerp = static_cast<float>(msg.readShort()) / 1000.0f;

  return *this;
}

vec2 Bullet::State::positionAt(int tick, double ofs, double tick_duration) const {
  if ((unsigned)tick < start_tick)
    return start_pos;
  
  double start_ofs = (tick - start_tick) * tick_duration + ofs;
  return start_pos + vec2::FromDegrees(dir) * start_ofs * 800.0;
}
/* <--- end bullet state ---> */

Bullet::Bullet(class GameServer *gameserver, int shooter) 
  : Entity(0.0f)
  , _gameserver(gameserver)
  , _shooter(shooter)
{
  _alive_time = 1.5;
}

void Bullet::snap(class Packer &msg, const ClientSession *client) {
  if (Entity *tank = _gameserver->entity(client->tankid)) {
    vec2 bullet_pos = _state.positionAt(_gameserver->gameTick(), 0.0, _gameserver->tickDuration());
    const vec2 &tank_pos = tank->position();
    
    if (fabs(bullet_pos.x - tank_pos.x) > 1000.0f)
      return;
    if (fabs(bullet_pos.y - tank_pos.y) > 1000.0f)
      return;
  }
  
  msg.writeShort(2); // 2 = bullet
  _state.write(msg);
}

void Bullet::tick() {
//  if (_state.start_tick > _gameserver->gameTick())
//    return;
  
  vec2 last_pos = _state.positionAt(_gameserver->gameTick() - 1, 0.0, _gameserver->tickDuration());
  vec2 current_pos = _state.positionAt(_gameserver->gameTick(), 0.0, _gameserver->tickDuration());

  _state.max_lerp = 10.0f;

  Tank *x_tank = _gameserver->intersectingTank(last_pos, current_pos, 0.0f, _shooter);
  if (x_tank) {
    x_tank->takeDamage(last_pos, 5);
    _gameserver->events().spawnTankHit(x_tank->id(), _shooter, x_tank->state().pos);
    // FIXME: get hit position from intersectingTank; then send that in event
    _alive_time = 0.0;
    _state.max_lerp = (length(last_pos - x_tank->state().pos) - 16.0f) / 800.0; // FIXME: don't hardcode velocity
  }

  vec2 hit_pos;
  std::pair<int, int> tile;
  if (_gameserver->map().intersectSolid(last_pos, current_pos, 0.0f, hit_pos, tile)) {
    _alive_time = 0.0;
    _state.max_lerp = length(hit_pos - last_pos) / 800.0;
    _gameserver->map().damageTile(tile);
  }
  
  _alive_time -= _gameserver->tickDuration();
  
  if (_alive_time <= 0.0) {
    _gameserver->destroyEntity(id());
  }
}
