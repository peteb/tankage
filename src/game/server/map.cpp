#include "map.h"
#include <game/server/client_session.h>
#include <game/server/gameserver.h>
#include <game/common/net_protocol.h>
#include <utils/packer.h>
#include <utils/log.h>
#include <utils/vec.h>
#include <utils/algorithm.h>
#include <algorithm>
#include <utility>

Map::Map(GameServer *gameserver) 
  : _gameserver(gameserver)
{
  std::fill(_data, _data + 32*32, 0);
  for (int y = 0; y < 32; ++y) {
    for (int x = 0; x < 32; ++x) {
      if (y >= 8 && y <= 24) {
        if (x >= 8 && x <= 24) {
          _data[y*32+x] = 1;
        }
      }
    }
  }
}

void Map::snap(Packer &msg, ClientSession *client) {
  // keep position of last snap, if the new position creates another snapset
  // than the last snap, snap! otherwise, just return.
 
  if (client->sent_map)
    return;
  
  Log(DEBUG) << "sending mapchunk to " << client->tankid;
  msg.writeShort(NET_MAPCHUNK);
  msg.writeShort(32*32); // size of data
  msg.writeData(_data, 32*32);
  
  client->sent_map = true;
}

std::pair<int, int> VecTile(const vec2 &pos) {
  vec2 ofs = pos;
  int tile_x = ofs.x / 32.0f;
  int tile_y = ofs.y / 32.0f;
  
  tile_x = clamp(tile_x + 1, 0, 32);
  tile_y = clamp(tile_y + 1, 0, 32);
  return std::make_pair(tile_x, tile_y);
}

bool Map::intersectSolid(const vec2 &start, const vec2 &end, float radius, vec2 &point, std::pair<int, int> &hit_tile) {  
  vec2 start_ofs = start + vec2(16.0f * 32.0f, 16.0f * 32.0f);
  vec2 end_ofs = end + vec2(16.0f * 32.0f, 16.0f * 32.0f);
  double dt = 1.0 / length(end_ofs - start_ofs);
  double pos = 0.0;
  vec2 last_lerp = start_ofs;
  
  // FIXME: break this up at least, so we don't lerp each iteration
  // FIXME: this function should really be improved.
  // FIXME: also, support radius... <--- functional. important
  // FIXME: return Map::Collision
  
  while (pos <= 1.0) {
    vec2 lerp_pos = lerp(start_ofs, end_ofs, pos);
    std::pair<int, int> tile = VecTile(lerp_pos);

    if (_data[tile.second * 32 + tile.first] == 0) {
      point = last_lerp - vec2(16.0f * 32.0f, 16.0f * 32.0f);
      hit_tile = tile;
      return true;
    }
    
    last_lerp = lerp_pos;
    pos += dt;
  }
  
  return false;
}

void Map::damageTile(const std::pair<int, int> &tile) {
  _data[tile.second * 32 + tile.first] = 1;
  _gameserver->events().spawnTileUpdate(tile.first, tile.second, 1, 
                                        vec2((tile.first - 16) * 32, (tile.second - 16) * 32));
}
