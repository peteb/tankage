#ifndef GAME_SNAILS_H
#define GAME_SNAILS_H

#include <game/common/system.h>
#include <game/common/replicated_system.h>

#include <utils/vec.h>
#include <vector>

class Snails : public ReplicatedSystem {
public:
  ~Snails();

  enum DuelingSnail {
    SNAIL_LEFT = 0,
    SNAIL_RIGHT
  };
  
  void init(const class Portal &interfaces);
  void render();
  class Snail *snail(int id) const;
  class Snail *intersectingSnails(const vec2 &start, const vec2 &end, float radius, Snail *ignore, vec2 &hitpos);

  void onTick(class Client *client);
  void onReceive(NetPacketType type, const class Packet &packet);
  
private:
  class Graphics *graphics;
  class WindowManager *wm;
  
  typedef std::vector<class Snail *> SnailVector;
  SnailVector snails;
  double lastUpdate;
};

#endif // !GAME_SNAILS_H
