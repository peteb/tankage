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

  /**
   * Updates the internal state of the Host by pulling in any new packets and
   * connection/disconnection events.
   * Will also send any unsent packets.
   */
  virtual void update() =0;
  virtual class Client *connectingClient() =0;
  virtual class Client *disconnectingClient() =0;
  virtual class Packet *pendingPacket() =0;
};

/**
 * A client is a connection to a server, or a connection on the server to a
 * client. Ie, the server contains a number of connected Client instances, and
 * each connecting user also has a Client instance.
 */
class Client {
public:  
  virtual ~Client() {}

  enum PacketFlags {
    PACKET_RELIABLE =     0x0001,
    PACKET_UNSEQUENCED =  0x0002
  };

  /**
   * Updates the internal state of the Client by pulling in any new packets and
   * connection/disconnection events.
   * Will also send any unsent packets.
   */
  virtual void update() =0; // Fixme: timeout
  virtual void send(const void *data, size_t size, unsigned flags, int channel) =0;
  virtual Packet *pendingPacket() =0;
  virtual bool isConnected() const =0;
  virtual void disconnect() =0;
  virtual std::string address() const =0;
};

class Network : public Interface {
public:
  static std::string id() {
    return "network";
  }

  virtual Host *startHost(const std::string &host,
                          size_t maxClients, size_t channels) =0;
  virtual Client *connect(const std::string &host) =0;
};


#endif // !ENGINE_NETWORK_H
