/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef HEALTH_METER_H
#define HEALTH_METER_H

#include "math/vec2.h"
#include "ref.h"

namespace Graphics {class Renderer; class RenderList; }

class HealthMeter {
public:
   HealthMeter();
   
   void setValue(float value, float animationSpeed = 200.0f);
   void setPosition(const vec2 & origin);
   void setRenderer(const Ref<Graphics::Renderer>::SharedPtr & newRenderer);
   void setDir(float dir);
   void enqueueRender(const Ref<Graphics::RenderList>::SharedPtr & renderList, float dt);
   
private:
   Ref<Graphics::Renderer>::SharedPtr renderer;
   float value, valueShown, dir, animationSpeed;
   vec2 origin;
   float t;
};

#endif // !HEALTH_METER_H
