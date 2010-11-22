/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef GRAPHICS_RENDER_LIST_H
#define GRAPHICS_RENDER_LIST_H

#include "ref.h"
#include <utility>
#include <vector>

namespace Graphics {
   class Renderer;
   class Mesh;
   class Device;
   
   class RenderList {
   public:
	  ~RenderList();
	  
	  void insert(const Ref<Renderer>::SharedPtr & renderer, const Ref<Mesh>::SharedPtr & modelData);
	  void render(Graphics::Device & target);
	  
   private:
	  typedef std::pair<Ref<Renderer>::SharedPtr, Ref<Mesh>::SharedPtr> Model;
	  
	  std::vector<Model *> models;
   };
}

#endif // !GRAPHICS_RENDER_LIST_H
