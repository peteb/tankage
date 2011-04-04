#include <engine/enet/network.h>
#include <enet/enet.h>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <utility>
#include <utils/key.h>
#include <algorithm>

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
 * Enet Packet
 */
class Packet : public ::Packet {
public:
  Packet(ENetPacket *packet, class Client *sender, int channelId)
    : _packet(packet)
    , _sender(sender)
    , _channelId(channelId)
  {
  }

  ~Packet() {
    enet_packet_destroy(_packet);
  }
  
  size_t size() const {
    return _packet->dataLength;
  }

  const void *data() const {
    return _packet->data;
  }

  class Client *sender() const {
    return _sender;
  }

  size_t channel() const {
    return _channelId;
  }
  
private:
  Packet(const Packet &packet) {}
  Packet &operator =(const Packet &) {return *this; }
  
  ENetPacket *_packet;
  class Client *_sender;
  int _channelId;
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
    enet_peer_disconnect(_peer, 0);
    enet_peer_reset(_peer);
  }

  void update(unsigned int timeout) {
    ENetEvent event;
    if (enet_host_service(_host, &event, timeout) > 0) {
      switch (event.type) {
      case ENET_EVENT_TYPE_NONE:
        break;
          
      case ENET_EVENT_TYPE_CONNECT:
        _connected = true;        
        break;

      case ENET_EVENT_TYPE_DISCONNECT:
        _connected = false;
        break;

      case ENET_EVENT_TYPE_RECEIVE:
      {
        
        /*Enet::Packet *packet =
          new Enet::Packet(event.packet,
                           this,
                           event.channelID);
        _pendingPackets.push_back(packet);*/
        break;
      }       

      }
    }
  }
  
  Packet *pendingPacket() {
    //if (_pendingPackets.empty())
    //  return NULL;
    
    enet_uint8 channel;
    ENetPacket *pack = enet_peer_receive(_peer, &channel);
    if (pack) {
      return new Packet(pack, this, channel);
    }
    //Enet::Packet *ret = _pendingPackets.front();
    //_pendingPackets.erase(_pendingPackets.begin());
    // the ownership of the Packet is transfered to the user
    return NULL;
  }
  
  void send(const void *data, size_t size, unsigned flags, int channel) {
    enet_uint32 convFlags = 0;
    if (flags & ::Client::PACKET_RELIABLE)
      convFlags |= ENET_PACKET_FLAG_RELIABLE;
    if (flags & ::Client::PACKET_UNSEQUENCED)
      convFlags |= ENET_PACKET_FLAG_UNSEQUENCED;

    ENetPacket *packet = enet_packet_create(data, size, convFlags);
    if (!packet) {
      throw std::runtime_error("enet: failed to create packet for data");
    }

    enet_peer_send(_peer, channel, packet);
  }
  
  bool isConnected() const {
    return _connected;
  }

  void disconnect() {
    enet_peer_disconnect(_peer, 0);
  }

  std::string address() const {
    char hostName[64];
    enet_address_get_host_ip(&_peer->address, hostName, 64);
    // Note: enet_address_get_host (without _ip) can be used for reverse dns lookup
    
    std::stringstream ss;
    ss << hostName;
    ss << ":" << _peer->address.port;
    return ss.str();
  }

  void flush() {
    enet_host_flush(_host);
  }

  void ping() {
    enet_peer_ping(_peer);
  }
  
  unsigned stats(Client::Statistic field) {
    if (!_peer) {
      throw std::runtime_error("enet: no peer associated with client");
    }
    
    unsigned ret = 0;
    
    switch (field) {
      // Fixme: add more fields here! Enet supports a lot:
      // http://enet.bespin.org/struct__ENetPeer.html
    case STAT_RTT:
      ret = _peer->lastRoundTripTime;
      break;
      
    default:
      throw std::runtime_error("enet: invalid client statistic fied");
    }

    return ret;
  }
  
private:
  std::vector<Enet::Packet *> _pendingPackets;
  ENetHost *_host;
  ENetPeer *_peer;
  bool _connected;
};


/**
 * Enet Host
 */
class Host : public ::Host {
public:
  Host(ENetHost *host)
    : _host(host)
  {}
  

  void update(unsigned int timeout) {
    ENetEvent event;
    // Fixme: configurable timeout here
    if (enet_host_service(_host, &event, timeout) > 0) {
      switch (event.type) {
      case ENET_EVENT_TYPE_CONNECT:
      {
        Enet::Client *client = new Enet::Client(_host, event.peer);

        event.peer->data = client;
        _pendingConnect.push_back(client);
        break;
      }
      
      case ENET_EVENT_TYPE_DISCONNECT:
      {
        if (event.peer->data) {
          _pendingDisconnect.push_back(static_cast<Client *>(event.peer->data));
          event.peer->data = NULL;
        }
        break;
      }

      case ENET_EVENT_TYPE_RECEIVE:
      {
        if (event.peer->data) {
          Enet::Packet *packet =
            new Enet::Packet(event.packet,
                             static_cast<Client *>(event.peer->data),
                             event.channelID);
          _pendingPackets.push_back(packet);
        }
        break;
      }
      

      case ENET_EVENT_TYPE_NONE:
        break;
      }
    }

  }
  
  void flush() {
    enet_host_flush(_host);
  }
  
  ::Client *connectingClient() {
    if (_pendingConnect.empty())
      return NULL;

    ::Client *ret = _pendingConnect.front();
    _pendingConnect.erase(_pendingConnect.begin());
    
    return ret;
  }

  ::Client *disconnectingClient() {
    if (_pendingDisconnect.empty())
      return NULL;

    ::Client *ret = _pendingDisconnect.front();
    _pendingDisconnect.erase(_pendingDisconnect.begin());
    // the ownership of the Client is transfered to the user
    return ret;
  }
  
  ::Packet *pendingPacket() {
    if (_pendingPackets.empty())
      return NULL;

    ::Packet *ret = _pendingPackets.front();
    _pendingPackets.erase(_pendingPackets.begin());
    // the ownership of the Packet is transfered to the user
    return ret;
  }


private:
  std::vector<Enet::Client *> _pendingConnect;
  std::vector<Enet::Client *> _pendingDisconnect;
  std::vector<Enet::Packet *> _pendingPackets;
  ENetHost *_host;
};


}

Enet::Network::Network() {
  if (enet_initialize() != 0) {
    throw std::runtime_error("enet: failed to initialize enet");
  }

  enet_time_set(0);
  atexit(enet_deinitialize);

  std::cout << "enet: initialized" << std::endl; // FIXME: use logging
}

Host *Enet::Network::startHost(const std::string &hostAddr,
                               size_t maxClients, size_t channels) {
  Address addr(hostAddr);
  // Fixme: the values below should be configurable
  ENetHost *host = enet_host_create(addr.enetAddress(),
                                    maxClients,
                                    channels,
                                    0,
                                    0);

  if (!host) {
    throw std::runtime_error("enet: failed to start host");
  }
                                    
  return new Enet::Host(host);
}

Client *Enet::Network::connect(const std::string &host, size_t channels) {
  Address peerAddr(host);
  ENetPeer *peer;
  ENetHost *client;

  client = enet_host_create(NULL, 1, channels, 0, 0);
  if (!client) {
    throw std::runtime_error("enet: failed to create host");
  }

  peer = enet_host_connect(client, peerAddr.enetAddress(), channels, 0);
  if (!peer) {
    enet_host_destroy(client);
    throw std::runtime_error("enet: failed to create peer");
  }
  
  return new Enet::Client(client, peer);
}

unsigned long Enet::Network::time() {
   return enet_time_get();
}
