#include <game/snails.h>
#include <game/items.h>

#include <engine/graphics.h>
#include <engine/texture.h>
#include <engine/image_loader.h>
#include <engine/window_manager.h>
#include <engine/portal.h>
#include <engine/image.h>

#include <utils/rect.h>
#include <utils/value.h>

#include <algorithm>
#include <iostream>

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

Snail *Snails::intersectingSnails(const vec2 &start, const vec2 &end,
                                  float radius, Snail *ignore, vec2 &hitpos) {

  for (SnailVector::iterator i = snails.begin(); i != snails.end(); ++i) {
    Snail *snail = *i;
    if (snail != ignore) {
      if (snail->intersects(start, end, radius, hitpos)) {
        return snail;
      }
    }
  }
  
  return 0;
}


Snail::Snail(const vec2 &initialPos, int id, SystemContext *ctx)
  : _position(initialPos)
  , originalPos(initialPos)
  , id(id)
  , context(ctx)
  , radius(29.0f)
  , vel(0.0f, 0.0f)
  , takingControl(false)
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
  roundedPos.x = round(_position.x);
  roundedPos.y = round(_position.y);

  graphics->drawQuad(rect(roundedPos, 64, 64));
//   graphics->disableTextures();
//   graphics->drawCircle(roundedPos, radius, 18);
}

void Snail::update(double dt) {
  secondsSinceFire += dt;
  
  if (_state[STATE_MOVE_UP])
    _position += vec2(0.0f, -500.0f) * dt;
  if (_state[STATE_MOVE_DOWN])
    _position += vec2(0.0f, 500.0f) * dt;

  _position += vel * dt;
  vel = vel * 0.99f;

  if (vel.magnitude() <= 20.0f) {
    takingControl = true;
  }

  if (takingControl) {
    const vec2 wantedPos(originalPos.x, _position.y);

    vec2 diff = wantedPos - _position;
    if (diff.magnitude() < 1.0f) {
      takingControl = false;
      vel = vec2::Zero();
    }
    else {
      vel += diff.normalize() * 2000.0f * dt;
    }
  }
  
  _position.y = clamp(_position.y, 32.0f, 600.0f - 32.0f);
  _position.x = clamp(_position.x, 32.0f, 800.0f - 32.0f);
  
  if (_state[STATE_SHOOT]) {// FIXME: rename SHOOT to SHOOTING
    if (secondsSinceFire >= 0.2) {
      vec2 dir = (id == Snails::SNAIL_LEFT ? vec2(1.0f, 0.0f) : vec2(-1.0f, 0.0f));
      context->items()->spawnProjectile(Items::PROJECTILE_BULLET, _position + dir * 64.0f, dir, this);
      secondsSinceFire = 0.0;
    }
    
  }
}

void Snail::takeDamage(const vec2 &pos, float damage) {
  vel += (_position - pos) * damage;
  takingControl = false;
}

bool Snail::takeItem(const std::string &type, int amount) {
  std::cout << "snail: received " << amount
            << " amount of " << type << std::endl;
  
  return true; // we took it
}

bool Snail::intersects(const vec2 &start, const vec2 &end,
                       float radius, vec2 &hitpos) {
  vec2 closest = closest_point(start, end, _position);
  if ((_position - closest).magnitude() <= radius + this->radius) {
    hitpos = closest;
    return true;
  }
  
  return false;
}

const vec2 &Snail::position() const {
  return _position;
}
