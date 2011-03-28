#ifndef GAME_SERVER_ENTITY_H
#define GAME_SERVER_ENTITY_H

#include <game/common/control.h>
#include <utils/vec.h>

class Entity {
public:
  Entity(float radius) : _radius(radius) {}
  virtual ~Entity() {}
  
  virtual void snap(class Packer &msg, const class ClientSession *client) {}
  virtual void tick() {}
  virtual int id() const {return -1; }
  virtual vec2 position() const {return vec2(0.0f, 0.0f); }
  
  float radius() const {return _radius; }
  
private:
  float _radius;
};

#endif // !GAME_SERVER_ENTITY_H
