#include "map.h"
#include <game/server/client_session.h>
#include <game/common/net_protocol.h>
#include <utils/packer.h>
#include <utils/log.h>
#include <algorithm>

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

