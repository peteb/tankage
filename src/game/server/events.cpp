#include "events.h"
#include <game/server/client_session.h>
#include <game/common/net_protocol.h>
#include <utils/packer.h>
#include <utils/vec.h>

class Event {
public:
  Event(short type)
    : params(param_buffer, param_buffer + BUFSZ)
    , _type(type)
  {
    snapped = false;
  }

  virtual void snap(Packer &msg, ClientSession *client) {
    msg.writeShort(_type);
    msg.writeData(params);    
  }

  bool snapped;

  Packer params;
  static const size_t BUFSZ = 32;
  char param_buffer[BUFSZ];

private:
  short _type;
};

class LocalEvent : public Event {
public:
  LocalEvent(short type, const vec2 &pos)
    : Event(type)
    , _pos(pos)
  {
  }
  
  void snap(Packer &msg, ClientSession *client) {
    if (true) { // FIXME:  check whether client is interested
      Event::snap(msg, client);
    }
  }
  
private:
  vec2 _pos;
};

void Events::snap(Packer &msg, class ClientSession *client) {
  for (size_t i = 0; i < _events.size(); ++i) {
    _events[i]->snap(msg, client);
    _events[i]->snapped = true;
  }
}

void Events::writeReliable(Packer &msg, class ClientSession *client) {
  for (size_t i = 0; i < _rel_events.size(); ++i) {
    _rel_events[i]->snap(msg, client);
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
  Event *event = spawnGlobalEvent(NET_PLAYER_JOINED, 0);
  event->params.writeInt(tankid);
  event->params.writeString(name);
}

// FIXME: some kind of EventBuilder perhaps, that adds the event in dtor, to make
//        it exception safe

Event *Events::spawnLocalEvent(short type, const class vec2 &pos, unsigned flags) {
  Event *event = new LocalEvent(type, pos);  
  _events.push_back(event);
  return event;
}

Event *Events::spawnGlobalEvent(short type, unsigned flags) {
  Event *event = new Event(type);  
  _events.push_back(event);
  return event;
}
