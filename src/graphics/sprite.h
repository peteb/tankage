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


	  void setGrid(int columns, int rows);
	  void setCell(int x, int y);
	  
	  void setOffset(const vec2 & offset);
      rect getSize() const;
	  rect getBoundingBox() const;
      void setEventHandler(const Ref<SpriteEventHandler>::WeakPtr & eventHandler);
	  void setDelegate(const Ref<Graphics::Sprite> & delegate);
      
   private:
      std::vector<Vertex2T2> constructVertices() const;

	  Ref<Graphics::Sprite> delegate;
      Ref<SpriteEventHandler>::WeakPtr eventHandler;
      Ref<Graphics::Renderer>::SharedPtr renderer;
      rect size, originalSize;
      vec2 position, offset;
      mat2 orientation;
	  int columns, rows;
	  int cellX, cellY;
   };
   
}

#endif /* end of include guard: SPRITE_H_CD2WG4HZ */
