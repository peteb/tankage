#include "map.h"
#include <engine/portal.h>
#include <engine/graphics.h>
#include <utils/packer.h>
#include <utils/log.h>
#include <algorithm>

ClientMap::ClientMap(Portal &services) {
  _gfx = services.requestInterface<Graphics>();
  std::fill(_data, _data + 32*32, 0);
}

void ClientMap::addChunk(Unpacker &msg) {
  short num_tiles = msg.readShort();
  Log(DEBUG) << "got map chunk containing " << num_tiles << " tiles";
  
  const char *tiles = reinterpret_cast<const char *>(msg.readData(num_tiles));
  std::copy(tiles, tiles + num_tiles, _data);
}

void ClientMap::render() {
  _gfx->disableTextures();
  _gfx->setBlend(Graphics::BLEND_NONE);
  
  for (int y = 0; y < 32; ++y) {
    for (int x = 0; x < 32; ++x) {
      char tile_val = _data[y*32+x];

      if (tile_val)
        _gfx->setColor(color4(0.42, 0.54f, 0.33f, 1.0f));
      else
        _gfx->setColor(color4(0.7, 0.7f, 0.7f, 1.0f));
        
      vec2 pos((x - 16) * 32, (y - 16) * 32);
      _gfx->drawQuad(rect(pos, 32, 32), 0.0f);
    }
  }
}