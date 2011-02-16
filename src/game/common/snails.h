#ifndef GAME_SNAILS_H
#define GAME_SNAILS_H

#include <game/common/system.h>
#include <game/common/replicated_system.h>

#include <utils/vec.h>
#include <vector>

class Snail {
public:
  enum SnailState {
    STATE_MOVE_UP,
    STATE_MOVE_DOWN,
    STATE_TURN_LEFT,
    STATE_TURN_RIGHT,
    STATE_TURN_TURRET_LEFT,
    STATE_TURN_TURRET_RIGHT,
    STATE_SHOOT,
    STATE_MAX
  };

  Snail(const vec2 &initialPos, int id, const SystemContext *ctx);
  ~Snail();
  
  void startState(SnailState state);
  void stopState(SnailState state);
  void setTexture(class Texture *texture);

  void render(class Graphics *graphics);
  bool update(double dt);
  void takeDamage(const vec2 &pos, float damage);
  bool takeItem(const std::string &type, int amount);
  
  bool intersects(const vec2 &start, const vec2 &end, float radius, vec2 &hitpos);
  void onSnap(NetSnailSnapshot &snapshot);
  
  const vec2 &position() const;
  
private:
  class Texture *texture;
  vec2 _position, originalPos;
  float _dir;
  NetSnailSnapshot snapshots[2];
  double sinceSnap;
  
  int id;
  const SystemContext *context;
  float radius;
  vec2 vel;
  bool _state[STATE_MAX];

  double secondsSinceFire;
  bool takingControl;
  int health;
};


class Snails : public ReplicatedSystem {
public:
  ~Snails();

  enum DuelingSnail {
    SNAIL_LEFT = 0,
    SNAIL_RIGHT
  };
  
  void init(const class Portal &interfaces);
  void render();
  Snail *snail(int id) const;
  Snail *intersectingSnails(const vec2 &start, const vec2 &end, float radius, Snail *ignore, vec2 &hitpos);

  void onTick(class Client *client);
  void onReceive(NetPacketType type, const class Packet &packet);
  
private:
  class Graphics *graphics;
  class WindowManager *wm;
  
  typedef std::vector<Snail *> SnailVector;
  SnailVector snails;
  double lastUpdate;
};

#endif // !GAME_SNAILS_H
