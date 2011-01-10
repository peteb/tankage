#include <game/system.h>
#include <game/snails.h>
#include <game/background.h>
#include <game/control.h>
#include <game/items.h>
#include <game/projectiles.h>
#include <stdexcept>

SystemContext::SystemContext() {
  ready = false;
}

void SystemContext::set(class Snails *snails) {
  _snails = snails;
}

void SystemContext::set(class Background *background) {
  _background = background;
}

void SystemContext::set(class Control *control) {
  _control = control;
}

void SystemContext::set(class Items *items) {
  _items = items;
}

void SystemContext::set(class Projectiles *projectiles) {
  _projectiles = projectiles;
}

class Snails *SystemContext::snails() const {
  if (!ready)
    throw std::runtime_error("snails system not initialized yet");

  return _snails;
}

class Background *SystemContext::background() const {
  if (!ready)
    throw std::runtime_error("background system not initialized yet");

  return _background;
}

class Control *SystemContext::control() const {
  if (!ready)
    throw std::runtime_error("control system not initialized yet");

  return _control;
}

class Items *SystemContext::items() const {
  if (!ready)
    throw std::runtime_error("items system not initialized yet");

  return _items;
}

class Projectiles *SystemContext::projectiles() const {
  if (!ready)
    throw std::runtime_error("projectiles system not initialized yet");

  return _projectiles;
}

void SystemContext::init() {
  if (ready)
    throw std::runtime_error("sysctx already initialized");

  _snails->init();
  _background->init();
  _control->init();
  _items->init();
  _projectiles->init();
  
  ready = true;
}

