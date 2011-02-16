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
#include <cmath>

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
    std::auto_ptr<Image> img(imgLoader->loadImage(std::string(RESOURCE_PATH) + "/tank_base.png"));
    std::auto_ptr<Image> img2(imgLoader->loadImage(std::string(RESOURCE_PATH) + "/tank_turret.png"));
    Snail *snail = new Snail(vec2(50.0f, 300.0f), Snails::SNAIL_LEFT, context);
    snail->setTexture(graphics->createTexture(img.get()),
                      graphics->createTexture(img2.get()));
  
    snails.push_back(snail);
  }

  // Second snail
  {
    std::auto_ptr<Image> img(imgLoader->loadImage(std::string(RESOURCE_PATH) + "/snail_r.png"));
    Snail *snail = new Snail(vec2(800-50.0f, 300.0f), Snails::SNAIL_RIGHT, context);
    snail->setTexture(graphics->createTexture(img.get()), NULL);
  
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


void Snails::onTick(class Client *client) {
  NetSnailsSnapMsg msg;
  msg.type = NET_SNAILS_SNAPSHOT;
  
  for (size_t i = 0; i < snails.size() && i < 2; ++i) {
    NetSnailSnapshot *snap = &msg.snaps[i];
    snap->x = htons(snails[i]->position().x);
    snap->y = htons(snails[i]->position().y);
  }

  // FIXME: make sure old versions will be thrown away by the connection layer
  client->send(&msg, sizeof(NetSnailsSnapMsg), 0, NET_CHANNEL_ABS);
}

void Snails::onReceive(NetPacketType type, const Packet &packet) {
  if (type == NET_SNAILS_SNAPSHOT) {
    const NetSnailsSnapMsg *msg =
      static_cast<const NetSnailsSnapMsg *>(packet.data());

    for (size_t i = 0; i < 2; ++i) {
      NetSnailSnapshot snap = msg->snaps[i];
      snap.x = ntohs(snap.x); // FIXME: maybe there should be a multiplier here
      snap.y = ntohs(snap.y);

      // FIXME: forward this update to the snails
      std::cout << "snail pos: " << snap.x << ":" << snap.y << std::endl;
      snails[i]->onSnap(snap);
    }
    
  }
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
  sinceSnap = 0.0;
  health = 100;
  _dir = 0.0f;
  _speed = 0.0f;
  _rotSpeed = 0.0f;
  _turretDir = 0.0f;
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

void Snail::setTexture(Texture *texture, Texture *turret) {
  this->texture = texture;
  _turret = turret;
}

void Snail::onSnap(NetSnailSnapshot &snapshot) {
  snapshots[1] = snapshots[0];
  snapshots[0] = snapshot;
  sinceSnap = 0.0;
//  _position.y = snapshot.y;
}

void Snail::render(Graphics *graphics) {
  
  graphics->setBlend(Graphics::BLEND_ALPHA);
  graphics->enableTextures();
  graphics->setColor(color4::White());
  texture->bind();

  vec2 roundedPos;
  roundedPos.x = round(_position.x);
  roundedPos.y = round(_position.y);

  graphics->drawQuad(rect(roundedPos, 16, 16), _dir);

  if (_turret) {
    _turret->bind();
    graphics->drawQuad(rect(roundedPos, 16, 16), _turretDir);
  }
  
//   graphics->disableTextures();
//   graphics->drawCircle(roundedPos, radius, 18);
}

bool Snail::update(double dt) {
  if (health <= 0) {
    std::cout << "snail: I'm dead :( returning false" << std::endl;
    return false;
  }

//  _position.y = double(snapshots[0].y + double(snapshots[0].y - snapshots[1].y) * sinceSnap / 25.0);
//  _position.y = snapshots[0].y + double(snapshots[0].y - snapshots[1].y) * sinceSnap / (1.0/25.0);
  
  sinceSnap += dt;
  secondsSinceFire += dt;

  vec2 vDir(cos(_dir / 180.0f * M_PI), sin(_dir / 180.0f * M_PI));
  
  if (_state[STATE_MOVE_UP]) {
    _speed = std::min(_speed + 200.0f * dt, 150.0);
  }
  else if (_state[STATE_MOVE_DOWN]) {
    _speed = std::max(_speed - 80.0f * dt, -40.0);    
  }
  else {
    if (_speed > 0.0)
      _speed = std::max(_speed -= 300.0f * dt, 0.0f);
    else
      _speed = std::min(_speed += 300.0f * dt, 0.0f);
  }

  _position += vDir * _speed * dt;

  if (_state[STATE_TURN_RIGHT]) {
    _rotSpeed = std::min(_rotSpeed + 800.0f * dt, 120.0);
    if (_speed > 0.0)
      _speed -= 140.0 * dt;
  }
  else if (_state[STATE_TURN_LEFT]) {
    _rotSpeed = std::max(_rotSpeed - 800.0f * dt, -120.0);    
    if (_speed > 0.0)
      _speed -= 140.0 * dt;
  }
  else {
    if (_rotSpeed > 0.0)
      _rotSpeed = std::max(_rotSpeed -= 800.0f * dt, 0.0f);
    else
      _rotSpeed = std::min(_rotSpeed += 800.0f * dt, 0.0f);
  }

  _dir += _rotSpeed * dt;
  _turretDir += _rotSpeed * dt;

  vec2 targetDiff = cursorPos - _position;
  targetDiff.normalize();
  double targetDir = -atan2(targetDiff.x, targetDiff.y) / M_PI * 180.0 + 90.0;

  if (abs(targetDir - _turretDir) > 180.0) {
    if (targetDir > _turretDir)
      _turretDir -= 200.0 * dt;
    else if (targetDir < _turretDir)
      _turretDir += 200.0 * dt;

  }
  else {
    if (targetDir > _turretDir)
      _turretDir += 200.0 * dt;
    else if (targetDir < _turretDir)
      _turretDir -= 200.0 * dt;
  }

  
  if (_turretDir > 360.0)
    _turretDir -= 360.0;

  if (_turretDir < -360.0)
    _turretDir += 360.0;
  
/*  if (_state[STATE_SHOOT]) {// FIXME: rename SHOOT to SHOOTING
    if (secondsSinceFire >= 0.2) {
      vec2 dir = (id == Snails::SNAIL_LEFT ? vec2(1.0f, 0.0f) : vec2(-1.0f, 0.0f));
      context->items()->spawnProjectile(Items::PROJECTILE_BULLET, _position + dir * 64.0f, dir, this);
      secondsSinceFire = 0.0;
    }
    
    }*/

  return true;
}

void Snail::setCursor(const vec2 & pos) {
  cursorPos = pos;
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
