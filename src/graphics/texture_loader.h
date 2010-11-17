/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef TEXTURE_LOADER_H_7GYX3QY2
#define TEXTURE_LOADER_H_7GYX3QY2

#include <map>
#include "ref.h"

namespace Graphics {
   class Texture;
   class Device;
   
   // this is really both a cache and a loader
   class TextureLoader {
   public:
	  TextureLoader();
	  ~TextureLoader();

	  void setDevice(Graphics::Device * device);
	  Ref<Graphics::Texture>::SharedPtr loadTexture(const std::string & name);
   
   private:
	  typedef std::map<std::string, Ref<Texture>::WeakPtr > TextureCache;
	  TextureCache cachedTextures;
	  Graphics::Device * creator;
   };
}

#endif /* end of include guard: TEXTURE_LOADER_H_7GYX3QY2 */
