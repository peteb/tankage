#ifndef GAME_SERVER_PAGE_H
#define GAME_SERVER_PAGE_H

#include <utils/vec.h>
#include <vector>
#include <utility>

typedef std::pair<int, int> TileCoord;

class Page {
public:
  static const int WIDTH = 64;
  static const int HEIGHT = 64;

  Page(class b2World *world);
  ~Page();
  
  void snap(class Packer &msg);
  void load();
  void unload();
  void refresh(const TileCoord &coord);
  char &tileAt(const TileCoord &coord);
  bool intersectSolid(const vec2 &start, const vec2 &end, 
                      vec2 &point, TileCoord &hit_tile);
  
private:
  void destroyFixtures();
  TileCoord vec2Tile(const vec2 &pos) const;
  
  class b2World *_world;
  class b2Body *_body;
  char _tiles[WIDTH * HEIGHT];
  std::vector<class b2Fixture *> _fixtures;
};

#endif // !GAME_SERVER_PAGE_H
