#ifndef GAME_SERVER_CLIENT_SESSION_H
#define GAME_SERVER_CLIENT_SESSION_H

#include <string>

class ClientSession {
public:
  enum {
    STATE_UNKNOWN = 0,
    STATE_IDENTIFIED = 1
  };
    
  ClientSession(class Client *client);

  int state;
  Client * const client;
  int tankid;
  std::string name;
  bool sent_map;
};

#endif // !GAME_SERVER_CLIENT_SESSION_H
