#include <game/server/client_session.h>

ClientSession::ClientSession(class Client *client)
  : client(client)
{
  state = 0;
}

