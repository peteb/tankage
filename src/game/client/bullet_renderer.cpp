#include "bullet_renderer.h"
#include <game/client/gameclient.h>
#include <engine/portal.h>
#include <engine/graphics.h>
#include <engine/texture.h>
#include <math.h>

BulletRenderer::BulletRenderer(GameClient *client, Portal &services)
  : _client(client)
{
  _gfx = services.requestInterface<Graphics>();
  _bullet_texture = _client->textureLoader().texture("ptr_bullet.png");
  _bullet_texture->setFiltering(true);
}

void BulletRenderer::render() {
  _gfx->setColor(color4::White());
  _gfx->enableTextures();
  _gfx->setBlend(Graphics::BLEND_ALPHA);
  _gfx->setTexture(_bullet_texture);

  BulletSnapshot::const_iterator it = _last.begin(), it_e = _last.end();
  
  for (; it != it_e; ++it) {
    const Bullet::State &state = *it;
    Snapshot<Bullet::State>::const_iterator current = _current.find(state.id);
    
    if (current != _current.end()) {
      if (_client->sinceSnap() > current->max_lerp)
        continue;
      
      vec2 pos = state.positionAt(_last.tick(), _client->sinceSnap(), _client->tickDuration());
      pos.x = floor(pos.x);
      pos.y = floor(pos.y);
      _gfx->drawQuad(rect(pos, 4, 4), state.dir);
    }
  }  
}

void BulletRenderer::addSnapshot(const Snapshot<Bullet::State> &snapshot) {
  _last = _current;
  _current = snapshot;
}
