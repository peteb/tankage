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
  
  std::auto_ptr<Image> img(imgLoader->loadImage("../data/cactus1.png"));
  cactusTexture = gfx->createTexture(img.get());
  
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
}

void Items::render() {
  CactusVector::iterator i = cactii.begin(), e = cactii.end();
  for (; i != e; ++i) {
    (*i)->render(gfx);
  }
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

  return (pos.y + 64.0f >= 0.0f); // should it be displayed?   
}
