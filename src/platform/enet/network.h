#ifndef PLATFORM_ENET_NETWORK_H
#define PLATFORM_ENET_NETWORK_H

#include <platform/network.h>
#include <utils/singleton.h>
#include <enet/enet.h>

namespace Enet {
class Network : public ::Network, public Singleton<Network>  {
public:
  Network();
  ~Network();
  
  Host *startHost(const std::string &host, size_t maxClients, size_t channels);
  Client *connect(const std::string &host, size_t channels);
  Packet *packet(const void *data, size_t dataSize, unsigned flags);
  
  unsigned long time();
};


}

#endif // !PLATFORM_ENET_NETWORK_H
