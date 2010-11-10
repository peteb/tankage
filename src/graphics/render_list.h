/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef GRAPHICS_RENDER_LIST_H
#define GRAPHICS_RENDER_LIST_H

#include <vector>
#include "vertex.h"
#include "ref.h"
#include <vector>
#include <utility>

namespace Graphics {
   class Texture;
   class Subsystem;
   
   class RenderList {
   public:
	  virtual ~RenderList() {}
	  virtual void insert(const Ref<Graphics::Texture>::SharedPtr & texture, const std::vector<Vertex2T2> & modelData) =0;
	  virtual void render(Graphics::Subsystem & renderer) =0;
   };

   class NonSortingRenderList : public RenderList {
   public:
	  ~NonSortingRenderList();
	  
	  void insert(const Ref<Graphics::Texture>::SharedPtr & texture, const std::vector<Vertex2T2> & modelData);
	  void render(Graphics::Subsystem & renderer);
	  
   private:
	  typedef std::pair<Ref<Graphics::Texture>::SharedPtr, std::vector<Vertex2T2> > Model;

	  std::vector<Model *> models;
   };
}

#endif // !GRAPHICS_RENDER_LIST_H
