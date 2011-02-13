#ifndef GAME_SERVER_CLIENT_SESSION_H
#define GAME_SERVER_CLIENT_SESSION_H

class ClientSession {
public:
  ClientSession(class Client *client);

private:
  class Client *_client;
};

#endif // !GAME_SERVER_CLIENT_SESSION_H
