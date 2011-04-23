#ifndef GAME_SERVER_PAGE_H
#define GAME_SERVER_PAGE_H

#include <utils/vec.h>
#include <vector>
#include <utility>

typedef std::pair<int, int> TileCoord;

class KaspariumGenerator {
public:
  KaspariumGenerator(const TileCoord &coord);
  void update(double dt, class Page &page, class GameServer *gameserver);
  
  const TileCoord &coord() const {
    return _coord;
  }
  
private:
  TileCoord _coord;
  double _reload;
  double _interval;
};

class Page {
public:
  static const int WIDTH = 64;
  static const int HEIGHT = 64;

  enum Tiles {
    TileGrass         = 0x00,
    TileWall          = 0x01,
    TileKasparium     = 0x03,
    TileKaspariumGen  = 0x05
  };
  
  Page(class b2World *world);
  ~Page();
  
  void snap(class Packer &msg);
  void load();
  void unload();
  void update(double dt, class GameServer *gameserver);
  void refresh(const TileCoord &coord);
  char &tileAt(const TileCoord &coord);
  bool intersectSolid(const vec2 &start, const vec2 &end, 
                      vec2 &point, TileCoord &hit_tile);
  vec2 tilePos(const TileCoord &coord) const;
  
private:
  void destroyFixtures();
  TileCoord vec2Tile(const vec2 &pos) const;
  
  class b2World *_world;
  class b2Body *_body;
  char _tiles[WIDTH * HEIGHT];
  std::vector<class b2Fixture *> _fixtures;
  std::vector<KaspariumGenerator> _gens;
};

#endif // !GAME_SERVER_PAGE_H
