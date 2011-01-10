#ifndef GAME_ITEMS_H
#define GAME_ITEMS_H

#include <game/system.h>
#include <utils/vec.h>
#include <vector>

class Cactus {
public:
  Cactus(const vec2 &pos, class Texture *tex);
  
  void render(class Graphics *gfx);
  bool update(double dt);
  
private:
  class Texture *tex;
  vec2 pos;
};

class Items : public System {
public:
  Items(const class Portal &interfaces, SystemContext *ctx);

  void update();
  void render();
  
private:
  class WindowManager *wm;
  class Graphics *gfx;
  class Texture *cactusTexture;
  
  double lastGentime;
  double lastUpdate;
  
  typedef std::vector<Cactus *> CactusVector;
  CactusVector cactii; 
};

#endif // !GAME_ITEMS_H
