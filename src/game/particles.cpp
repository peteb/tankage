#include <game/particles.h>
#include <engine/portal.h>
#include <engine/graphics.h>

void Particles::init(const Portal &modules) {
  gfx = modules.requestInterface<Graphics>();
}

void Particles::render() {
  
}
// TODO: fix this, and TextureLoader
