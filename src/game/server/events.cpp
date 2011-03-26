#include "events.h"
#include <game/server/client_session.h>
#include <game/common/net_protocol.h>
#include <utils/packer.h>
#include <utils/vec.h>

class Event {
public:
  Event(short type, const vec2 &pos)
    : type(type)
    , pos(pos)
    , params(param_buffer, param_buffer + BUFSZ)
  {
    snapped = false;
  }
  
  short type;
  bool snapped;
  vec2 pos;
  Packer params;
  static const size_t BUFSZ = 32;
  char param_buffer[BUFSZ];
};

void Events::snap(Packer &msg, class ClientSession *client) {
  for (size_t i = 0; i < _events.size(); ++i) {
    msg.writeShort(_events[i]->type);
    msg.writeData(_events[i]->params);
    _events[i]->snapped = true;
  }
}

void Events::writeReliable(Packer &msg, class ClientSession *client) {
  for (size_t i = 0; i < _rel_events.size(); ++i) {
    msg.writeShort(_rel_events[i]->type);
    msg.writeData(_rel_events[i]->params);
    _rel_events[i]->snapped = true;
  }  
}

void Events::removeSnapped() {
  std::vector<Event *>::iterator it = _events.begin();
  while (it != _events.end()) {
    if ((*it)->snapped) {
      delete *it;
      it = _events.erase(it);
    }
    else
      ++it;
  }

  // remove reliable
  it = _rel_events.begin();
  while (it != _rel_events.end()) {
    if ((*it)->snapped) {
      delete *it;
      it = _rel_events.erase(it);
    }
    else
      ++it;
  }
}

void Events::createPlayerJoined(int tankid, const std::string &name) {
  // unreliable goes into snapshot, reliable goes in own packet
  
  Event *event = new Event(NET_PLAYER_JOINED, vec2(0.0f, 0.0f));
  event->params.writeInt(tankid);
  event->params.writeString(name);
  _events.push_back(event);
}
