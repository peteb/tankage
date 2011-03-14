#include <netinet/in.h>
#include <game/common/actors.h>
#include <game/common/projectiles.h>
#include <game/common/net_protocol.h>
#include <game/common/tank.h>
#include <game/common/players.h>
#include <game/common/control.h>

#include <game/server/client_session.h>
#include <game/server/gameserver.h>

#include <engine/graphics.h>
#include <engine/texture.h>
#include <engine/image_loader.h>
#include <engine/window_manager.h>
#include <engine/portal.h>
#include <engine/image.h>
#include <engine/config.h>
#include <engine/packet.h>
#include <engine/network.h>

#include <utils/rect.h>
#include <utils/value.h>
#include <utils/color.h>
#include <utils/algorithm.h>

#include <arpa/inet.h>

#include <algorithm>
#include <iostream>
#include <memory>
#include <cmath>

Actors::Actors()
  : ReplicatedSystem(SERVER_TICK|CLIENT_RECEIVE)
{
  lastId = 0;
  gameTime = 0.0;
}

Actors::~Actors() {
  // delete all the tanks when game terminates
  std::for_each(tanks.begin(), tanks.end(), delete_op());
}

void Actors::init(const class Portal &interfaces) {
  graphics = interfaces.requestInterface<Graphics>();
  wm = interfaces.requestInterface<WindowManager>();
  
  ImageLoader *imgLoader = interfaces.requestInterface<ImageLoader>();

  std::auto_ptr<Image> img(imgLoader->loadImage(std::string(RESOURCE_PATH) + "/tank_base.png"));
  std::auto_ptr<Image> img2(imgLoader->loadImage(std::string(RESOURCE_PATH) + "/tank_turret.png"));

  tankBase = graphics->createTexture(img.get());
  tankTurret = graphics->createTexture(img2.get());

  lastUpdate = wm->timeSeconds();
}

Tank *Actors::createActor() {
  Tank *newTank = new Tank(++lastId, context);
  newTank->setTexture(tankBase, tankTurret);
  tanks.push_back(newTank);

  return newTank;
}

void Actors::render() {
  double thisUpdate = wm->timeSeconds();
  double dt = thisUpdate - lastUpdate;
  lastUpdate = thisUpdate;
  gameTime += dt;
  
  TankVector::iterator i = tanks.begin(), e = tanks.end();
  for (; i != e; ++i) {
    Tank *tank = *i;
    const Tank::Input *predictDelta = context->control()->lastInput(tank->id());

    if (predictDelta) {
      // Client side prediction, but also updating on the server side
      tank->advance(*predictDelta, dt);
    }

    tank->render(graphics);
  }

  graphics->disableTextures();
  graphics->setColor(color4(1.0f, 0.0f, 0.0f, 1.0f));
  graphics->drawCircle(correctedState.pos, 3.0f, 12.0f);
  vec2 delta = vec2::FromDegrees(correctedState.base_dir);

  graphics->setColor(color4(0.0f, 0.0f, 1.0f, 1.0f));
  graphics->drawCircle(correctedState.pos + delta * 16.0f, 3.0f, 12.0f);
  
}


void Actors::onTick(class Client *client) {
  ClientSession *session = context->gameserver()->session(client);
  if (!session) {
    // No session? Huh.
    return;
  }
  
  Player *player = context->players()->player(session->player);
  if (!player) {
    return;
  }

    
  size_t packetSize = sizeof(NetTanksSnapMsg) +
    sizeof(NetTankSnapshot) * tanks.size();

  NetTanksSnapMsg *msg = static_cast<NetTanksSnapMsg *>(malloc(packetSize));
  msg->type = NET_TANKS_UPDATE;
  msg->num_snapshots = tanks.size();

  
  const Tank::Input *lastInput = context->control()->lastInput(player->actor());
  msg->last_input = (lastInput ? lastInput->time : 0.0f);
  
  for (size_t i = 0; i < tanks.size(); ++i) {
    if (tanks[i]->id() == player->actor()) {
      msg->last_input += tanks[i]->count();
    }
    msg->snaps[i] = tanks[i]->snapshot();
  }

  client->send(msg, packetSize, 0 /*Client::PACKET_UNSEQUENCED*/, NET_CHANNEL_ABS);
}


