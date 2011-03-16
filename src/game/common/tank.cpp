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


namespace {
const float MAX_DELTA_TIME = 1.0 / 30.0;

double wrap(double value, double lower, double upper) { // FIXME: util.
  double distance = upper - lower;
  double times = floor((value - lower) / distance);
  return value - (times * distance);
}

}


/* <--- begin network packing ---> */
TankState::TankState(const NetTankSnapshot &snapshot) {
  actor = ntohs(snapshot.id);
  pos.x = static_cast<float>(ntohs(snapshot.x)) / 60.0f;
  pos.y = static_cast<float>(ntohs(snapshot.y)) / 60.0f;
  base_dir = float(ntohs(snapshot.base_dir)) / 10.0;
  turret_dir = ntohs(snapshot.turret_dir);
}

TankState::operator NetTankSnapshot() const {
  NetTankSnapshot ret;
  ret.id = htons(actor);
  ret.x = htons(pos.x * 60.0f);
  ret.y = htons(pos.y * 60.0f);
  ret.base_dir = htons(base_dir * 10.0);
  ret.turret_dir = htons(turret_dir);

  return ret;
}
/* <--- end network packing ---> */


void TankState::advance(const PlayerInput &delta, double duration) {
  shooting = (delta.buttons & PlayerInput::STATE_SHOOTING);
  
  for (; duration >= MAX_DELTA_TIME; duration -= MAX_DELTA_TIME) {
    integrate(delta, MAX_DELTA_TIME);
  }

  // integrate any leftovers
  integrate(delta, duration);
}

void TankState::integrate(const PlayerInput &delta, double dt) {
  vec2 vDir = vec2::FromDegrees(base_dir);

  if (delta.buttons & PlayerInput::STATE_TURN_RIGHT) {
    base_dir += 140.0f * dt;
  }
  if (delta.buttons & PlayerInput::STATE_TURN_LEFT) {
    base_dir -= 140.0f * dt;
  }

  float speed = 0.0f;
  if (delta.buttons & PlayerInput::STATE_MOVE_UP) {
    speed = 200.0f; //std::min(_speed + 200.0f * dt, 150.0);
  }
  else if (delta.buttons & PlayerInput::STATE_MOVE_DOWN) {
    speed = -80.0f; //std::max(_speed - 80.0f * dt, -40.0);    
  }
  else {
    if (speed > 0.0)
      speed = 0.0f; //std::max(_speed -= 300.0f * dt, 0.0f);
    else
      speed = 0.0f; //std::min(_speed += 300.0f * dt, 0.0f);
  }

  pos += vDir * speed * dt;


  vec2 targetDiff = normalized(vec2(delta.aim_x, delta.aim_y) - pos);
  double targetDir = atan2(targetDiff.y, targetDiff.x) / M_PI * 180.0;
  double angle = wrap(targetDir - turret_dir, 0.0, 360.0);

  if (angle >= 180.0)
    angle -= 360.0;

  
  double add = 0.0;
  if (angle > 1.0) {
    add = std::min(200.0 * dt, angle);
    
  }
  else if (angle < 1.0f) {
    add = std::max(-200.0 * dt, angle);
  }

  turret_dir += add;
  //turret_dir += _rotSpeed * dt;

  base_dir = wrap(base_dir, 0.0, 360.0);
  turret_dir = wrap(turret_dir, 0.0, 360.0);
}


Tank::Tank(const SystemContext *ctx)
  : context(ctx)
{
  _reload_time = 0.0;
  _health = 100.0f;
  _radius = 15.0f;
  _alive = true;
}

void Tank::setTexture(Texture *texture, Texture *turret) {
  this->texture = texture;
  _turret = turret;
}

void Tank::render(Graphics *graphics) {
  graphics->setBlend(Graphics::BLEND_ALPHA);
  graphics->enableTextures();
  graphics->setColor(color4::White());
  texture->bind();

  vec2 roundedPos;
  roundedPos.x = round(_state.pos.x);
  roundedPos.y = round(_state.pos.y);

  graphics->drawQuad(rect(_state.pos, 16, 16), _state.base_dir);

  if (_turret) {
    _turret->bind();
    graphics->drawQuad(rect(_state.pos, 16, 16), _state.turret_dir);
  }
}

// FIXME: bool -> some enum
bool Tank::update(double dt) {
  if (!_alive) {
    return false;
  }
  
  _count += dt;
  if (_reload_time > 0.0) {
    _reload_time -= dt;
  }
  
  if (_state.shooting && _reload_time <= 0.0) {
    _reload_time = 1.0;
    shoot();
  }

  return _alive;
}

void Tank::shoot() {
  if (context->isServer()) {
    std::cout << "shoot" << std::endl;
    vec2 turret_diff = vec2::FromDegrees(_state.turret_dir);
    context->projectiles()->spawnProjectile(Projectiles::PROJECTILE_BULLET,
                                            _state.pos + turret_diff,
                                            _state.turret_dir,
                                            id());
  }
}

void Tank::die() {
  std::cout << "I died." << std::endl;
  _alive = false;
}

void Tank::takeDamage(const vec2 &pos, float damage) {
  _health -= damage;

  if (_health <= 0.0f) {
    die();
  }
}

const TankState &Tank::state() const {
  return _state;
}

void Tank::assign(const TankState &new_state) {
  _state = new_state;
}


void Tank::resetCount(double time) {
  _count = time;
}

double Tank::count() const {
  return _count;
}


bool Tank::intersects(const vec2 &start, const vec2 &end,
                       float radius, vec2 &hitpos) const {
  vec2 closest = closest_point(start, end, _state.pos);
  if (length(_state.pos - closest) <= radius + _radius) {
    hitpos = closest;
    return true;
  }
  
  return false;
}

