#include <engine/enet/network.h>
#include <enet/enet.h>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <iostream>


namespace {
// Secret helper class for addresses
class Address {
public:  
  Address(const std::string &addr) {
    size_t colonPos = addr.find(':');
    int port;
    std::string ip;
    
    if (colonPos != std::string::npos) {
      ip = addr.substr(0, colonPos);
      std::stringstream ss;
      ss << addr.substr(colonPos + 1);
      ss >> port;
    }
    else {
      ip = addr;
      port = 0;
    }

    setAddress(ip, port);
  }

  const ENetAddress *enetAddress() const {
    return &address;
  }

private:
  void setAddress(const std::string &ip, int port) {
    address.host = ENET_HOST_ANY;
    address.port = port;
    
    if (!ip.empty()) {
      if (enet_address_set_host(&address, ip.c_str()) < 0) {
        throw std::runtime_error("failed to create enet address");
      }
    }
  }

  ENetAddress address;
};

}

namespace Enet {
class Host : public ::Host {
public:
  Host(ENetHost *host)
    : _host(host)
  {}
  

private:
  ENetHost *_host;
};
}

Enet::Network::Network() {
  if (enet_initialize() != 0) {
    throw std::runtime_error("Failed to initialize enet");
  }

  std::cout << "enet: initialized" << std::endl;
  atexit(enet_deinitialize);
}

Host *Enet::Network::startHost(const std::string &hostAddr) {
  Address addr(hostAddr);
  ENetHost *host = enet_host_create(addr.enetAddress(),
                                    32,
                                    2,
                                    0,
                                    0);

  if (!host) {
    throw std::runtime_error("failed to start host");
  }
                                    
  return new Enet::Host(host);
}

Client *Enet::Network::connect(const std::string &host) {
  return NULL;
}

