#ifndef ENGINE_ENET_NETWORK_H
#define ENGINE_ENET_NETWORK_H

#include <engine/network.h>
#include <utils/singleton.h>
#include <enet/enet.h>

namespace Enet {
class Network : public ::Network, public Singleton<Network>  {
public:
  Network();

  Host *startHost(const std::string &host);
  Client *connect(const std::string &host);
};


}

#endif // !ENGINE_ENET_NETWORK_H
