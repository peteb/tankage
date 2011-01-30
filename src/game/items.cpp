#include <game/items.h>
#include <game/cactus.h>
#include <game/projectile.h>
#include <game/powerup.h>
#include <game/snails.h>
#include <game/particles.h>
#include <game/texture_loader.h>

#include <engine/window_manager.h>
#include <engine/graphics.h>
#include <engine/portal.h>
#include <engine/texture.h>

#include <utils/algorithm.h>

#include <memory>
#include <functional>
#include <algorithm>
#include <iostream>

Items::~Items() {
  // deletes all the items when game terminates
  std::for_each(items.begin(), items.end(), delete_op());
}

void Items::init(const class Portal &interfaces) {
  wm = interfaces.requestInterface<WindowManager>();
  gfx = interfaces.requestInterface<Graphics>();
  TextureLoader *textures = context->textureLoader();
  
  cactusTexture = textures->texture("cactii.png");
  bulletTexture = textures->texture("bullet.png");
  healthPowerup = textures->texture("health_powerup.png");
  smoke = textures->texture("smoke.png");

    
  lastUpdate = wm->timeSeconds();
  lastGentime = lastUpdate;
}
                                        
void Items::update() {
  double thisUpdate = wm->timeSeconds();
  double dt = thisUpdate - lastUpdate;
  lastUpdate = thisUpdate;

  // if update returns false, remove
  {
    ItemVector::iterator beg =
      remove_nif(items.begin(), items.end(), &Item::update, dt);
    std::for_each(beg, items.end(), delete_op());    
    items.erase(beg, items.end());
  }
  
  double secSinceGen = thisUpdate - lastGentime;
  
  if (secSinceGen > 0.5) {
    lastGentime = thisUpdate;
    float ran = float(rand()) / RAND_MAX;
    vec2 cactusPos = vec2(400.0f + (ran - 0.5f) * 100.0f, 632.0f);

    if (rand() % 20 > 16) {
      items.push_back(new Powerup(cactusPos, healthPowerup, "health", 20, context));
    }
    else {
      items.push_back(new Cactus(cactusPos, cactusTexture));
    }
  }

  {
    ProjectileVector::iterator beg =
      remove_nif(projectiles.begin(), projectiles.end(),
                 &Projectile::update, dt);
    
    std::for_each(beg, projectiles.end(), delete_op());
    projectiles.erase(beg, projectiles.end());
  }
}

void Items::spawnProjectile(ProjectileType type, const vec2 &pos,
                            const vec2 &dir, class Snail *shooter) {
  class ParticleGroup *particles = context->particles()->group(smoke);
  std::auto_ptr<Projectile> newProjectile(
    new Projectile(particles, shooter, bulletTexture, context, pos));

  newProjectile->setVel(dir * 2000.0f);
  projectiles.push_back(newProjectile.release());
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


