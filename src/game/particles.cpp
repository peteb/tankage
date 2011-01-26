#include <game/particles.h>
#include <engine/portal.h>
#include <engine/graphics.h>
#include <engine/window_manager.h>
#include <utils/key.h>
#include <algorithm>
#include <utility>

void Particles::init(const Portal &modules) {
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

}

