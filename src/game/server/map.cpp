#include "map.h"
#include <game/server/client_session.h>
#include <game/common/net_protocol.h>
#include <utils/packer.h>
#include <utils/log.h>
#include <utils/vec.h>
#include <algorithm>
#include <utility>

Map::Map() {
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
  return std::make_pair(0, 0);
}

bool Map::intersectSolid(const vec2 &start, const vec2 &end, float radius) {
  vec2 ofs = start + vec2(16.0f * 32.0f, 16.0f * 32.0f);
  int tile_x = ofs.x / 32.0f;
  int tile_y = ofs.y / 32.0f;
  
  tile_x = clamp(tile_x, 0, 32);
  tile_y = clamp(tile_y, 0, 32);
  
  Log(DEBUG) << tile_x << " - " << tile_y << "(" << std::string(start) << ")";
  
  if (_data[tile_y * 32 + tile_x] == 0)
    return true;
  
  return false;
}