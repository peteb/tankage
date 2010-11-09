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
   class BoundedSprite;
   class Texture;

   class Subsystem;

   class SpriteEventHandler {
   public:
      virtual ~SpriteEventHandler() {}
      
      virtual void enteredView() {}
      virtual void leftView() {}
   };
   

   class Sprite : public ReferenceFrame2 {
   private:
      Sprite(const Ref<Graphics::Texture>::SharedPtr & texture, BoundedSprite * sceneNode);

      friend class Graphics::Subsystem;

   public:
      std::vector<Vertex2T2> constructVertices() const;

      void enteredView();
      void leftView();
      
      // ReferenceFrame2 --------------------------------------------
      void setPosition(const vec2 & newPos);
      vec2 getPosition() const;
      void setOrientation(const mat2 & orient);
      mat2 getOrientation() const;
      // ------------------------------------------------------------
      
      rect getSize() const;
      Ref<Graphics::Texture>::SharedPtr getTexture() const;
      
      void setEventHandler(const Ref<SpriteEventHandler>::WeakPtr & eventHandler);
      
   private:
      Ref<SpriteEventHandler>::WeakPtr eventHandler;
      Ref<Graphics::Texture>::SharedPtr texture;
      rect size;
      vec2 position;
      mat2 orientation;
      BoundedSprite * sceneNode;
   };
   
}

#endif /* end of include guard: SPRITE_H_CD2WG4HZ */
