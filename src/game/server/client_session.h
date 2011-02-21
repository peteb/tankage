#ifndef GAME_SERVER_CLIENT_SESSION_H
#define GAME_SERVER_CLIENT_SESSION_H

#include <game/common/players.h> // for PlayerId..

class ClientSession {
public:
  enum {
    STATE_IDENTIFIED = 1
  };
    
  ClientSession(class Client *client);

  int state;
  Client * const client;
  PlayerId player;
};

#endif // !GAME_SERVER_CLIENT_SESSION_H
