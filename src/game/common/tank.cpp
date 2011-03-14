#include <netinet/in.h>
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
  //std::fill(_state, _state + STATE_MAX, 0);
  secondsSinceFire = 0.0;
  //sinceSnap = 0.0;
  health = 100;
  _dir = 0.0f;
  _speed = 0.0f;
  _rotSpeed = 0.0f;
  _turretDir = 0.0f;
  
  resetCount();
  //sinceHistory = 0.0;
}

Tank::~Tank() {
}

void Tank::setTexture(Texture *texture, Texture *turret) {
  this->texture = texture;
  _turret = turret;
}

void Tank::assign(const Tank::State &state) {
  _position = state.pos;
  _dir = state.base_dir;
  _turretDir = state.turret_dir;
}

Tank::State Tank::snapshot() const {
  Tank::State state;
  state.actor = _id;
  state.pos = _position; 
  state.base_dir = _dir;
  state.turret_dir = _turretDir;
  
  return state;
}

void Tank::render(Graphics *graphics) {
  graphics->setBlend(Graphics::BLEND_ALPHA);
  graphics->enableTextures();
  graphics->setColor(color4::White());
  texture->bind();

  vec2 roundedPos;
  roundedPos.x = round(_position.x);
  roundedPos.y = round(_position.y);

  graphics->drawQuad(rect(_position, 16, 16), _dir);

  if (_turret) {
    _turret->bind();
    graphics->drawQuad(rect(_position, 16, 16), _turretDir);
  }
}

Tank::State::State(const NetTankSnapshot &snapshot) {
  actor = ntohs(snapshot.id);
  pos.x = static_cast<float>(ntohs(snapshot.x)) / 60.0f;
  pos.y = static_cast<float>(ntohs(snapshot.y)) / 60.0f;
  base_dir = float(ntohs(snapshot.base_dir)) / 10.0;
  turret_dir = ntohs(snapshot.turret_dir);
}

Tank::State::operator NetTankSnapshot() const {
  NetTankSnapshot ret;
  ret.id = htons(actor);
  ret.x = htons(pos.x * 60.0f);
  ret.y = htons(pos.y * 60.0f);
  ret.base_dir = htons(base_dir * 10.0);
  ret.turret_dir = htons(turret_dir);

  return ret;
}



double Wrap(double value, double lower, double upper) { // FIXME: util.
  double distance = upper - lower;
  double times = floor((value - lower) / distance);
  return value - (times * distance);
}


bool Tank::advance(const Input &delta, double time) {
  _count += time;
  if (health <= 0) {
    return false;
  }

  secondsSinceFire += time;

  vec2 vDir = vec2::FromDegrees(_dir);

  if (delta.buttons & STATE_TURN_RIGHT) {
    _dir += 140.0f * time;
  }
  if (delta.buttons & STATE_TURN_LEFT) {
    _dir -= 140.0f * time;
  }
  
  if (delta.buttons & STATE_MOVE_UP) {
    _speed = 200.0f; //std::min(_speed + 200.0f * time, 150.0);
  }
  else if (delta.buttons & STATE_MOVE_DOWN) {
    _speed = -80.0f; //std::max(_speed - 80.0f * time, -40.0);    
  }
  else {
    if (_speed > 0.0)
      _speed = 0.0f; //std::max(_speed -= 300.0f * time, 0.0f);
    else
      _speed = 0.0f; //std::min(_speed += 300.0f * time, 0.0f);
  }

  _position += vDir * _speed * time;


  vec2 targetDiff = normalized(vec2(delta.aim_x, delta.aim_y) - _position);
  double targetDir = atan2(targetDiff.y, targetDiff.x) / M_PI * 180.0;
  double angle = Wrap(targetDir - _turretDir, 0.0, 360.0);

  if (angle >= 180.0)
    angle -= 360.0;

  
  double add = 0.0;
  if (angle > 1.0) {
    add = std::min(200.0 * time, angle);
    
  }
  else if (angle < 1.0f) {
    add = std::max(-200.0 * time, angle);
  }

  _turretDir += add;
  _turretDir += _rotSpeed * time;

  _dir = Wrap(_dir, 0.0, 360.0);
  _turretDir = Wrap(_turretDir, 0.0, 360.0);
  
/*  if (delta.buttons & STATE_SHOOT) {// FIXME: rename SHOOT to SHOOTING
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
    
    }*/

  return true;
}

void Tank::resetCount(double time) {
  _count = time;
}

double Tank::count() const {
  return _count;
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
