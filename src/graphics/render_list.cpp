/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "render_list.h"
#include "gfx_subsystem.h"
#include "renderer.h"

using namespace Graphics;

RenderList::~RenderList() {
   for (size_t i = 0; i < models.size(); ++i) {
	  delete models[i];
   }
}

void RenderList::insert(const Ref<Renderer>::SharedPtr & renderer,
						const Ref<Mesh>::SharedPtr & modelData)
{
   models.push_back(new Model(renderer, modelData));
}

void RenderList::render(RenderContext & context) {
   for (size_t i = 0; i < models.size(); ++i) {
	  Renderer * renderer = models[i]->first.get();
	  Mesh * meshData = models[i]->second.get();
	  
	  if (meshData) {
		renderer->render(*meshData, context);
	  }
   }
   
}
