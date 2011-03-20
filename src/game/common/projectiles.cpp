#include <netinet/in.h>
#include <game/common/projectiles.h>
#include <game/common/projectile.h>
#include <game/common/actors.h>
#include <game/common/texture_loader.h>
#include <game/common/peer.h>

#include <game/client/particles.h>

#include <engine/window_manager.h>
#include <engine/graphics.h>
#include <engine/portal.h>
#include <engine/texture.h>
#include <engine/network.h>

#include <utils/algorithm.h>

#include <arpa/inet.h>

#include <memory>
#include <functional>
#include <algorithm>
#include <iostream>

Projectiles::Projectiles()
  : ReplicatedSystem(CLIENT_RECEIVE|SERVER_TICK)
{
  projectileId = 0;
}

Projectiles::~Projectiles() {

}

void Projectiles::init(const class Portal &interfaces) {
  wm = interfaces.requestInterface<WindowManager>();
  gfx = interfaces.requestInterface<Graphics>();
  context->system<Peer>()->registerSystem(this);

  TextureLoader *textures = context->textureLoader();
  
  bulletTexture = textures->texture("bullet.png");
  smoke = textures->texture("smoke.png");

  lastUpdate = wm->timeSeconds();
}
                                        
void Projectiles::update() {
  double thisUpdate = wm->timeSeconds();
  double dt = thisUpdate - lastUpdate;
  lastUpdate = thisUpdate;

  // Execute update on all projectiles, remove those who return false
  ProjectileVector::iterator beg =
    remove_nif(projectiles.begin(), projectiles.end(),
               &Projectile::update, dt);
  
  std::for_each(beg, projectiles.end(), delete_op());
  projectiles.erase(beg, projectiles.end());
}

void Projectiles::spawnProjectile(ProjectileType type, const vec2 &pos,
                                  float dir, ActorId shooterId) {
  Projectile *newProjectile = new Projectile(context, ++projectileId);
  newProjectile->setVelocity(vec2::FromDegrees(dir) * 2000.0f);
  newProjectile->setPosition(pos);
  newProjectile->setTexture(bulletTexture);
  newProjectile->setShooter(shooterId);
  projectiles.push_back(newProjectile);
}

void Projectiles::render() {
  for (ProjectileVector::iterator i = projectiles.begin(), e = projectiles.end();
       i != e; ++i) {
    (*i)->render(gfx);
  }
}

void Projectiles::onTick(Client *client) {
  size_t packetSize = sizeof(NetProjectilesSnapMsg) +
    sizeof(NetProjectileSnapshot) * projectiles.size();

  NetProjectilesSnapMsg *msg = static_cast<NetProjectilesSnapMsg *>(malloc(packetSize));
  msg->type = NET_PROJECTILES_UPDATE;
  msg->num_snapshots = projectiles.size();

  for (size_t i = 0; i < projectiles.size(); ++i) {
    msg->snaps[i] = projectiles[i]->snapshot();
  }

  client->send(msg, packetSize, 0 /*Client::PACKET_UNSEQUENCED*/, NET_CHANNEL_ABS);
}

void Projectiles::onReceive(NetPacketType type, const Packet &packet) {
  if (type == NET_PROJECTILES_UPDATE) {
    const NetProjectilesSnapMsg *msg =
      static_cast<const NetProjectilesSnapMsg *>(packet.data());

    for (size_t i = 0; i < msg->num_snapshots; ++i) {
      const NetProjectileSnapshot &snapshot = msg->snaps[i];
      const uint32_t projectileId = ntohs(snapshot.id);

      Projectile *ile = projectile(projectileId);
      if (!ile) {
        ile = createProjectile(projectileId);
      } 
      ile->onSnap(snapshot); 
    } // for
  }
} // onReceive

Projectile *Projectiles::projectile(ProjectileId id) const {
  Projectile *ile = 0;
  for (size_t a(0); a != projectiles.size(); ++a) {
    if (projectiles[a]->id() == id) {
      ile = projectiles[a];
      break;
    }
  }
  return ile;
}

Projectile *Projectiles::createProjectile(ProjectileId id) {
  Projectile *ile = new Projectile(context, id);
  ile->setTexture(bulletTexture);
  projectiles.push_back(ile);
  return ile;
}

