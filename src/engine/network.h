#ifndef ENGINE_NETWORK_H
#define ENGINE_NETWORK_H

#include <engine/interface.h>
#include <string>

/*
  Use cases
  * Start server
  * Stop server
  * Handle new connections (get + accept)
  * Set config vars (MAX_BANDWIDTH, etc)
  * Send reliable messages to a client
  * Read incoming data
  * Disconnect a client
  * Broadcast a message
  * Create a packet
*/

class Packet {
public:
  virtual ~Packet() {}

  virtual size_t size() const =0;
  virtual void resize(size_t) =0;
  virtual void *data() =0;

};

class Host {
public:
  virtual ~Host() {}
  
  virtual void receive() =0;
  virtual class Client *connectingClient() =0;
  virtual class Client *disconnectingClient() =0;
  virtual class Packet *pendingPacket() =0;
//  virtual void broadcast(Packet *) =0;
};

class Client {
public:  
  virtual ~Client() {}

  // No connecting/disconnectingClient, state() instead?
  virtual void receive() =0;
  virtual Packet *pendingPacket() =0;
  virtual void send(Packet *) =0;
};

class Network : public Interface {
public:
  static std::string id() {
    return "network";
  }

  virtual Host *startHost(const std::string &host) =0;
  virtual Client *connect(const std::string &host) =0;
};


#endif // !ENGINE_NETWORK_H
