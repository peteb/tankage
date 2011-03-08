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
  
  TankVector::iterator i = tanks.begin(), e = tanks.end();
  for (; i != e; ++i) {
    Tank *tank = *i;
    const Tank::Input *predictDelta = context->control()->lastInput(tank->id());

    if (predictDelta) {
      // Client side prediction
      tank->advance(*predictDelta, dt);
    }

    tank->render(graphics);
  }
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
    msg->snaps[i] = tanks[i]->snapshot();
  }

  client->send(msg, packetSize, 0 /*Client::PACKET_UNSEQUENCED*/, NET_CHANNEL_ABS);
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
      if (tankEntry) {
        if (actor != localActor) {
          tankEntry->assign(msg->snaps[i]);
        }
        else {
          // Correct failures in the prediction
          // 1. get delta for time in packet
          // 2. set tank to this delta
          // 3. step forward all inputs after
          // 4. get the state at this point in time -> retval
          // FIXME: send input 10hz anyway, verify that saved moves are the correct ones
          // Control::history(time) -> iterator beg, iterator end
          // onTick will subsample the active move and send that
          
          Control::MoveRange history =
            context->control()->history(msg->last_input);
          
          if (history.first != history.second) {
            Tank::State rState(msg->snaps[i]); // FIXME: use same in tankEntry->assign
            vec2 diff = history.first->absolute.pos - rState.pos;

            std::cout << "DIFF: " << length(diff) << std::endl;
            if (length(diff) > 1.0f || abs(history.first->absolute.base_dir - rState.base_dir) > 1.0f) {
              std::cout << "Rewinding: " <<
                std::distance(history.first, history.second) << std::endl;
              std::cout << "   to snap: " << history.first->absolute.pos.x << std::endl;
              
              // Tank::State beforeRewind = tankEntry->snapshot();
              tankEntry->assign(rState);
              
              // Replay
              Control::MoveRing::iterator iter = history.first;
              for (; iter != history.second; ++iter) {
                //std::cout << "   advance " << iter->time << std::endl;
                //tankEntry->advance(iter->delta, iter->time);
              }
              
              context->control()->removeHistory(history.second);
            }
          }
        }
      }
      else {
        std::cout << "got update for not existing tank" << std::endl;
        createTank(snapshot);
      }
    }

  }
}

void Actors::createTank(const NetTankSnapshot &net_snapshot) {
  Tank *newTank = new Tank(ntohs(net_snapshot.id), context);
  newTank->setTexture(tankBase, tankTurret);
  tanks.push_back(newTank);
  newTank->assign(net_snapshot);
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
