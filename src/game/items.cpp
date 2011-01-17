#include <game/items.h>
#include <game/cactus.h>

#include <engine/window_manager.h>
#include <engine/graphics.h>
#include <engine/image_loader.h>
#include <engine/portal.h>
#include <engine/image.h>
#include <engine/texture.h>
#include <game/snails.h>
#include <memory>
#include <functional>
#include <algorithm>
#include <iostream>

Items::Items(const class Portal &interfaces, SystemContext *ctx)
  : System(ctx)
{
  wm = interfaces.requestInterface<WindowManager>();
  gfx = interfaces.requestInterface<Graphics>();
  ImageLoader *imgLoader = interfaces.requestInterface<ImageLoader>();

  {  
    std::auto_ptr<Image> img(imgLoader->loadImage("../data/cactus1.png"));
    cactusTexture = gfx->createTexture(img.get());
  }
  {  
    std::auto_ptr<Image> img(imgLoader->loadImage("../data/bullet.png"));
    bulletTexture = gfx->createTexture(img.get());
  }
  {  
    std::auto_ptr<Image> img(imgLoader->loadImage("../data/health_powerup.png"));
    healthPowerup = gfx->createTexture(img.get());
  }

    
  lastUpdate = wm->timeSeconds();
  lastGentime = lastUpdate;
}

void Items::update() {
  double thisUpdate = wm->timeSeconds();
  double dt = thisUpdate - lastUpdate;
  lastUpdate = thisUpdate;

  // if update returns false, remove
  items.erase(std::remove_if(items.begin(),
                             items.end(),
                             std::not1(std::bind2nd(std::mem_fun(&Item::update), dt))),
              items.end());
  

  double secSinceGen = thisUpdate - lastGentime;
  
  if (secSinceGen > 0.5) {
    lastGentime = thisUpdate;
    float ran = float(rand()) / RAND_MAX;
    vec2 cactusPos = vec2(400.0f + (ran - 0.5f) * 100.0f, 632.0f);

    if (rand() % 20 > 16) {
      items.push_back(new Powerup(cactusPos, healthPowerup, "health", 20));
    }
    else {
      items.push_back(new Cactus(cactusPos, cactusTexture));
    }
  }

  projectiles.erase(std::remove_if(projectiles.begin(),
                                   projectiles.end(),
                                   std::not1(std::bind2nd(std::mem_fun(&Projectile::update), dt))),
                    projectiles.end());

}

void Items::spawnProjectile(ProjectileType type, const vec2 &pos,
                            const vec2 &dir, class Snail *shooter) {
  std::auto_ptr<Projectile> proj(new Projectile(pos, dir * 8000.0f,
                                                shooter, bulletTexture, context));
  projectiles.push_back(proj.release());
}

void Items::render() {
  std::for_each(items.begin(), items.end(), std::bind2nd(std::mem_fun(&Item::render), gfx));
  std::for_each(projectiles.begin(), projectiles.end(), std::bind2nd(std::mem_fun(&Projectile::render), gfx));
}

Item *Items::intersectingItem(const vec2 &start, const vec2 &end, float radius, vec2 &hitpos) {
  ItemVector::iterator i = items.begin(), e = items.end();
  for (; i != e; ++i) {
    if ((*i)->intersects(start, end, radius, hitpos)) {
      return *i;
    }
  }

  return 0;
}

Item::Item(const vec2 &pos, float radius)
  : pos(pos)
  , radius(radius)
{

}

bool Item::intersects(const vec2 &start, const vec2 &end, float radius, vec2 &hitpos) {
  vec2 closest = closest_point(start, end, pos);
  if ((pos - closest).magnitude() <= radius + this->radius) {
    hitpos = closest;
    return true;
  }
  
  return false;
}


Powerup::Powerup(const vec2 &pos, class Texture *tex, const std::string &type, int amount)
  : Item(pos, 16.0f)
  , tex(tex)
  , type(type)
  , amount(amount)
  , taken(0)
{
}

void Powerup::render(Graphics *gfx) {
  gfx->setBlend(Graphics::BLEND_ALPHA);
  gfx->enableTextures();
  tex->bind();

  vec2 roundedPos;
  roundedPos.x = round(pos.x);
  roundedPos.y = round(pos.y);

  gfx->drawQuad(rect(roundedPos, 16, 16));
}

bool Powerup::update(double dt) {
  if (taken == 0)
    pos += vec2(0.0f, -200.0f) * dt;
  else
    pos += vec2(700.0f, 0.0f) * dt * static_cast<float>(taken);
  
  return (pos.y + 16.0f >= 0.0f) || (pos.x < -16.0f) || (pos.x > 816.0f);
}

bool Powerup::takeDamage(const vec2 &pos, float damage) {
  if (pos.x < this->pos.x)
    taken = -1;
  else
    taken = 1;
  
  return true;
}





Projectile::Projectile(const vec2 &pos, const vec2 &vel, class Snail *shooter, class Texture *tex, const SystemContext *ctx)
  : tex(tex)
  , pos(pos)
  , vel(vel)
  , shooter(shooter)
  , ctx(ctx)
{
}

bool Projectile::update(double dt) {
  const vec2 prevPos = pos;
  pos += vel * dt;
  
  vec2 hitPos;
  Snail *hit = ctx->snails()->intersectingSnails(prevPos, pos, 1.0f, shooter, hitPos);
  if (hit) {
    hit->takeDamage(hitPos, 10.0f);
    return false;
  }

  Item *hitItem = ctx->items()->intersectingItem(prevPos, pos, 1.0f, hitPos);
  if (hitItem) {
    if (hitItem->takeDamage(hitPos, 10.0f)) {
      return false;
    }
  }
  
  return pos.x - 64.0f <= 800.0f;
}

void Projectile::render(Graphics *gfx) {
  gfx->setBlend(Graphics::BLEND_ALPHA);
  gfx->enableTextures();
  tex->bind();

  gfx->drawQuad(rect(pos, 64, 64));

}

