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

#include <Box2D/Box2D.h>


Map::Map(GameServer *gameserver, b2World *world) 
  : _gameserver(gameserver)
  , _world(world)
  , _page(world)
{
  _page.load();
}

void Map::snap(Packer &msg, ClientSession *client) {
  // keep position of last snap, if the new position creates another snapset
  // than the last snap, snap! otherwise, just return.
 
  if (client->sent_map)
    return;
  
  Log(DEBUG) << "sending mapchunk to " << client->tankid;
  _page.snap(msg);
  client->sent_map = true;
}

bool Map::intersectSolid(const vec2 &start, const vec2 &end, 
                         vec2 &point, TileCoord &hit_tile) {  
  return _page.intersectSolid(start, end, point, hit_tile);
}

void Map::damageTile(const TileCoord &tile) {
  Log(DEBUG) << "removing " << tile.first << ", " << tile.second;

  _page.tileAt(tile) = 1;
  _page.refresh(tile);
  _gameserver->events().spawnTileUpdate(tile.first, tile.second, 1, 
                                        vec2((tile.first - 16) * 32, 
                                             (tile.second - 16) * 32));
}

