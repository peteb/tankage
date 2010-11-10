/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "render_list.h"
#include "graphics/gfx_subsystem.h"

Graphics::NonSortingRenderList::~NonSortingRenderList() {
   for (size_t i = 0; i < models.size(); ++i) {
	  delete models[i];
   }
}

void Graphics::NonSortingRenderList::insert(const Ref<Graphics::Texture>::SharedPtr & texture,
											const std::vector<Vertex2T2> & modelData)
{
   models.push_back(new Model(texture, modelData));
}

void Graphics::NonSortingRenderList::render(Graphics::Subsystem & renderer) {
   for (size_t i = 0; i < models.size(); ++i) {
	  renderer.render(models[i]->first, models[i]->second);
   }

}
