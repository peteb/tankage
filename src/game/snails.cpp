#include <game/snails.h>
#include <engine/graphics.h>
#include <engine/texture.h>
#include <engine/image_loader.h>
#include <engine/window_manager.h>
#include <engine/portal.h>
#include <engine/image.h>
#include <game/items.h>
#include <utils/rect.h>
#include <utils/value.h>

#include <algorithm>

Snails::Snails(const class Portal &interfaces, SystemContext *ctx)
  : System(ctx)
{
  graphics = interfaces.requestInterface<Graphics>();
  wm = interfaces.requestInterface<WindowManager>();
  ImageLoader *imgLoader = interfaces.requestInterface<ImageLoader>();

  // First snail
  {
    std::auto_ptr<Image> img(imgLoader->loadImage("../data/snail_l.png"));
    Snail *snail = new Snail(vec2(50.0f, 300.0f), Snails::SNAIL_LEFT, ctx);
    snail->setTexture(graphics->createTexture(img.get()));
  
    snails.push_back(snail);
  }

  // Second snail
  {
    std::auto_ptr<Image> img(imgLoader->loadImage("../data/snail_r.png"));
    Snail *snail = new Snail(vec2(800-50.0f, 300.0f), Snails::SNAIL_RIGHT, ctx);
    snail->setTexture(graphics->createTexture(img.get()));
  
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
    (*i)->update(dt);
    (*i)->render(graphics);
  }
}

Snail *Snails::snail(int id) const {
  return snails.at(id);
}


Snail::Snail(const vec2 &initialPos, int id, SystemContext *ctx)
  : position(initialPos)
  , id(id)
  , context(ctx)
{
  std::fill(&_state[0], &_state[STATE_MAX], 0);
  secondsSinceFire = 0.0;
}

void Snail::startState(SnailState state) {
  _state[state] = true;
}

void Snail::stopState(SnailState state) {
  _state[state] = false;
}

void Snail::setTexture(Texture *texture) {
  this->texture = texture;
}

void Snail::render(Graphics *graphics) {
  graphics->setBlend(Graphics::BLEND_ALPHA);
  graphics->enableTextures();
  texture->bind();

  vec2 roundedPos;
  roundedPos.x = round(position.x);
  roundedPos.y = round(position.y);

  graphics->drawQuad(rect(roundedPos, 64, 64));
}

void Snail::update(double dt) {
  secondsSinceFire += dt;
  
  if (_state[STATE_MOVE_UP])
    position += vec2(0.0f, -500.0f) * dt;
  if (_state[STATE_MOVE_DOWN])
    position += vec2(0.0f, 500.0f) * dt;

  position.y = clamp(position.y, 32.0f, 600.0f - 32.0f);
  
  if (_state[STATE_SHOOT]) {// FIXME: rename SHOOT to SHOOTING
    if (secondsSinceFire >= 0.5) {
      vec2 dir = (id == Snails::SNAIL_LEFT ? vec2(1.0f, 0.0f) : vec2(-1.0f, 0.0f));
      context->items()->spawnProjectile(Items::PROJECTILE_BULLET, position, dir, id);
      secondsSinceFire = 0.0;
    }
    
  }
}
