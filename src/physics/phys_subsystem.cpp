/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include <iostream>

#include "phys_subsystem.h"
#include "physics/body.h"

using Physics::Subsystem;

Subsystem::Subsystem() 
   : maxArea(vec2::Zero)
   , leftArea(vec2::Zero, vec2::Zero)
   , rightArea(vec2::Zero, vec2::Zero)
{
   
}

void Subsystem::update(float dt) {
   
}

void Subsystem::resizeArea(int width, int height) {
   maxArea = vec2(width, height);
   leftArea.origin = vec2(0.0f, 0.0f);
   leftArea.size = vec2(width / 2.0f, height);
   
   rightArea.origin = vec2(leftArea.size.x, 0.0f);
   rightArea.size = vec2(width / 2.0f, height);
}

Physics::Body * Subsystem::createBody(const std::string & area) {
   rect * rectArea = 0;
   
   if (area == "AREA_LEFT")
      rectArea = &leftArea;
   else if (area == "AREA_RIGHT")
      rectArea = &rightArea;
   else
      std::cout << "Warning: unknown area: " << area << std::endl;
      
   Physics::Body * body = new Physics::Body(*this, rectArea);
   
   return body;
}