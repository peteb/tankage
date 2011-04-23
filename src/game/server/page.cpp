#include "page.h"
#include <utils/packer.h>
#include <utils/log.h>
#include <game/common/net_protocol.h>
#include <game/server/gameserver.h>
#include <Box2D/Box2D.h>

Page::Page(b2World *world) 
  : _world(world)
{
  _body = 0;
  
  
  std::fill(_tiles, _tiles + WIDTH*HEIGHT, TileWall);
  for (int y = 0; y < HEIGHT; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      if (y >= HEIGHT/3 && y <= HEIGHT/3*2) {
        if (x >= WIDTH/3 && x <= WIDTH/3*2) {
          _tiles[y*WIDTH+x] = TileGrass;
        }
      }
      
      if (rand() % 30 == 1 && _tiles[y*WIDTH+x] != TileGrass) {
        _tiles[y*WIDTH+x] = TileKasparium;
      }
      
      if (_tiles[y*WIDTH+x] == TileKasparium && rand() % 5 == 1) {
        _tiles[y*WIDTH+x] = TileKaspariumGen;
        _gens.push_back(TileCoord(x, y));
      }
    }
  }
}

Page::~Page() {
  unload();
}


void Page::snap(Packer &msg) {
  msg.writeShort(NET_MAPCHUNK);
  msg.writeData(_tiles, WIDTH*HEIGHT);  
}

void Page::load() {
  Log(DEBUG) << "loading page fixtures...";
  if (!_body) {
    b2BodyDef static_body_def;
    _body = _world->CreateBody(&static_body_def);        
  }

  destroyFixtures();
  
  b2PolygonShape tile_shape;
  _fixtures.resize(WIDTH*HEIGHT);
  
  for (int y = 0; y < HEIGHT; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      char tile_type = tileAt(TileCoord(x, y));
      if ((tile_type & 0x0F) & TileWall) {
        tile_shape.SetAsBox(0.5f, 0.5f, b2Vec2(x - WIDTH/2, y - HEIGHT/2), 0.0f);
        _fixtures[x + y * WIDTH] = _body->CreateFixture(&tile_shape, 1.0f);
      }
    }
  }  

  Log(DEBUG) << "   done";
}

void Page::unload() {
  if (!_body)
    return;
  
  destroyFixtures();
  
  _world->DestroyBody(_body);
  _body = 0;
}


TileCoord Page::vec2Tile(const vec2 &pos) const {
  vec2 ofs = pos;
  
  int tile_x = ofs.x / 32.0f;
  int tile_y = ofs.y / 32.0f;
  
  tile_x = clamp(tile_x, 0, 63);
  tile_y = clamp(tile_y, 0, 63);
  
  return TileCoord(tile_x, tile_y);
}

vec2 Page::tilePos(const TileCoord &coord) const {
  return vec2(float(coord.first - WIDTH/2) * 32.0f, 
              float(coord.second - HEIGHT/2) * 32.0f);
}

bool Page::intersectSolid(const vec2 &start, const vec2 &end, 
                          vec2 &point, TileCoord &hit_tile) {  
  vec2 start_ofs = start + vec2(32.0f * 32.0f + 16.0f, 32.0f * 32.0f + 16.0f);
  vec2 end_ofs = end + vec2(32.0f * 32.0f + 16.0f, 32.0f * 32.0f + 16.0f);
  double mag = length(end_ofs - start_ofs);

  if (mag <= 0.0001)
    return false;
  
  double dt = 1.0 / mag;
  double pos = 0.0;
  vec2 last_lerp = start_ofs;
  
  // FIXME: break this up at least, so we don't lerp each iteration
  // FIXME: this function should really be improved.
  // FIXME: also, support radius... <--- functional. important
  // FIXME: return Map::Collision
  
  while (pos <= 1.0) {
    vec2 lerp_pos = lerp(start_ofs, end_ofs, pos);
    TileCoord tile = vec2Tile(lerp_pos);
    
    if ((tileAt(tile) & 0x0F) & TileWall) {
      point = last_lerp; 
      hit_tile = tile;
      return true;
    }
    
    last_lerp = lerp_pos - vec2(32.0f * 32.0f + 16.0f, 32.0f * 32.0f + 16.0f);
    pos += dt;
  }
  
  return false;
}

void Page::refresh(const TileCoord &coord) {
  if (!_body)
    return;
  
  size_t fixture = coord.first + coord.second * WIDTH;
  if (_fixtures.size() < fixture)
    return;
  
  Log(DEBUG) << "refreshing tile " << coord.first << ", " << coord.second;
  
  b2PolygonShape tile_shape;
  if (_fixtures.at(fixture))
    _body->DestroyFixture(_fixtures[fixture]);
  
  char tile_type = tileAt(coord);
  if ((tile_type & 0x0F) & TileWall) {
    tile_shape.SetAsBox(0.5f, 0.5f, b2Vec2(coord.first - WIDTH/2, coord.second - HEIGHT/2), 0.0f);
    b2Fixture *new_fix = _body->CreateFixture(&tile_shape, 1.0f);
    _fixtures[fixture] = new_fix;
  }
  else {
    _fixtures[fixture] = 0;
  }
  
}

void Page::destroyFixtures() {
  if (!_body)
    return;
  
  for (size_t i = 0; i < _fixtures.size(); ++i) {
    _body->DestroyFixture(_fixtures[i]);    
  }
  
  _fixtures.clear();
}

char &Page::tileAt(const TileCoord &coord) {
  return _tiles[clamp(coord.first, 0, 63) + clamp(coord.second, 0, 63) * WIDTH];
}

void Page::update(double dt, GameServer *gameserver) {
  std::vector<KaspariumGenerator>::iterator iter = _gens.begin();
  while (iter != _gens.end()) {
    char &tile = tileAt(iter->coord());
    if ((tile & 0x0F) == TileKaspariumGen) {
      iter->update(dt, *this, gameserver);
      ++iter;
    }
    else {
      iter = _gens.erase(iter);
    }
  }
}

KaspariumGenerator::KaspariumGenerator(const TileCoord &coord)
  : _coord(coord)
{
  _interval = double(rand() % 20 + 10);
  _reload = _interval;
}

void KaspariumGenerator::update(double dt, Page &page, GameServer *gameserver) {
  _reload -= dt;
  
  if (_reload <= 0.0) {
    _reload = _interval;
    
    std::vector<TileCoord> neighbors;
    neighbors.push_back(TileCoord(-1, -1));
    neighbors.push_back(TileCoord( 0, -1));
    neighbors.push_back(TileCoord( 1, -1));
    neighbors.push_back(TileCoord( 1,  0));
    neighbors.push_back(TileCoord( 1,  1));
    neighbors.push_back(TileCoord( 0,  1));
    neighbors.push_back(TileCoord(-1,  1));
    neighbors.push_back(TileCoord(-1,  0));
    
    size_t ofs = rand() % neighbors.size();
    
    for (size_t i = 0; i < neighbors.size(); ++i) {
      TileCoord &nbor = neighbors[(i + ofs) % neighbors.size()];
      TileCoord abs = coord();
      abs.first += nbor.first;
      abs.second += nbor.second;
      
      char &tile = page.tileAt(abs);
      if ((tile & 0x0F) != Page::TileKasparium && 
          (tile & 0x0F) != Page::TileKaspariumGen) {
        tile = Page::TileKasparium;
        page.refresh(abs);
        gameserver->events().spawnTileUpdate(abs.first, abs.second, tile, 
                                              vec2((abs.first - 16) * 32, 
                                                   (abs.second - 16) * 32));  
        
        break;
      }
    }
  }
}
