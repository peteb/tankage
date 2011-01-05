/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef GEOM_H_R5WJNYU0
#define GEOM_H_R5WJNYU0

#include "coord_system2.h"
#include "math/rect.h"
#include "ref.h"

#include <bitset>

class Object;
class PhysSubsystem;
class Geom;
class Body;
	
namespace Graphics {
class RenderList;
class Renderer;
}

class GeomEventHandler {
public:
  virtual ~GeomEventHandler() {}
  virtual void collided(const Ref<Geom>::SharedPtr & with) {}
};

class Geom : public CoordSystem2 {      
public:
  friend class PhysSubsystem; // TODO: get rid of this
  
  Geom(const rect & size);
  
  void setBody(const Ref<Body> & body);
  void setRefFrame(const Ref<CoordSystem2> & refFrame);
  void setEventHandler(const Ref<GeomEventHandler> & eventHandler);
  void setOffset(const vec2 & offset);
  void setPriority(int prio);
  int getPriority() const;
  
  rect getSize() const;
  void enqueueRender(const Ref<Graphics::RenderList>::SharedPtr & renderList);
  
  void setCollisionId(unsigned int collisionId);
  void setCollisionMask(const std::bitset<32> & mask);
  void collided(const Ref<Geom>::SharedPtr & with);
  
  Ref<Object>::WeakPtr getOwner() const;
  
  // CoordSystem2 --------------------------------------------
  void setTransform(const CoordSystemData2 & cs);
  CoordSystemData2 getTransform() const;
  
  Ref<Body> linkedBody;   
private:
  
  Ref<CoordSystem2> refFrame;
  Ref<GeomEventHandler> eventHandler;
  
  rect size;
  int priority;
  
  std::bitset<32> collisionMask;
  unsigned int collisionId;
  
  vec2 position;
};

#endif /* end of include guard: GEOM_H_R5WJNYU0 */
