#include "tank_renderer.h"
#include <game/server/tank.h>
#include <iostream>
#include <game/client/gameclient.h>
#include <engine/portal.h>
#include <engine/graphics.h>

TankRenderer::TankRenderer(class GameClient *client)
  : _client(client)
{
  
}

void TankRenderer::init(const Portal &interfaces, const class SystemContext *context) {
  _context = context;
  _gfx = interfaces.requestInterface<Graphics>();
}

void TankRenderer::addSnapshot(const Snapshot<Tank::State> &snapshot) {
  prev = current;
  current = snapshot;
}

void TankRenderer::render() {
  TankSnapshot::const_iterator it = current.begin(), it_e = current.end();
  for (; it != it_e; ++it) {
    renderTank(*it);
  }
}

void TankRenderer::renderTank(const Tank::State &state) {
  _gfx->setColor(color4(0.0f, 0.0f, 1.0f, 1.0f));
  _gfx->disableTextures();
  _gfx->drawQuad(rect(state.pos, 16, 16), state.base_dir);
  
}