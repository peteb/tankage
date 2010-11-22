/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef SPRITE_H_CD2WG4HZ
#define SPRITE_H_CD2WG4HZ

#include <vector>
#include "math/vec2.h"
#include "reference_frame2.h"
#include "graphics/vertex.h"
#include "math/rect.h"
#include "ref.h"

namespace Graphics {
   class Renderer;
   class RenderList;

   class SpriteEventHandler {
   public:
      virtual ~SpriteEventHandler() {}
      
      virtual void enteredView() {}
      virtual void leftView() {}
   };
   

   class Sprite : public ReferenceFrame2 {
   public:
      Sprite(const Ref<Graphics::Renderer>::SharedPtr & renderer, const rect & size);

	   void enqueueRender(const Ref<Graphics::RenderList>::SharedPtr & renderList);
      void enteredView();
      void leftView();
      
      // ReferenceFrame2 --------------------------------------------
      void setPosition(const vec2 & newPos);
      vec2 getPosition() const;
      void setOrientation(const mat2 & orient);
      mat2 getOrientation() const;
      // ------------------------------------------------------------
      
      rect getSize() const;
	   rect getBoundingBox() const;
      void setEventHandler(const Ref<SpriteEventHandler>::WeakPtr & eventHandler);
      
   private:
      std::vector<Vertex2T2> constructVertices() const;

      Ref<SpriteEventHandler>::WeakPtr eventHandler;
      Ref<Graphics::Renderer>::SharedPtr renderer;
      rect size;
      vec2 position;
      mat2 orientation;
   };
   
}

#endif /* end of include guard: SPRITE_H_CD2WG4HZ */
