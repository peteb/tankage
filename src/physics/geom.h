/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef GEOM_H_R5WJNYU0
#define GEOM_H_R5WJNYU0

#include "reference_frame2.h"
#include "math/rect.h"
#include "ref.h"

#include <bitset>

namespace Physics {
	class Subsystem;
	class Body;
	class Geom;
	
	class GeomEventHandler {
	public:
		virtual ~GeomEventHandler() {}
		virtual void collided(const boost::shared_ptr<Geom> & with) {}
	};

   class Geom : public ReferenceFrame2 {
   private:
      Geom(const rect & size);
      
      friend class Physics::Subsystem;
      
   public:
      void setBody(const Ref<Physics::Body> & body);
      void setRefFrame(const Ref<ReferenceFrame2> & refFrame);
	   void setEventHandler(const Ref<GeomEventHandler> & eventHandler);

	   rect getSize() const;
      
      void setCollisionId(unsigned int collisionId);
      void setCollisionMask(const std::bitset<32> & mask);
	  void collided(const boost::shared_ptr<Geom> & with);

	  
	  // ReferenceFrame2 --------------------------------------------
      void setPosition(const vec2 & newPos);
      vec2 getPosition() const;
      void setOrientation(const mat2 & orient);
      mat2 getOrientation() const;
      // ------------------------------------------------------------
   
   private:
	   Ref<Physics::Body> linkedBody;
	   Ref<ReferenceFrame2> refFrame;
	   Ref<GeomEventHandler> eventHandler;
	   rect size;
      
	   std::bitset<32> collisionMask;
	   unsigned int collisionId;
	   
	   vec2 position;
   };
}

#endif /* end of include guard: GEOM_H_R5WJNYU0 */
