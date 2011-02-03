#include <engine/enet/network.h>
#include <enet/enet.h>
#include <sstream>

Host *Enet::Network::startHost(::Address *host) {
  return NULL;
}

Client *Enet::Network::connect(::Address *host) {
  return NULL;
}

Address *Enet::Network::address(const std::string &adr) {
  Address *ret = NULL;
  size_t colonPos = adr.find(':');

  if (colonPos != std::string::npos) {
    std::string straddr = adr.substr(0, colonPos);
    std::stringstream ss;
    ss << adr.substr(colonPos + 1);
    int port = 0;
    ss >> port;
    ret = new Address(straddr, port);
  }
  else {
    ret = new Address(adr, 0);
  }
  
  return ret;
}


Enet::Address::Address(const std::string &addr, int port) {
  address.host = ENET_HOST_ANY;
  address.port = port;

  if (!addr.empty()) {
    enet_address_set_host(&address, addr.c_str());
  }
}
