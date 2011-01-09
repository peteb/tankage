#ifndef GAME_SNAILS_H
#define GAME_SNAILS_H

#include <game/system.h>

class Snail {
public:
  enum SnailState {
    STATE_MOVE_UP,
    STATE_MOVE_DOWN
  };
  
  void startState(SnailState state);
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
};

#endif // !GAME_SNAILS_H
