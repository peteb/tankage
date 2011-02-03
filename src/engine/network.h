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

class Host {
public:
  virtual ~Host() {}
};

class Client {
public:
  enum State {
    CONNECTING,
    CONNECTED,
    DISCONNECTED
  };
  
  virtual ~Client() {}

  virtual State state() =0;
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
