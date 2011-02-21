#include <game/common/projectiles.h>
#include <game/common/projectile.h>
#include <game/common/actors.h>
#include <game/common/texture_loader.h>
#include <game/client/particles.h>

#include <engine/window_manager.h>
#include <engine/graphics.h>
#include <engine/portal.h>
#include <engine/texture.h>
#include <engine/network.h>

#include <utils/algorithm.h>

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
  TextureLoader *textures = context->textureLoader();
  
  bulletTexture = textures->texture("bullet.png");
  smoke = textures->texture("smoke.png");

  lastUpdate = wm->timeSeconds();
}
                                        
void Projectiles::update() {
  double thisUpdate = wm->timeSeconds();
  double dt = thisUpdate - lastUpdate;
  lastUpdate = thisUpdate;


  ProjectileVector::iterator beg =
    remove_nif(projectiles.begin(), projectiles.end(),
               &Projectile::update, dt);
  
  std::for_each(beg, projectiles.end(), delete_op());
  projectiles.erase(beg, projectiles.end());
}

void Projectiles::spawnProjectile(ProjectileType type, const vec2 &pos,
                            const vec2 &dir, int shooterId) {
  class ParticleGroup *particles = context->particles()->group(smoke);
  std::auto_ptr<Projectile> newProjectile(
    new Projectile(particles, shooterId, bulletTexture, context, pos, ++projectileId));

  newProjectile->setVel(dir * 2000.0f);
  projectiles.push_back(newProjectile.release());
}

void Projectiles::render() {
  std::for_each(projectiles.begin(), projectiles.end(), std::bind2nd(std::mem_fun(&Projectile::render), gfx));
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

  client->send(msg, packetSize, Client::PACKET_UNSEQUENCED, NET_CHANNEL_ABS);
}

void Projectiles::onReceive(NetPacketType type, const Packet &packet) {
  if (type == NET_PROJECTILES_UPDATE) {
    const NetProjectilesSnapMsg *msg =
      static_cast<const NetProjectilesSnapMsg *>(packet.data());

    for (size_t i = 0; i < msg->num_snapshots; ++i) {
      const NetProjectileSnapshot &snapshot = msg->snaps[i];
      const int projectileId = ntohs(snapshot.id);

      bool found = false;
      for (size_t a = 0; a < projectiles.size(); ++a) {
        if (projectiles[a]->id() == projectileId) {
          projectiles[a]->onSnap(snapshot);
          found = true;
          break;
        }
      }

      if (!found) {
        class ParticleGroup *particles = context->particles()->group(smoke);
        Projectile *newProjectile = new Projectile(
          particles, 0, bulletTexture, context, vec2::Zero(), projectileId);

        newProjectile->onSnap(snapshot);
        projectiles.push_back(newProjectile);
      }
    }

  }
}
