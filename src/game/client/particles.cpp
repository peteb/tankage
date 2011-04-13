#include <game/client/particles.h>
#include <platform/portal.h>
#include <platform/graphics.h>
#include <platform/window_manager.h>
#include <platform/texture.h>
#include <utils/key.h>
#include <algorithm>
#include <utility>

Particles::Particles(const Portal &modules) {
  gfx = modules.requestInterface<Graphics>();
  wm = modules.requestInterface<WindowManager>();
  
  lastUpdate = wm->timeSeconds();
}

void Particles::render() {
  double thisUpdate = wm->timeSeconds();
  double dt = thisUpdate - lastUpdate;
  lastUpdate = thisUpdate;

  for (GroupList::iterator i = groups.begin(), e = groups.end();
       i != e; ++i) {
    (*i).second->render(dt, gfx);
  }
}

ParticleGroup *Particles::group(class Texture *texture) {
  GroupList::iterator iter = std::lower_bound(
    groups.begin(), groups.end(), texture, key_cmp());

  if (iter != groups.end())
    return (*iter).second;

  // Create the particle group
  ParticleGroup *newGroup = new ParticleGroup(texture);
  groups.push_back(std::make_pair(texture, newGroup));
  std::sort(groups.begin(), groups.end(), key_cmp());
  
  return newGroup;
}


ParticleGroup::ParticleGroup(class Texture *texture)
  : texture(texture)
{
}

void ParticleGroup::render(double dt, Graphics *gfx) {
  if (!texture)
    return;

  texture->bind();
  gfx->enableTextures();
  gfx->setBlend(Graphics::BLEND_ALPHA);
  
  ParticleList::iterator iter = particles.begin(), e = particles.end();
  
  for (;iter != e;) {
    if (iter->update(dt)) {
      gfx->setColor(iter->color);
      gfx->drawQuad(rect(iter->pos, 16, 4), 0.0f); // FIXME: this is definitely slow,
                                             // should batch!
      ++iter;
    }
    else {
      *iter = *(--e);
    }
  }

  particles.erase(e, particles.end());
}

void ParticleGroup::addParticle(const Particle &particle) {
  particles.push_back(particle);
}

Particle::Particle() {
  color = color4::White();
}

bool Particle::update(double dt) {
  ttd -= dt;
  color.a = clamp(ttd, 0.0, 0.4);
  return ttd > 0.0;
}
