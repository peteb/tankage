#include "resource.h"

#include <game/server/client_session.h>
#include <game/server/gameserver.h>
#include <game/server/tank.h>
#include <utils/packer.h>
#include <utils/log.h>

/* <--- begin resource state ---> */
Resource::State &Resource::State::write(class Packer &msg) {
  msg.writeInt(id);
  msg.writeByte(type);
  msg.writeShort(pos.x * 10.0f);
  msg.writeShort(pos.y * 10.0f);
  msg.writeInt(start_tick);

  return *this;
}

Resource::State &Resource::State::read(class Unpacker &msg) {
  id = msg.readInt();
  type = msg.readByte();
  pos.x = float(msg.readShort()) / 10.0f;
  pos.y = float(msg.readShort()) / 10.0f;
  start_tick = msg.readInt();
  
  return *this;  
}
/* <--- end resource state ---> */


Resource::Resource(class GameServer *gameserver) 
  : Entity(8.0f)
  , _gameserver(gameserver)
{
  
}

void Resource::snap(class Packer &msg, const class ClientSession *client) {
  if (Entity *tank = _gameserver->entity(client->tankid)) {
    const vec2 &tank_pos = tank->position();    
    if (fabs(_state.pos.x - tank_pos.x) > 1000.0f)
      return;
    if (fabs(_state.pos.y - tank_pos.y) > 1000.0f)
      return;
  }  
  
  msg.writeShort(3);
  _state.write(msg);
  
}

void Resource::tick() {
  // FIXME: maybe something that just takes a point, and not a ray?
  Tank *x_tank = _gameserver->intersectingTank(_state.pos, 
                                               _state.pos + vec2(0.0f, 1.0f), 
                                               radius(), 0);
  if (x_tank) {
    x_tank->takeItem(_state.type, 5);
    _gameserver->destroyEntity(id());
  }
  
}

