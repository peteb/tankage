#include "map.h"
#include <platform/portal.h>
#include <platform/graphics.h>
#include <utils/packer.h>
#include <utils/log.h>
#include <algorithm>

ClientMap::ClientMap(Portal &services) {
  _gfx = services.requestInterface<Graphics>();
  std::fill(_data, _data + 64*64, 0);
}

void ClientMap::addChunk(Unpacker &msg) {
  std::pair<const unsigned char *, size_t> chunk = msg.readData();
  Log(DEBUG) << "got map chunk containing " << chunk.second << " tiles";
  
  std::copy(chunk.first, chunk.first + chunk.second, _data);
}

void ClientMap::render() {
  _gfx->disableTextures();
  _gfx->setBlend(Graphics::BLEND_NONE);
  
  for (int y = 0; y < 64; ++y) {
    for (int x = 0; x < 64; ++x) {
      char tile_val = _data[y*64+x];

      if (tile_val)
        _gfx->setColor(color4(0.42, 0.54f, 0.33f, 1.0f));
      else
        _gfx->setColor(color4(0.7, 0.7f, 0.7f, 1.0f));
        
      vec2 pos((x - 32) * 32, (y - 32) * 32);
      _gfx->drawQuad(rect(pos, 16, 16), 0.0f);
    }
  }
}

void ClientMap::setTile(const std::pair<int, int> &tile, char state) {
  _data[tile.second*64 + tile.first] = state;
}
