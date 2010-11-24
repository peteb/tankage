/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "render_list.h"
#include "gfx_subsystem.h"
#include "renderer.h"

using namespace Graphics;

RenderList::~RenderList()
{
   for (size_t i = 0; i < models.size(); ++i) {
	  delete models[i];
   }
}

void RenderList::insert(const Ref<Renderer>::SharedPtr & renderer,
						const Ref<Mesh>::SharedPtr & modelData)
{
   std::auto_ptr<Model> newModel;
   
   if (renderer)
	  newModel.reset(new Model(renderer, modelData));
   else
	  newModel.reset(new Model(defaultRenderer.lock(), modelData));

   models.push_back(newModel.release());
}

void RenderList::render(Device & target)
{
   for (size_t i = 0; i < models.size(); ++i) {
	  const Ref<Renderer>::SharedPtr & renderer = models[i]->first;
	  const Ref<Mesh>::SharedPtr & meshData = models[i]->second;
	  
	  if (meshData && renderer) { // TODO: add default renderer
		  renderer->render(meshData, target);
	  }
   }
   
}

void RenderList::setDefaultRenderer(const Ref<Graphics::Renderer> & renderer)
{
   defaultRenderer = renderer;
}
