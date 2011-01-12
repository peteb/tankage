#include <game/items.h>
#include <engine/window_manager.h>
#include <engine/graphics.h>
#include <engine/image_loader.h>
#include <engine/portal.h>
#include <engine/image.h>
#include <engine/texture.h>
#include <memory>
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

    
  lastUpdate = wm->timeSeconds();
  lastGentime = lastUpdate;
}

void Items::update() {
  double thisUpdate = wm->timeSeconds();
  double dt = thisUpdate - lastUpdate;
  lastUpdate = thisUpdate;
  
  CactusVector::iterator i = cactii.begin();
  for (; i != cactii.end(); ) {
    if (!(*i)->update(dt)) {
      delete *i;
      i = cactii.erase(i);
    }
    else {
      ++i;
    }
  }

  double secSinceGen = thisUpdate - lastGentime;
  
  if (secSinceGen > 0.5) {
    lastGentime = thisUpdate;
    float ran = float(rand()) / RAND_MAX;
    vec2 cactusPos = vec2(400.0f + (ran - 0.5f) * 100.0f, 632.0f);
    cactii.push_back(new Cactus(cactusPos, cactusTexture));
  }

  {
    projectiles.erase(std::remove_if(projectiles.begin(),
                                     projectiles.end(),
                                     std::not1(std::bind2nd(std::mem_fun(&Projectile::update), dt))),
                      projectiles.end());
  }

}

void Items::spawnProjectile(ProjectileType type, const vec2 &pos,
                            const vec2 &dir, int shooterId) {
  std::auto_ptr<Projectile> proj(new Projectile(pos, dir * 4000.0f,
                                                shooterId, bulletTexture));
  projectiles.push_back(proj.release());
}

void Items::render() {
  std::for_each(cactii.begin(), cactii.end(), std::bind2nd(std::mem_fun(&Cactus::render), gfx));
  std::for_each(projectiles.begin(), projectiles.end(), std::bind2nd(std::mem_fun(&Projectile::render), gfx));
}


Cactus::Cactus(const vec2 &pos, class Texture *tex)
  : pos(pos)
  , tex(tex)
{
}

void Cactus::render(Graphics *gfx) {
  gfx->setBlend(Graphics::BLEND_ALPHA);
  gfx->enableTextures();
  tex->bind();

  gfx->drawQuad(rect(pos, 64, 64));
}

bool Cactus::update(double dt) {
  pos += vec2(0.0f, -200.0f) * dt;

  return (pos.y + 64.0f >= 0.0f);
}




Projectile::Projectile(const vec2 &pos, const vec2 &vel, int shooterId, class Texture *tex)
  : tex(tex)
  , pos(pos)
  , vel(vel)
  , shooterId(shooterId)
{
}

bool Projectile::update(double dt) {
  pos += vel * dt;
  return pos.x - 64.0f <= 800.0f;
}

void Projectile::render(Graphics *gfx) {
  gfx->setBlend(Graphics::BLEND_ALPHA);
  gfx->enableTextures();
  tex->bind();

  gfx->drawQuad(rect(pos, 64, 64));

}

