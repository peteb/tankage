#ifndef GAME_COMMON_PEER_H
#define GAME_COMMON_PEER_H

#include <game/common/system.h>
#include <vector>

class Peer : public System {
public:
  static SystemContext::SystemId id() {
    return SystemContext::SYSTEM_PEER;
  }
  
  virtual ~Peer() {}
  void registerSystem(class ReplicatedSystem *system);
  
protected:
  std::vector<class ReplicatedSystem *> _systems;
};


#endif // !GAME_COMMON_PEER_H
