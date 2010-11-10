/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef HEALTH_METER_H
#define HEALTH_METER_H

#include "math/vec2.h"
#include "ref.h"

namespace Graphics {class Texture; class RenderList; }

class HealthMeter {
public:
   void setValue(float value);
   void draw();
   void setPosition(const vec2 & origin);
   void setTexture(const Ref<Graphics::Texture>::SharedPtr & newTexture);
   void setDir(float dir);
   void enqueueRender(const Ref<Graphics::RenderList>::SharedPtr & renderList);
   
private:
   Ref<Graphics::Texture>::SharedPtr texture;
   float value, dir;
   vec2 origin;
};

#endif // !HEALTH_METER_H
