#ifndef GAME_SNAILS_H
#define GAME_SNAILS_H

#include <game/system.h>
#include <utils/vec.h>
#include <vector>

class Snail {
public:
  enum SnailState {
    STATE_MOVE_UP,
    STATE_MOVE_DOWN,
    STATE_MAX
  };

  Snail(const vec2 &initialPos);
  
  void startState(SnailState state);
  void stopState(SnailState state);
  void setTexture(class Texture *texture);

  void render(class Graphics *graphics);
  void update();
  
private:
  class Texture *texture;
  vec2 position;

  bool _state[STATE_MAX];
};

class Snails : public System {
public:
  Snails(const class Portal &interfaces, SystemContext *ctx);

  enum DuelingSnail {
    SNAIL_LEFT = 0,
    SNAIL_RIGHT
  };
  
  void render();
  Snail *snail(int id) const;

private:
  class Graphics *graphics;

  typedef std::vector<Snail *> SnailVector;
  SnailVector snails;
};

#endif // !GAME_SNAILS_H
