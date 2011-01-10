#include <game/projectiles.h>
#include <engine/graphics.h>
#include <engine/image.h>
#include <engine/portal.h>
#include <engine/texture.h>
#include <engine/image_loader.h>
#include <engine/window_manager.h>

Projectiles::Projectiles(const class Portal &interfaces, SystemContext *ctx)
  : System(ctx)
{
  gfx = interfaces.requestInterface<Graphics>();
  wm = interfaces.requestInterface<WindowManager>();
  ImageLoader *imgLoader = interfaces.requestInterface<ImageLoader>();
  
  std::auto_ptr<Image> img(imgLoader->loadImage("../data/bullet.png"));
  bulletTexture = gfx->createTexture(img.get());
  lastUpdate = wm->timeSeconds();
}

void Projectiles::spawnBullet(const vec2 &pos, const vec2 &dir, int shooterId) {
  bullets.push_back(new Bullet(bulletTexture, pos, dir * 4000.0f, shooterId));
}

void Projectiles::update() {
  double thisUpdate = wm->timeSeconds();
  double dt = thisUpdate - lastUpdate;
  lastUpdate = thisUpdate;
  
  for (BulletVector::iterator i = bullets.begin(); i != bullets.end(); ) {
    if (!(*i)->update(dt))
      i = bullets.erase(i);
    else {
      (*i)->render(gfx);
      ++i;
    }
  }
}

Bullet::Bullet(class Texture *tex, const vec2 &pos, const vec2 &vel, int shooterId)
  : tex(tex)
  , pos(pos)
  , vel(vel)
  , shooterId(shooterId)
{
}

bool Bullet::update(double dt) {
  pos += vel * dt;
  return true;
}

void Bullet::render(Graphics *gfx) {
  gfx->setBlend(Graphics::BLEND_ALPHA);
  gfx->enableTextures();
  tex->bind();

  gfx->drawQuad(rect(pos, 64, 64));

}

