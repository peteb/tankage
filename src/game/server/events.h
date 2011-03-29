#ifndef GAME_SERVER_EVENTS_H
#define GAME_SERVER_EVENTS_H

#include <utils/packer.h>
#include <vector>
#include <string>

class Events {
public:
  void snap(class Packer &msg, class ClientSession *client);  
  void removeSnapped();
  void writeReliable(class Packer &msg, class ClientSession *client);

  void createPlayerJoined(int tankid, const std::string &name);
  void spawnTankHit(int tankid, int shooter, const class vec2 &pos);
  void spawnTileUpdate(int x, int y, char state, const class vec2 &pos);
  
private:
  enum {
    EVENT_UNRELIABLE = 0x0001
  };
  
  class Event *spawnLocalEvent(short type, const class vec2 &pos, unsigned flags);
  class Event *spawnGlobalEvent(short type, unsigned flags);

  std::vector<class Event *> _events;
  std::vector<class Event *> _rel_events;
};


#endif // !GAME_SERVER_EVENTS_H
