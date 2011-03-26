#ifndef GAME_SERVER_EVENTS_H
#define GAME_SERVER_EVENTS_H

#include <utils/packer.h>
#include <vector>
#include <string>

class Events {
public:
  void snap(class Packer &msg, class ClientSession *client);  
  void createPlayerJoined(int tankid, const std::string &name);
  void removeSnapped();
  void writeReliable(class Packer &msg, class ClientSession *client);
  
private:
  std::vector<class Event *> _events;
  std::vector<class Event *> _rel_events;
};


#endif // !GAME_SERVER_EVENTS_H
