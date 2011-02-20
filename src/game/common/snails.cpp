#include <game/common/snails.h>
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

Snails::~Snails() {
  // delete all the snails when game terminates
  std::for_each(snails.begin(), snails.end(), delete_op());
}

void Snails::init(const class Portal &interfaces) {
  graphics = interfaces.requestInterface<Graphics>();
  wm = interfaces.requestInterface<WindowManager>();
  ImageLoader *imgLoader = interfaces.requestInterface<ImageLoader>();

  // First snail
  {
    std::auto_ptr<Image> img(imgLoader->loadImage(std::string(RESOURCE_PATH) + "/tank_base.png"));
    std::auto_ptr<Image> img2(imgLoader->loadImage(std::string(RESOURCE_PATH) + "/tank_turret.png"));
    Snail *snail = new Snail(vec2(50.0f, 300.0f), Snails::SNAIL_LEFT, context);
    snail->setTexture(graphics->createTexture(img.get()),
                      graphics->createTexture(img2.get()));
  
    snails.push_back(snail);
  }

  // Second snail
  {
    std::auto_ptr<Image> img(imgLoader->loadImage(std::string(RESOURCE_PATH) + "/snail_r.png"));
    Snail *snail = new Snail(vec2(800-50.0f, 300.0f), Snails::SNAIL_RIGHT, context);
    snail->setTexture(graphics->createTexture(img.get()), NULL);
  
    snails.push_back(snail);
  }
  
  lastUpdate = wm->timeSeconds();
}

void Snails::render() {
  double thisUpdate = wm->timeSeconds();
  double dt = thisUpdate - lastUpdate;
  lastUpdate = thisUpdate;
  
  SnailVector::iterator i = snails.begin(), e = snails.end();
  for (; i != e; ++i) {
    if ((*i)->update(dt)) {
      // It's alive!
      (*i)->render(graphics);
    }
  }
}

Snail *Snails::snail(int id) const {
  return snails.at(id);
}

Snail *Snails::intersectingSnails(const vec2 &start, const vec2 &end,
                                  float radius, Snail *ignore, vec2 &hitpos) {

  for (SnailVector::iterator i = snails.begin(); i != snails.end(); ++i) {
    Snail *snail = *i;
    if (snail != ignore) {
      if (snail->intersects(start, end, radius, hitpos)) {
        return snail;
      }
    }
  }
  
  return 0;
}


void Snails::onTick(class Client *client) {
  size_t packetSize = sizeof(NetTanksSnapMsg) +
    sizeof(NetTankSnapshot) * snails.size();
  
  NetTanksSnapMsg *msg = static_cast<NetTanksSnapMsg *>(malloc(packetSize));
  
  msg->type = NET_TANKS_UPDATE;
  msg->num_snapshots = snails.size();
  
  for (size_t i = 0; i < snails.size(); ++i) {
    msg->snaps[i] = snails[i]->snapshot();
  }

  client->send(&msg, packetSize, 0, NET_CHANNEL_ABS);
}

void Snails::onReceive(NetPacketType type, const Packet &packet) {
  if (type == NET_TANKS_UPDATE) {
    const NetTanksSnapMsg *msg =
      static_cast<const NetTanksSnapMsg *>(packet.data());

    for (size_t i = 0; i < msg->num_snapshots; ++i) {
      snails[i]->onSnap(msg->snaps[i]);
    }
    
  }
}


