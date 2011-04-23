#include "map.h"
#include <game/client/gameclient.h>
#include <platform/portal.h>
#include <platform/graphics.h>
#include <utils/packer.h>
#include <utils/log.h>
#include <algorithm>

ClientMap::ClientMap(Portal &services, GameClient *gameclient) 
  : _gameclient(gameclient)
{
  _gfx = services.requestInterface<Graphics>();
  std::fill(_data, _data + 64*64, 0);
}

void ClientMap::addChunk(Unpacker &msg) {
  std::pair<const char *, size_t> chunk = msg.readData();
  Log(DEBUG) << "got map chunk containing " << chunk.second << " tiles";
  
  std::copy(chunk.first, chunk.first + chunk.second, _data);
}

void ClientMap::render() {
  _gfx->disableTextures();
  _gfx->setBlend(Graphics::BLEND_NONE);
  
  for (int y = 0; y < 64; ++y) {
    for (int x = 0; x < 64; ++x) {
      char tile_val = _data[y*64+x];
      char tile_damage = (tile_val & 0xF0) >> 4;
      float dmg_scale = float(tile_damage) / 16.0f;
      dmg_scale = lerp(0.0f, 0.8f, dmg_scale);
      
      color4 grass_color = color4(0.42, 0.54f, 0.33f, 1.0f);
      color4 tile_color;
      vec2 scale(1.0f, 1.0f);
      
      switch (tile_val & 0x0F) {
      case 0: // grass
        tile_color = grass_color;
        break;
          
      case 1: // rock
        tile_color = color4(0.7, 0.7f, 0.7f, 1.0f);
        break;
          
      case 3: // kasparium
        tile_color = color4(0.3, 0.3, 0.4, 1.0f);
        break;  
          
      case 5:
        tile_color = color4(0.3, 0.3, 0.6, 1.0f);
        tile_color.b = sin(_gameclient->localTime() * 2.0 * M_PI) * 0.1 + 0.6;
        //scale.x = sin(_gameclient->localTime() * 360.0) * 0.1 + 1.1;
        //scale.y = scale.x;
        break;
      }

      tile_color = lerp(tile_color, grass_color, dmg_scale);
      tile_color.a = 1.0f;
      
      vec2 pos((x - 32) * 32, (y - 32) * 32);
      _gfx->setColor(tile_color);
      _gfx->drawQuad(rect(pos, 16 * scale.x, 16 * scale.y), 0.0f);
    }
  }
}

void ClientMap::setTile(const std::pair<int, int> &tile, char state) {
  _data[tile.second*64 + tile.first] = state;
}
