#include <game/common/tank.h>
#include <game/common/projectiles.h>
#include <game/common/net_protocol.h>
#include <game/common/players.h>

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

#include <arpa/inet.h>

#include <algorithm>
#include <iostream>
#include <memory>
#include <cmath>


Tank::Tank(ActorId id, const SystemContext *ctx)
  : context(ctx)
  , radius(29.0f)
  , _id(id)
{
  std::fill(_state, _state + STATE_MAX, 0);
  secondsSinceFire = 0.0;
  sinceSnap = 0.0;
  health = 100;
  _dir = 0.0f;
  _speed = 0.0f;
  _rotSpeed = 0.0f;
  _turretDir = 0.0f;
  _snapshotted = false;
}

Tank::~Tank() {
}

void Tank::startState(State state) {
  _state[state] = true;
}

void Tank::stopState(State state) {
  _state[state] = false;
}

void Tank::setTexture(Texture *texture, Texture *turret) {
  this->texture = texture;
  _turret = turret;
}

void Tank::onSnap(const NetTankSnapshot &netshot) {
  // Convert byte order
  NetTankSnapshot snapshot;
  snapshot.x = ntohs(netshot.x);
  snapshot.y = ntohs(netshot.y);
  snapshot.base_dir = ntohs(netshot.base_dir);
  snapshot.turret_dir = ntohs(netshot.turret_dir);

  snapshots[1] = snapshots[0];
  snapshots[0] = snapshot;
  
  sinceSnap = 0.0;
  _snapshotted = true;
}

NetTankSnapshot Tank::snapshot() const {
  NetTankSnapshot snap;
  snap.id = htons(_id);
  snap.x = htons(_position.x);
  snap.y = htons(_position.y);
  snap.base_dir = htons(_dir); //(360.0 + _dir) * 4.0);
  snap.turret_dir = htons(_turretDir);
  
  return snap;
}

void Tank::render(Graphics *graphics) {
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

double Wrap(double value, double lower, double upper) {
  double distance = upper - lower;
  double times = floor((value - lower) / distance);
  return value - (times * distance);
}


bool Tank::update(double dt) {
  if (health <= 0) {
    std::cout << "snail: I'm dead :( returning false" << std::endl;
    return false;
  }

//  _position.y = double(snapshots[0].y + double(snapshots[0].y - snapshots[1].y) * sinceSnap / 25.0);
  //_position.y = snapshots[0].y + double(snapshots[0].y - snapshots[1].y) * sinceSnap / (1.0/25.0);
  // _position.x = snapshots[0].x + double(snapshots[0].x - snapshots[1].x) * sinceSnap / (1.0/25.0);
  //if (_id != context->players()->localPlayer()) {
    // Only update tank if it's a remote player
    if (_snapshotted) {
      vec2 lPos;
      lPos.x = lerp<double, double>(snapshots[0].x, snapshots[1].x, sinceSnap / (1.0/25.0));
      lPos.y = lerp<double, double>(snapshots[0].y, snapshots[1].y, sinceSnap / (1.0/25.0));
      _turretDir = snapshots[0].turret_dir;
      _dir = (static_cast<double>(snapshots[0].base_dir)); // / 4.0) - 360.0;

      if (length(lPos - _position) > 1.0) {
        _position = lPos;
      }
    }
    // }

  // FIXME: improve the jerkiness. Probably enet doesn't throw away old packets?
  
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

  vec2 targetDiff = normalized(cursorPos - _position);
  double targetDir = atan2(targetDiff.y, targetDiff.x) / M_PI * 180.0;
  double angle = Wrap(targetDir - _turretDir, 0.0, 360.0);

  if (angle >= 180.0)
    angle -= 360.0;

  
  double add = 0.0;
  if (angle > 1.0) {
    add = std::min(200.0 * dt, angle);
    
  }
  else if (angle < 1.0f) {
    add = std::max(-200.0 * dt, angle);
  }

  _turretDir += add;
  _turretDir += _rotSpeed * dt;

  _dir = Wrap(_dir, 0.0, 360.0);
  _turretDir = Wrap(_turretDir, 0.0, 360.0);
  
  if (_state[STATE_SHOOT]) {// FIXME: rename SHOOT to SHOOTING
    if (secondsSinceFire >= 0.2) {
      vec2 dir = vec2::FromDirection(_turretDir);

      if (!_snapshotted) {
        context->projectiles()->spawnProjectile(
          Projectiles::PROJECTILE_BULLET,
          _position + dir * 32.0f,
          _turretDir,
          _id);
      }
      
      secondsSinceFire = 0.0;
    }
    
  }

  return true;
}

void Tank::setCursor(const vec2 & pos) {
  cursorPos = pos;
}

void Tank::takeDamage(const vec2 &pos, float damage) {
  vel += normalized(_position - pos)  * damage * 10.0f;
  std::cout << std::string(normalized(_position - pos)) << std::endl;
  
  health -= static_cast<int>(damage);
  std::cout << "snail: I received " << damage
            << " amount of hurt >:-/" << std::endl;
}

bool Tank::takeItem(const std::string &type, int amount) {
  std::cout << "snail: I received " << amount
            << " amount of " << type << std::endl;
  
  return true; // we took it
}

bool Tank::intersects(const vec2 &start, const vec2 &end,
                       float radius, vec2 &hitpos) const {
  vec2 closest = closest_point(start, end, _position);
  if (length(_position - closest) <= radius + this->radius) {
    hitpos = closest;
    return true;
  }
  
  return false;
}

const vec2 &Tank::position() const {
  return _position;
}

const ActorId Tank::id() const {
  return _id;
}
