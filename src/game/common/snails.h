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

  const vec2 &position() const;
  
private:
  class Texture *texture;
  vec2 _position, originalPos;
  vec2 vel;
  
  int id;
  bool _state[STATE_MAX];
  const SystemContext *context;
  double secondsSinceFire;
  float radius;
  bool takingControl;
  int health;
};


class Snails : public System, public ReplicatedSystem {
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

  void onIdent(class Client *client);
  void onTick(class Client *client);
  void onReceive(NetPacketType type, const void *data, size_t size);
  
private:
  class Graphics *graphics;
  class WindowManager *wm;
  
  typedef std::vector<Snail *> SnailVector;
  SnailVector snails;
  double lastUpdate;
};

#endif // !GAME_SNAILS_H
