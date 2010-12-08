//===- render_list.h - Render list class definition -------------*- c++ -*-===//
//
//                                   Snail Wail
// 
// (c) Copyright 2010 Peter Backman. All Rights Reserved. 
//
//===----------------------------------------------------------------------===//
//
// This file declares the RenderList class.
//
//===----------------------------------------------------------------------===//

#ifndef GRAPHICS_RENDER_LIST_H
#define GRAPHICS_RENDER_LIST_H

#include "ref.h"
#include <utility>
#include <vector>

namespace Graphics {
   class Renderer;
   class Mesh;
   class Device;

   /// A utility class that collects meshes and which renderers that should be
   /// used for rendering. A default renderer can be set which will be used if a
   /// mesh is added without a renderer.   
   class RenderList {
   public:
	  ~RenderList();

      /// Insert a model/mesh into the list using the renderer, or the default
      /// renderer will be used.
	  void insert(const Ref<Renderer>::SharedPtr & renderer,
                  const Ref<Mesh>::SharedPtr & modelData);

      /// Renders all the meshes (using their associated renderers) of the list.
	  void render(Graphics::Device & target);

      /// Sets the default renderer which will be used if a mesh is added
      /// without a renderer.
	  void setDefaultRenderer(const Ref<Graphics::Renderer> & renderer);
	  
   private:
	  typedef std::pair<Ref<Renderer>::SharedPtr, Ref<Mesh>::SharedPtr> Model;
	  
	  std::vector<Model *> models;
	  Ref<Graphics::Renderer> defaultRenderer;
   };
}

#endif // !GRAPHICS_RENDER_LIST_H
