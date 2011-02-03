#ifndef ENGINE_ENET_NETWORK_H
#define ENGINE_ENET_NETWORK_H

#include <engine/network.h>
#include <utils/singleton.h>
#include <enet/enet.h>

namespace Enet {
class Network : public ::Network, public Singleton<Network>  {
public:
  Host *startHost(::Address *host);
  Client *connect(::Address *host);
  Address *address(const std::string &adr);
};


class Address : public ::Address {
public:
  Address(const std::string &addr, int port);
  
private:
  ENetAddress address;
};

}

#endif // !ENGINE_ENET_NETWORK_H
