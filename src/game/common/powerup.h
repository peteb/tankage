#ifndef GAME_POWERUP_H
#define GAME_POWERUP_H

#include <game/common/items.h>
#include <string>
#include <utils/vec.h>

class Powerup : public Item {
public:
  Powerup(const class vec2 &pos, class Texture *tex,
          const std::string &type, int amount, const class SystemContext *ctx);

  void render(class Graphics *gfx);
  bool update(double dt);
  bool takeDamage(const vec2 &pos, float damage, class Snail *shooter);

private:
  class Texture *tex;
  std::string type;
  int amount;
  vec2 vel;
  const class SystemContext *ctx;
  float radius;
};

#endif // !GAME_POWERUP_H
