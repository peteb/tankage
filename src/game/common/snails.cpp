#include <game/common/snails.h>
#include <game/common/items.h>
#include <game/common/net_protocol.h>

#include <engine/graphics.h>
#include <engine/texture.h>
#include <engine/image_loader.h>
#include <engine/window_manager.h>
#include <engine/portal.h>
#include <engine/image.h>
#include <engine/config.h>
#include <engine/packet.h>
#include <engine/network.h>

#include <utils/rect.h>
#include <utils/value.h>
#include <utils/color.h>
#include <utils/algorithm.h>


#include <algorithm>
#include <iostream>
#include <memory>

Snails::~Snails() {
  // delete all the snails when game terminates
  std::for_each(snails.begin(), snails.end(), delete_op());
}

void Snails::init(const class Portal &interfaces) {
  graphics = interfaces.requestInterface<Graphics>();
  wm = interfaces.requestInterface<WindowManager>();
  ImageLoader *imgLoader = interfaces.requestInterface<ImageLoader>();

  // First snail
  {
    std::auto_ptr<Image> img(imgLoader->loadImage(std::string(RESOURCE_PATH) + "/snail_l.png"));
    Snail *snail = new Snail(vec2(50.0f, 300.0f), Snails::SNAIL_LEFT, context);
    snail->setTexture(graphics->createTexture(img.get()));
  
    snails.push_back(snail);
  }

  // Second snail
  {
    std::auto_ptr<Image> img(imgLoader->loadImage(std::string(RESOURCE_PATH) + "/snail_r.png"));
    Snail *snail = new Snail(vec2(800-50.0f, 300.0f), Snails::SNAIL_RIGHT, context);
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
    if ((*i)->update(dt)) {
      // It's alive!
      (*i)->render(graphics);
    }
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

/*
 * Snails subsystem network use cases:
 * - Send full state on new connection
 * - Send events when snails die, shoot, etc.
 * - Frequent updates of positions, etc.
 *
 * Fixme: ReplicatedSystem::onIdent(Client *), then send own packet
 *        ReplicatedSystem::onTick(Client *)
 * No don't do this. 
 */


/*
 * channels[EVENTS].writeU8(blabla)    // will now receive a onReceive(EVENTS, blabla)
 * channels[ABS].write() // will now receive onReceive(ABS, bla)
 */



void Snails::onTick(class Client *client) {
  
}

void Snails::onReceive(NetPacketType type, const Packet &packet) {
  std::cout << "received packet " << (int)type << std::endl;
}


Snail::Snail(const vec2 &initialPos, int id, const SystemContext *ctx)
  : _position(initialPos)
  , originalPos(initialPos)
  , id(id)
  , context(ctx)
  , radius(29.0f)
  , vel(0.0f, 0.0f)
  , takingControl(false)
{
  std::fill(_state, _state + STATE_MAX, 0);
  secondsSinceFire = 0.0;
  health = 100;
}

Snail::~Snail() {
  delete this->texture;
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
  graphics->setColor(color4::White());
  texture->bind();

  vec2 roundedPos;
  roundedPos.x = round(_position.x);
  roundedPos.y = round(_position.y);

  graphics->drawQuad(rect(roundedPos, 32, 32));
//   graphics->disableTextures();
//   graphics->drawCircle(roundedPos, radius, 18);
}

bool Snail::update(double dt) {
  if (health <= 0) {
    std::cout << "snail: I'm dead :( returning false" << std::endl;
    return false;
  }
  
  secondsSinceFire += dt;
  
  if (_state[STATE_MOVE_UP])
    _position += vec2(0.0f, -500.0f) * dt;
  if (_state[STATE_MOVE_DOWN])
    _position += vec2(0.0f, 500.0f) * dt;

  _position += vel * dt;
  vel -= vec2(vel.x * 0.5, vel.y) * dt * 4.5;

  if (vel.magnitude() < 90.0) {
    takingControl = true;
  }

  if (takingControl) {
    const vec2 wantedPos(originalPos.x, _position.y);

    vec2 diff = _position - wantedPos;
    if (diff.x < 1.0f) {
      takingControl = false;

    }
    else {
      vel -= diff.normalize() * dt * 400.0;
    }
  }

  // TODO: add proper euler integration
  
  _position.y = clamp(_position.y, 32.0f, 600.0f - 32.0f);
  _position.x = clamp(_position.x, 32.0f, 800.0f - 32.0f);
  _position.x = std::max(_position.x, originalPos.x);
  
  if (_state[STATE_SHOOT]) {// FIXME: rename SHOOT to SHOOTING
    if (secondsSinceFire >= 0.2) {
      vec2 dir = (id == Snails::SNAIL_LEFT ? vec2(1.0f, 0.0f) : vec2(-1.0f, 0.0f));
      context->items()->spawnProjectile(Items::PROJECTILE_BULLET, _position + dir * 64.0f, dir, this);
      secondsSinceFire = 0.0;
    }
    
  }

  return true;
}

void Snail::takeDamage(const vec2 &pos, float damage) {
  vel += (_position - pos).normalize()  * damage * 10.0f;
  std::cout << std::string((_position - pos).normalize()) << std::endl;
  
  takingControl = false;
  health -= static_cast<int>(damage);
  std::cout << "snail: I received " << damage
            << " amount of hurt >:-/" << std::endl;
}

bool Snail::takeItem(const std::string &type, int amount) {
  std::cout << "snail: I received " << amount
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
