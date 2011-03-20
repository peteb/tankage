#include <game/common/peer.h>

void Peer::registerSystem(class ReplicatedSystem *system) {
  _systems.push_back(system);
}

