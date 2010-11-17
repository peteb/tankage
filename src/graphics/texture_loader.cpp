/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "texture_loader.h"
#include "texture.h"
#include "device.h"
#include <IL/IL.h>
#include <stdexcept>
#include <sstream>

using Graphics::TextureLoader;

TextureLoader::TextureLoader()
   : creator(NULL)
{
   ilInit();
}

TextureLoader::~TextureLoader() {
   
}

void TextureLoader::setDevice(Graphics::Device * device) {
   this->creator = device;
}


Ref<Graphics::Texture>::SharedPtr TextureLoader::loadTexture(const std::string & name) {
   // Check cache for texture
   TextureCache::iterator iter = cachedTextures.find(name);
   
   if (iter != cachedTextures.end()) {
      if (iter->second.expired()) // the cache entry expired
         cachedTextures.erase(iter);
      else
         return Ref<Graphics::Texture>::SharedPtr(iter->second);
   }

   // Not found in cache, load
   if (!creator) {
	  throw std::runtime_error("no creator device specified for texture loader");
   }
   
   ILuint newImage = 0;
   ilGenImages(1, &newImage);
   ilBindImage(newImage);

   if (ilLoadImage(name.c_str()) == IL_FALSE) {
	  std::stringstream ss;

	  ILenum errorCode;
	  while ((errorCode = ilGetError()) != IL_NO_ERROR) {
		 ss << errorCode << ", ";
	  }

	  throw std::runtime_error("failed to load image '" + name + "': " + ss.str());
   }
	
   ILuint width = ilGetInteger(IL_IMAGE_WIDTH);
   ILuint height = ilGetInteger(IL_IMAGE_HEIGHT);
   ILuint format = ilGetInteger(IL_IMAGE_FORMAT); // IL_RGB, IL_RGBA, IL_BGR, IL_BGRA, etc
   ILuint bpp = ilGetInteger(IL_IMAGE_BPP);
   
   const void * data = ilGetData();
   Ref<Graphics::Texture>::SharedPtr texture(creator->createTexture(data, width, height, bpp, format));

   cachedTextures.insert(std::make_pair(name, texture));
      
   return texture;
}

