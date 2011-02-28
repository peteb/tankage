#ifndef GAME_CONTROL_H
#define GAME_CONTROL_H

#include <game/common/system.h>
#include <game/common/actors.h>
#include <game/common/tank.h>
#include <game/common/replicated_system.h>

#include <string>
#include <map>
#include <vector>
#include <utility>

class Control : public ReplicatedSystem {
public:
  struct Move {
    float time;
    Tank::Input delta;
    Tank::State absolute;
  };

  typedef std::vector<Move> MoveVector;

  template<typename T>
  class CircularForwardIterator : public std::iterator<std::forward_iterator_tag,
                                                       T, ptrdiff_t, const T *, const T&> {
  public:
    CircularForwardIterator(size_t pos, T &source)
      : pos(pos)
      , source(source)
    {
    }
    
    CircularForwardIterator &operator++() {
      pos = (pos + 1) % source.size();
      return *this;
    }

    const typename T::value_type operator *() const {
      return source[pos];
    }
    
    bool operator != (const CircularForwardIterator &other) const {
      return pos != other.pos;
    }
    
  private:
    size_t pos;
    T &source;
  };

  typedef CircularForwardIterator<MoveVector> MoveIterator;
  typedef std::pair<MoveIterator, MoveIterator> MoveRange;
  
  Control();
  
  void init(const class Portal &interfaces);
  void update();
  void onReceive(NetPacketType type, const class Packet &packet);
  const Tank::Input *lastInput(ActorId actor) const;
  MoveRange history(float time);
  
private:  
  void onTick(class Client *client);
  Tank::Input currentState() const;
  
  class WindowManager *wm;
  double lastTick;
  int keyUp, keyDown, keyLeft, keyRight, keyShoot;
  class Input *input;
  std::string snail;

  std::map<ActorId, Tank::Input> states;
  MoveVector moves;
  
  // data that will be replicated
  double inputBegan;
  Tank::Input state;
};

#endif // !GAME_CONTROL_H
