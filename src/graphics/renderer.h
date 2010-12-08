//===- renderer.h - Renderer interface definition ---------------*- c++ -*-===//
//
//                                   Snail Wail
// 
// (c) Copyright 2010 Peter Backman. All Rights Reserved. 
//
//===----------------------------------------------------------------------===//
//
// This file declares the Renderer interface.
//
//===----------------------------------------------------------------------===//

#ifndef GRAPHICS_RENDERER_H
#define GRAPHICS_RENDERER_H

#include "ref.h"
#include "math/rect.h"

namespace Graphics {
   class Mesh;
   class Device;

   /// A renderer uses a graphical device to draw a mesh onto the screen,
   /// implementing this interface makes it possible to specify how the
   /// rendering process is done at a finer level. This is similar in
   /// functionality to "materials" or "shaders" (Id engines). 
   class Renderer {
   public:
	  virtual ~Renderer() {}

      /// Render the mesh onto the target.
      virtual void render(const Ref<Graphics::Mesh>::SharedPtr & mesh,
                          Device & target) =0;

      /// Native size of the renderer. Ie, if the renderer uses a texture, it
      /// could return the size of the texture in pixels. Used by sprites,
      /// etc. for initial value.
      virtual rect getNativeSize() const =0;
   };

}

#endif // !GRAPHICS_RENDERER_H
