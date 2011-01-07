/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef PHYS_SUBSYSTEM_H_YCDJQ9A8
#define PHYS_SUBSYSTEM_H_YCDJQ9A8

#include <string>
#include <vector>

#include <utils/vec.h>
#include <utils/rect.h>
#include "ref.h"

class Geom;
class Body;

namespace Graphics {
  class RenderList;
}


class PhysSubsystem {
public:
  PhysSubsystem();
  
  void update(float dt);
  void resizeArea(int width, int height);
  void addBody(const Ref<Body>::WeakPtr & body);
  void enqueueGeoms(const Ref<Graphics::RenderList>::SharedPtr & renderList);
  
  Ref<Body>::SharedPtr createBody();
  Ref<Geom>::SharedPtr createRectGeom(const rect & size);
  
private:
  void checkCollisions();
  
  // vec2 maxArea;
  // rect leftArea, rightArea;
  std::vector<Ref<Body>::WeakPtr> bodies;
  std::vector<Ref<Geom>::WeakPtr> geoms;
};
   
   


// might be able to remove HiController
// HiController -> PlayerLogic -> PhysicsThingie ->        Sprite
// (udlr, etc)     (restrain)    (restrain, collision)    (render)

// AiController -> PlayerLogic -> PhysicsThingie ->        Sprite

#endif /* end of include guard: PHYS_SUBSYSTEM_H_YCDJQ9A8 */
