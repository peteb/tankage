/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef HEALTH_METER_H
#define HEALTH_METER_H

#include "math/vec2.h"
#include <boost/shared_ptr.hpp>

namespace Graphics {class Texture; }

class HealthMeter {
public:
   void setValue(float value);
   void draw();
   void setPosition(const vec2 & origin);
   void setTexture(const boost::shared_ptr<Graphics::Texture> & newTexture);
   void setDir(float dir);
   
private:
   boost::shared_ptr<Graphics::Texture> texture;
   float value, dir;
   vec2 origin;
};

#endif // !HEALTH_METER_H
