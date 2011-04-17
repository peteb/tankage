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

#include "Box2D/Box2D.h"

//namespace {
//inline TileCoord clamp(const TileCoord &val, const TileCoord &min, 
//                       const TileCoord &max) {
//  TileCoord ret;
//  ret.first = clamp(val.first, min.first, max.first);
//  ret.second = clamp(val.second, min.second, max.second);
//  return ret;
//}
//}

Map::Map(GameServer *gameserver, b2World &world) 
  : _gameserver(gameserver)
  , _world(world)
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
  
  
  b2BodyDef static_body_def;
  b2Body *static_body = _world.CreateBody(&static_body_def);
  b2PolygonShape tile_shape;
  
  for (int y = -16; y < 16; ++y) {
    for (int x = -16; x < 16; ++x) {
      char tile_type = at(TileCoord(x, y));
      if (tile_type == 0) {
        tile_shape.SetAsBox(0.5f, 0.5f, b2Vec2(x - 0.5f, y - 0.5f), 0.0f);
        
        /*b2FixtureDef fixture_def;
         fixture_def.shape = &tile_shape;
         fixture_def.density = 1.0f;
         fixture_def.friction = 0.3f;*/
        static_body->CreateFixture(&tile_shape, 1.0f);
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

TileCoord VecTile(const vec2 &pos) {
  vec2 ofs = pos;
  int tile_x = ofs.x / 32.0f - 16;
  int tile_y = ofs.y / 32.0f - 16;
  
  tile_x = clamp(tile_x + 1, -16, 15);
  tile_y = clamp(tile_y + 1, -16, 15);
  
  return TileCoord(tile_x, tile_y);
}

bool Map::intersectSolid(const vec2 &start, const vec2 &end, float radius, 
                         vec2 &point, TileCoord &hit_tile) {  
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
    TileCoord tile = VecTile(lerp_pos);

    if (at(tile) == 0) {
      point = last_lerp - vec2(16.0f * 32.0f, 16.0f * 32.0f);
      hit_tile = tile;
      return true;
    }
    
    last_lerp = lerp_pos;
    pos += dt;
  }
  
  return false;
}

void Map::damageTile(const TileCoord &tile) {
  at(tile) = 1;
  _gameserver->events().spawnTileUpdate(tile.first, tile.second, 1, 
                                        vec2(tile.first * 32, 
                                             tile.second * 32));
}

char Map::at(const TileCoord &tile) const {
  int x = clamp(tile.first, -16, 15);
  int y = clamp(tile.second, -16, 15);
  return _data[(x + 16) * 32 + (y + 16)];
}

char &Map::at(const TileCoord &tile) {
  int x = clamp(tile.first, -16, 15);
  int y = clamp(tile.second, -16, 15);
  return _data[(x + 16) * 32 + (y + 16)];
}
