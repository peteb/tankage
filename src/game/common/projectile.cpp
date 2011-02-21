#include <game/common/projectile.h>
#include <game/common/system.h>
#include <game/common/actors.h>
#include <game/common/tank.h>
#include <game/client/particles.h>

#include <engine/graphics.h>
#include <engine/texture.h>

#include <utils/vec.h>
#include <utils/rect.h>
#include <cmath>

Projectile::Projectile(class ParticleGroup *partGroup,
                       int shooterId, class Texture *tex,
                       const SystemContext *ctx, const vec2 &pos, int id)
  : partGroup(partGroup)
  , tex(tex)
  , ctx(ctx)
  , pos(pos)
  , shooterId(shooterId)
  , _id(id)
{
  sinceEmit = 0.0;
  /*
    group = ctx->particles()->group(particle_texture);
    group->addParticle(pos);
  */

}

bool Projectile::update(double dt) {
  const vec2 prevPos = pos;
  pos += vel * dt;
  sinceEmit += dt;
  
  vec2 hitPos;
  Tank *hit = ctx->actors()->intersectingTank(prevPos, pos, 1.0f, shooterId, hitPos);
  if (hit) {
    hit->takeDamage(hitPos, 10.0f);
    return false;
  }

  if (sinceEmit > 0.01) {
    Particle particle;
    particle.pos = pos;
    particle.ttd = 0.2;
    partGroup->addParticle(particle);
    sinceEmit = 0.0;
  }
  
  // update effect
  // partGroup->emitParticle(Particle(blabla));
//  emitter.setPosition(pos);
  if (pos.x - 64.0f > 800.0f || pos.x + 64.0f < 0.0f)
    return false;
  if (pos.y - 64.0f > 800.0f || pos.y + 64.0f < 0.0f)
    return false;
  
  return true;
}

void Projectile::render(Graphics *gfx) {
  gfx->setBlend(Graphics::BLEND_ALPHA);
  gfx->enableTextures();
  tex->bind();

  vec2 dir = normalized(vel);
  gfx->drawQuad(rect(pos, 32, 32), degrees(dir)); // FIXME: get rid of hardcoded sizes

}

void Projectile::setVel(const vec2 &vel) {
  this->vel = vel;
}

NetProjectileSnapshot Projectile::snapshot() const {
  NetProjectileSnapshot snap;
  snap.id = htons(_id);
  snap.x = htons(pos.x);
  snap.y = htons(pos.y);
  return snap;
}

void Projectile::onSnap(const NetProjectileSnapshot &netshot) {
  NetProjectileSnapshot snapshot;
  snapshot.x = ntohs(netshot.x);
  snapshot.y = ntohs(netshot.y);
  pos.x = snapshot.x;
  pos.y = snapshot.y;
}
