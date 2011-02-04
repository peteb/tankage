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

/**
 * Enet Host
 */
class Host : public ::Host {
public:
  Host(ENetHost *host)
    : _host(host)
  {}
  

  void receive() {
    ENetEvent event;
    if (enet_host_service(_host, &event, 0) > 0) {
      switch (event.type) {
      case ENET_EVENT_TYPE_CONNECT:
        std::cout << "connecting: " << event.peer->address.host << std::endl;
        break;

      case ENET_EVENT_TYPE_DISCONNECT:
        std::cout << "disconnecting: " << event.peer->address.host << std::endl;
        break;
      }
    }

  }
  
  class Client *connectingClient() {
    return NULL;
  }
  
  class Client *disconnectingClient() {
    return NULL;
  }
  
  class Packet *pendingPacket() {
    return NULL;
  }


private:
  ENetHost *_host;
};

/**
 * Enet Client
 */
class Client : public ::Client {
public:
  Client(ENetHost *host, ENetPeer *peer)
    : _host(host)
    , _peer(peer)
  {
    _connected = false;
  }

  ~Client() {
    enet_peer_reset(_peer);
  }

  void receive() {
    ENetEvent event;
    if (enet_host_service(_host, &event, 0) > 0) {
      switch (event.type) {
      case ENET_EVENT_TYPE_CONNECT:
        _connected = true;        
        break;

      case ENET_EVENT_TYPE_DISCONNECT:
        _connected = false;
        break;

      }
    }
  }
  
  Packet *pendingPacket() {
    return NULL;
  }
  
  void send(Packet *) {

  }

  bool isConnected() const {
    return _connected;
  }
  
private:
  ENetHost *_host;
  ENetPeer *_peer;
  bool _connected;
};

}

Enet::Network::Network() {
  if (enet_initialize() != 0) {
    throw std::runtime_error("enet: failed to initialize enet");
  }

  std::cout << "enet: initialized" << std::endl;
  atexit(enet_deinitialize);
}

Host *Enet::Network::startHost(const std::string &hostAddr) {
  Address addr(hostAddr);
  // Fixme: the values below should be configurable
  ENetHost *host = enet_host_create(addr.enetAddress(),
                                    32,
                                    2,
                                    0,
                                    0);

  if (!host) {
    throw std::runtime_error("enet: failed to start host");
  }
                                    
  return new Enet::Host(host);
}

Client *Enet::Network::connect(const std::string &host) {
  Address peerAddr(host);
  ENetPeer *peer;
  ENetHost *client;

  client = enet_host_create(NULL, 1, 2, 0, 0);
  if (!client) {
    throw std::runtime_error("enet: failed to create host");
  }

  peer = enet_host_connect(client, peerAddr.enetAddress(), 2, 0);
  if (!peer) {
    enet_host_destroy(client);
    throw std::runtime_error("enet: failed to create peer");
  }
  
  return new Enet::Client(client, peer);
}

