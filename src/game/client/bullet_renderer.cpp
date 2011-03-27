#include "bullet_renderer.h"
#include <game/client/gameclient.h>
#include <engine/portal.h>
#include <engine/graphics.h>
#include <engine/texture.h>

BulletRenderer::BulletRenderer(GameClient *client, Portal &services)
  : _client(client)
{
  _gfx = services.requestInterface<Graphics>();
  _bullet_texture = _client->textureLoader().texture("ptr_bullet.png");
  _bullet_texture->setFiltering(true);
}

void BulletRenderer::render() {
  BulletSnapshot::const_iterator it = _current.begin(), it_e = _current.end();
  
  _gfx->setColor(color4::White());
  _gfx->enableTextures();
  _gfx->setBlend(Graphics::BLEND_ALPHA);
  _gfx->setTexture(_bullet_texture);
  
  for (; it != it_e; ++it) {
    const Bullet::State &state = *it;
    vec2 pos = state.positionAt(_current.tick(), _client->sinceSnap(), _client->tickDuration());
    pos.x = round(pos.x);
    pos.y = round(pos.y);
    _gfx->drawQuad(rect(pos, 4, 4), state.dir);
  }  
}

void BulletRenderer::addSnapshot(const Snapshot<Bullet::State> &snapshot) {
  _current = snapshot;
}
