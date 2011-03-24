#ifndef GAME_SERVER_ENTITY_H
#define GAME_SERVER_ENTITY_H

#include <game/common/control.h>

class Entity {
public:
  virtual ~Entity() {}
  
  virtual void snap(class Packer &msg, const class ClientSession *client) {}
  virtual void tick() {}
  virtual int id() const {return -1; }
};

#endif // !GAME_SERVER_ENTITY_H
