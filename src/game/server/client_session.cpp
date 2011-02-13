#include <game/server/client_session.h>

ClientSession::ClientSession(class Client *client)
  : _client(client)
{
  state = 0;
}

