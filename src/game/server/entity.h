#ifndef GAME_SERVER_ENTITY_H
#define GAME_SERVER_ENTITY_H

class Entity {
public:
  virtual ~Entity() {}
  
  virtual void snap(class Packer &msg, const class ClientSession *client) {}
  virtual void tick() {}
};

#endif // !GAME_SERVER_ENTITY_H
