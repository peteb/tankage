/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef SPRITE_H_CD2WG4HZ
#define SPRITE_H_CD2WG4HZ

#include <vector>
#include "math/vec2.h"
#include "coord_system2.h"
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
   

   class Sprite : public CoordSystem2 {
   public:
      Sprite(const Ref<Graphics::Renderer>::SharedPtr & renderer, const rect & size);
	  
	  void enqueueRender(const Ref<Graphics::RenderList>::SharedPtr & renderList);
      void enteredView();
      void leftView();
      
      // CoordSystem2 --------------------------------------------
      void setTransform(const CoordSystemData2 & cs);
      CoordSystemData2 getTransform() const;

	  void setGrid(int columns, int rows);
	  void setCell(int x, int y);
	  
      rect getSize() const;
	  rect getBoundingBox() const;
      void setEventHandler(const Ref<SpriteEventHandler>::WeakPtr & eventHandler);
	  void setDelegate(const Ref<CoordSystem2> & delegate);
      
   private:
      std::vector<Vertex2T2> constructVertices() const;

	  Ref<CoordSystem2> delegate;
      Ref<SpriteEventHandler>::WeakPtr eventHandler;
      Ref<Graphics::Renderer>::SharedPtr renderer;
      rect size, originalSize;
      vec2 position;
      mat2 orientation;
	  int columns, rows;
	  int cellX, cellY;
   };
   
}

#endif /* end of include guard: SPRITE_H_CD2WG4HZ */
