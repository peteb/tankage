#ifndef ENGINE_ENET_NETWORK_H
#define ENGINE_ENET_NETWORK_H

#include <engine/network.h>
#include <utils/singleton.h>

namespace Enet {
class Network : public ::Network, public Singleton<Network>  {
public:
  Host *startHost(Address *host);
  Client *connect(Address *host);
  Address *address(const std::string &adr);
};

}

#endif // !ENGINE_ENET_NETWORK_H