#include <game/common/actors.h>
#include <game/common/items.h>
#include <game/common/net_protocol.h>
#include <game/common/tank.h>

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


#include <algorithm>
#include <iostream>
#include <memory>
#include <cmath>

Actors::Actors() {
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

Tank *Actors::createActor(class Client *client) {
  Tank *newTank = new Tank(++lastId, context);
  newTank->setTexture(tankBase, tankTurret);
  tanks.push_back(newTank);

  return newTank;
}

void Actors::render() {
  double thisUpdate = wm->timeSeconds();
  double dt = thisUpdate - lastUpdate;
  lastUpdate = thisUpdate;

  // The tanks list should be immutable during update operations
  TankVector::iterator i = tanks.begin(), e = tanks.end();
  for (; i != e; ++i) {
    if ((*i)->update(dt)) {
      // It's alive!
      (*i)->render(graphics);
    }
  }
}


void Actors::onTick(class Client *client) {
  size_t packetSize = sizeof(NetTanksSnapMsg) +
    sizeof(NetTankSnapshot) * tanks.size();

  NetTanksSnapMsg *msg = static_cast<NetTanksSnapMsg *>(malloc(packetSize));
  msg->type = NET_TANKS_UPDATE;
  msg->num_snapshots = tanks.size();
  
  for (size_t i = 0; i < tanks.size(); ++i) {
    msg->snaps[i] = tanks[i]->snapshot();
  }

  client->send(msg, packetSize, 0, NET_CHANNEL_ABS);
}

void Actors::onReceive(NetPacketType type, const Packet &packet) {
  if (type == NET_TANKS_UPDATE) {
    const NetTanksSnapMsg *msg =
      static_cast<const NetTanksSnapMsg *>(packet.data());

    for (size_t i = 0; i < msg->num_snapshots; ++i) {
      const NetTankSnapshot &snapshot = msg->snaps[i];
      const ActorId actor = ntohs(snapshot.id);
      Tank *tankEntry = tank(actor);
      if (tankEntry) {
        tankEntry->onSnap(msg->snaps[i]);
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
  newTank->onSnap(net_snapshot);
}

Tank *Actors::tank(int id) const {
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
