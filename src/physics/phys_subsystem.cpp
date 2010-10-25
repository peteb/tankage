/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include <iostream>
#include <boost/make_shared.hpp>

#include "phys_subsystem.h"
#include "physics/body.h"
#include "physics/geom.h"

using namespace Physics;

Subsystem::Subsystem() 
   // : maxArea(vec2::Zero)
   // , leftArea(vec2::Zero, vec2::Zero)
   // , rightArea(vec2::Zero, vec2::Zero)
{
   
}

void Subsystem::update(float dt) {
   std::vector<boost::weak_ptr<Body> >::iterator iter = bodies.begin();
         
   //std::cout << "bodies: " << bodies.size() << std::endl;
   
   while (iter != bodies.end()) {
      if (boost::shared_ptr<Body> body = iter->lock()) {
         body->update(dt);
         ++iter;
      }      
      else {
         iter = bodies.erase(iter);
      }
   }
   
   checkCollisions();
}

void Subsystem::resizeArea(int width, int height) {
   // maxArea = vec2(width, height);
   // leftArea.origin = vec2(0.0f, 0.0f);
   // leftArea.size = vec2(width / 2.0f, height);
   // 
   // rightArea.origin = vec2(leftArea.size.x, 0.0f);
   // rightArea.size = vec2(width / 2.0f, height);
}

void Subsystem::checkCollisions() {
   // first pass, generate Geom, Rect tupple
   
   std::vector<std::pair<rect, boost::shared_ptr<Geom> > > fixedGeoms;

   std::vector<boost::weak_ptr<Geom> >::iterator iter = geoms.begin();
   
   while (iter != geoms.end()) {
      // todo: lock geom, add to fixedGeoms, remove if not possible
      ++iter;
   }
}

boost::shared_ptr<Body> Subsystem::createBody() {
   boost::shared_ptr<Body> body = boost::shared_ptr<Body>(new Body(*this));
   bodies.push_back(body);
   
   return body;
}

boost::shared_ptr<Geom> Subsystem::createRectGeom(const rect & size) {
   boost::shared_ptr<Geom> geom = boost::shared_ptr<Geom>(new Geom(size));
   geoms.push_back(geom);
   return geom;
}

// void Subsystem::addBody(const boost::weak_ptr<Body> & body) {
//    bodies.push_back(body);   
// }
// 