namespace {
void replay(const Control::MoveRange &moves,
            Tank *target,
            const std::pair<double, double> &time)
{
  typedef Control::MoveRange::first_type iterator;
  double ofs = time.first;

  for (iterator iter = moves.first, next = ++iterator(moves.first);
       iter != moves.second;
       ++iter, ++next)
  {
    double clamped_endtime =
      (next != moves.second ? 
       std::min<double>(time.second, next->delta.time) :
       time.second);
        
    double duration = clamped_endtime - ofs;

    // FIXME: is this really the best way?
    for (double tim = 0.0; tim <= duration; tim += 0.001) {
      target->advance(iter->delta, 0.001);
    }
    
    ofs += duration;
  }
}
}

Tank::State Actors::rebaseHistory(double time, const Tank::State &newState, Tank *tank) {
  Tank::State ret = newState;
  Control::MoveRange history = context->control()->history(time);
  
  if (history.first != history.second) {
    Tank::State initial_state = tank->snapshot();
    double initial_count = tank->count();

    tank->assign(newState); // rewind history
    replay(history, tank, std::make_pair(time, gameTime + 1.0/20.0));
    ret = tank->snapshot();
    // FIXME: get tickrate (1.0/2.0) from server

    // FIXME: instead of doing this below, we could maybe get a copy of the tank
    // object, thus avoiding any side-effects. Need to make sure the copy can be
    // done fast (preferably just a copy of Tank::State)
    tank->assign(initial_state);
    tank->resetCount(initial_count);
  }

  return ret;
}

void Actors::onReceive(NetPacketType type, const Packet &packet) {
  if (type == NET_TANKS_UPDATE) {
    const NetTanksSnapMsg *msg =
      static_cast<const NetTanksSnapMsg *>(packet.data());

    ActorId localActor = context->players()->localActor();
    
    for (size_t i = 0; i < msg->num_snapshots; ++i) {
      const NetTankSnapshot &snapshot = msg->snaps[i];
      const ActorId actor = ntohs(snapshot.id);
      Tank *tankEntry = tank(actor);

      if (!tankEntry) {
        tankEntry = createTank(snapshot);
      }

      if (actor != localActor) {
        tankEntry->assign(msg->snaps[i]);
      }
      else {
        // It's the local player
        Tank::State corrected = rebaseHistory(msg->last_input, snapshot,
                                              tankEntry);
        
        const Tank::State current = tankEntry->snapshot();
        double diff = std::max(length(current.pos - corrected.pos), current.base_dir - corrected.base_dir);
        
        correctedState = corrected; // for rendering
        
        if (diff > 10.0f) {
          // a quick snap if too much error
          std::cout << "snap!" << std::endl;
          tankEntry->assign(corrected);
        }
        else if (diff >= 0.02f) {
          // lerp if minor
          // FIXME: lerping and probably be done smoother if added to the tank
          // something like, tank->setTargetState(...);
          
          Tank::State lerpState = current;
          lerpState = lerp(current, corrected, 0.1);
          tankEntry->assign(lerpState);

        }
        
      }
    }

    
  }
}

Tank *Actors::createTank(const NetTankSnapshot &net_snapshot) {
  Tank *newTank = new Tank(ntohs(net_snapshot.id), context);
  newTank->setTexture(tankBase, tankTurret);
  tanks.push_back(newTank);
  newTank->assign(net_snapshot);
  
  return newTank;
}

Tank *Actors::tank(ActorId id) const {
  for (TankVector::const_iterator i = tanks.begin(), e = tanks.end();
       i != e; ++i) {
    if ((*i)->id() == id) {
      return *i;
    }
  }

  return static_cast<Tank *>(0);
}

Tank *Actors::intersectingTank(const vec2 &start, const vec2 &end,
                               float radius, ActorId ignore, vec2 &hitpos) {

  for (TankVector::iterator i = tanks.begin(), e = tanks.end(); i != e; ++i) {
    const Tank *tank = *i;
    if (tank->id() != ignore) {
      if (tank->intersects(start, end, radius, hitpos)) {
        return *i;
      }
    }
  }
  
  return static_cast<Tank *>(0);
}
