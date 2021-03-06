#include "resource_renderer.h"

#include <game/client/gameclient.h>
#include <platform/portal.h>
#include <platform/graphics.h>
#include <platform/texture.h>

#include <utils/log.h>

ResourceRenderer::ResourceRenderer(class GameClient *client, Portal &services)
  : _client(client)
{
  _gfx = services.requestInterface<Graphics>();
  _texture = _client->textureLoader().texture("kasparium.png");
  _texture->setFiltering(false);
}

void ResourceRenderer::render() {
  _gfx->enableTextures();
  _gfx->setBlend(Graphics::BLEND_ALPHA);
  _gfx->setTexture(_texture);
  
  ResourceSnapshot::const_iterator it = _last.begin(), it_e = _last.end();
  
  for (; it != it_e; ++it) {
    const Resource::State &state = *it;
    Snapshot<Resource::State>::const_iterator current = _current.find(state.id);
    
    if (current != _current.end()) {
      double time = (_last.tick() - state.start_tick) * _client->tickDuration() 
                     + _client->sinceSnap();
      color4 color(1.0f, 1.0f, 1.0f, clamp(time, 0.0, 1.0));
      vec2 pos = state.pos;
      pos.x = floor(pos.x);
      pos.y = floor(pos.y);
      color.a = clamp(1.0 + sin(time * 2.0 * 3.1415926535) * 0.5, 0.0, 1.0);
      color.a *= clamp(time, 0.0, 1.0);
      
      _gfx->setColor(color);      
      _gfx->drawQuad(rect(state.pos, 8, 8), time * 180.0);        
    }
  }  
}

void ResourceRenderer::addSnapshot(const Snapshot<Resource::State> &snapshot) {
  _last = _current;
  _current = snapshot;
}
