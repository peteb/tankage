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

Projectile::Projectile(const SystemContext *ctx, uint32_t id)
  : ctx(ctx)
  , _id(id)
{
  snapshotted = false;
}

bool Projectile::update(double dt) {
  const vec2 prevPos = position;
  position += velocity * dt;
  
  vec2 hitPos;
  Tank *hit = ctx->actors()->intersectingTank(prevPos, position, 1.0f, shooterId, hitPos);
  if (hit) {
    hit->takeDamage(hitPos, 10.0f);
    return false;
  }

  if (position.x - 64.0f > 800.0f || position.x + 64.0f < 0.0f)
    return false;
  if (position.y - 64.0f > 800.0f || position.y + 64.0f < 0.0f)
    return false;
  
  return true;
}

void Projectile::render(Graphics *gfx) {
  gfx->setBlend(Graphics::BLEND_ALPHA);
  gfx->enableTextures();
  tex->bind();

  vec2 dir = normalized(velocity);
  gfx->drawQuad(rect(position, 32, 32), degrees(dir)); // FIXME: get rid of hardcoded sizes
}

void Projectile::setVelocity(const vec2 &vel) {
  velocity = vel;
}

void Projectile::setPosition(const vec2 &pos) {
  position = pos;
}

NetProjectileSnapshot Projectile::snapshot() const {
  NetProjectileSnapshot snap;
  snap.id = htons(_id);
  snap.x = htons(position.x);
  snap.y = htons(position.y);
  snap.dir = htons(degrees(normalized(velocity)) + 180.0);
  return snap;
}

void Projectile::onSnap(const NetProjectileSnapshot &netshot) {
  NetProjectileSnapshot snapshot;
  snapshot.x = ntohs(netshot.x);
  snapshot.y = ntohs(netshot.y);

  const vec2 lastPos = position;
  vec2 newPos(snapshot.x, snapshot.y);
  if (length(newPos - position) >= 2.0) {
    position = newPos;
  }

  velocity = vec2::FromDirection(ntohs(netshot.dir) - 180.0) * 1000.0;
}
