#ifndef ENGINE_NETWORK_H
#define ENGINE_NETWORK_H

#include <engine/interface.h>


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
  Fixme: maybe a class? "createHost", etc, then it could be a Network interface
  * createHost, createClient, address
*/

class Network : public Interface {
public:
  static std::string id() {
    return "network";
  }



};


#endif // !ENGINE_NETWORK_H
