#ifndef PLATFORM_NETWORK_H
#define PLATFORM_NETWORK_H

#include <platform/interface.h>
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
  virtual const void *data() const =0;
  virtual class Client *sender() const =0;
  virtual size_t channel() const =0;
};

class Host {
public:
  virtual ~Host() {}

  /**
   * Updates the internal state of the Host by pulling in any new packets and
   * connection/disconnection events.
   * Will also send any unsent packets.
   */
  virtual void update(unsigned int timeout = 0) =0;
  virtual class Client *connectingClient() =0;
  virtual class Client *disconnectingClient() =0;
  virtual class Packet *pendingPacket() =0;
  virtual void flush() =0;
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

  enum Statistic {
    STAT_RTT
  };
  
  /**
   * Updates the internal state of the Client by pulling in any new packets and
   * connection/disconnection events.
   * Will also send any unsent packets.
   */
  virtual void update(unsigned int timeout) =0; // Fixme: timeout
  virtual void send(const void *data, size_t size, unsigned flags, int channel) =0;
  virtual Packet *pendingPacket() =0;
  virtual bool isConnected() const =0;
  virtual void disconnect() =0;
  virtual std::string address() const =0;
  virtual void flush() =0;
  virtual unsigned stats(Statistic field) =0;
  virtual void ping() =0;
  
};

class Network : public Interface {
public:
  static std::string id() {
    return "network";
  }

  virtual Host *startHost(const std::string &host,
                          size_t maxClients, size_t channels) =0;
  virtual Client *connect(const std::string &host, size_t channels) =0;
  virtual unsigned long time() =0;
};


#endif // !PLATFORM_NETWORK_H
