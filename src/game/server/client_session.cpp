#include <game/server/client_session.h>

ClientSession::ClientSession(class Client *client)
  : client(client)
{
  name = "Some guy";
  state = 0;
  sent_map = false;
}

