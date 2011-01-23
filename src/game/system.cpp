#include <game/system.h>
#include <game/snails.h>
#include <game/background.h>
#include <game/control.h>
#include <game/items.h>
#include <stdexcept>

SystemContext::SystemContext() {
  ready = false;
  std::fill(systems, systems + SYSTEM_MAX, (System *)0);
}

void SystemContext::set(unsigned id, System *system) {
  if (systems[id])
    throw std::runtime_error("system already set");

  systems[id] = system;
}

void SystemContext::init(class Portal &modules) {
  if (ready)
    throw std::runtime_error("sysctx already initialized");

  for (unsigned i = 0; i < SYSTEM_MAX; ++i) {
    if (systems[i]) {
      systems[i]->context = this;
      systems[i]->init(modules);
    }    
  }
  
  ready = true;
}

class System *SystemContext::resolveSystem(unsigned id) const {
  if (!systems[id])
    throw std::runtime_error("system not set");
  
  if (!ready)
    throw std::runtime_error("system context not ready");
  
  return systems[id];  
}
