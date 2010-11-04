/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include <iostream>
#include <boost/make_shared.hpp>
#include <set>

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
   // first pass, generate <Rect, Geom> tupple, calculate bounding boxes
   // some kind of sorting might be possible here
   typedef std::vector<std::pair<rect, boost::shared_ptr<Geom> > > GeomVector;
   GeomVector resGeoms;

   std::vector<boost::weak_ptr<Geom> >::iterator iter = geoms.begin();
   
   while (iter != geoms.end()) {
      if (boost::shared_ptr<Geom> lockedGeom = iter->lock()) {
         rect fixedRect = lockedGeom->getSize();
         
         if (boost::shared_ptr<Physics::Body> lockedBody = lockedGeom->linkedBody.lock())
            fixedRect.origin = lockedBody->getPosition();
         else if (boost::shared_ptr<ReferenceFrame2> lockedRef = lockedGeom->refFrame.lock())
            fixedRect.origin = lockedRef->getPosition();
         
         resGeoms.push_back(std::make_pair(fixedRect, lockedGeom));
         ++iter;
      }
      else {
         iter = geoms.erase(iter);
      }
   }

   // a set to remember which pairs have collided already
   std::set<std::pair<Physics::Geom *, Physics::Geom *> > collided;
   
   // second pass, check collisions
   for (GeomVector::iterator it1 = resGeoms.begin(); it1 != resGeoms.end(); ++it1) {      
      for (GeomVector::iterator it2 = resGeoms.begin(); it2 != resGeoms.end(); ++it2) {
		 if (it1->second.get() != it2->second.get()) { 
			if (it1->second->collisionMask.test(it2->second->collisionId) ||
				it2->second->collisionMask.test(it1->second->collisionId)) {

			   if (rect::intersect(it1->first, it2->first) && collided.find(std::make_pair(it2->second.get(), it1->second.get())) == collided.end() ) {
				  if (it1->second->collisionMask.test(it2->second->collisionId))
					 it1->second->collided(it2->second);
				  if (it2->second->collisionMask.test(it1->second->collisionId))
					 it2->second->collided(it1->second);
				  
				  std::cout << "COLLISION " << it1->second.get() << " - " << it2->second.get() << std::endl;

				  collided.insert(std::make_pair(it1->second.get(), it2->second.get()));
			   }
			   
			}
			
		 }
      }
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
