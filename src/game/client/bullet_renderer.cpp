#include "bullet_renderer.h"
#include <game/client/gameclient.h>
#include <engine/portal.h>
#include <engine/graphics.h>

BulletRenderer::BulletRenderer(GameClient *client, Portal &services)
  : _client(client)
{
  _gfx = services.requestInterface<Graphics>();
  _bullet_texture = _client->textureLoader().texture("ptr_bullet.png");
}

void BulletRenderer::render() {
  
}

void BulletRenderer::addSnapshot(const Snapshot<Bullet::State> &snapshot) {
  prev = current;
  current = snapshot;
}
