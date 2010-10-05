/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "texture_loader.h"
#include "texture.h"
#include <IL/IL.h>
#include <stdexcept>
#include <sstream>
#include <OpenGL/OpenGL.h>
#include <boost/make_shared.hpp>

TextureLoader::TextureLoader() {
   ilInit();
}

TextureLoader::~TextureLoader() {
   
}

boost::shared_ptr<Texture> TextureLoader::loadTexture(const std::string & name) {
   // check cache for texture
   TextureCache::iterator iter = cachedTextures.find(name);
   
   if (iter != cachedTextures.end()) {
      if (iter->second.expired()) // the cache entry expired
         cachedTextures.erase(iter);
      else
         return boost::shared_ptr<Texture>(iter->second);
   }
   
   // not found in cache, load
   ILuint newImage = 0;
	ilGenImages(1, &newImage);
	ilBindImage(newImage);

	if (ilLoadImage(name.c_str()) == IL_FALSE) {
		std::stringstream ss;

		ILenum errorCode;
		while ((errorCode = ilGetError()) != IL_NO_ERROR) {
			ss << errorCode << ", ";
		}

		throw std::runtime_error("failed to load image, " + ss.str());
	}
	
   ILuint width = ilGetInteger(IL_IMAGE_WIDTH);
   ILuint height = ilGetInteger(IL_IMAGE_HEIGHT);
   ILuint format = ilGetInteger(IL_IMAGE_FORMAT); // IL_RGB, IL_RGBA, IL_BGR, IL_BGRA, etc
   ILuint bpp = ilGetInteger(IL_IMAGE_BPP);
   
   const void * data = ilGetData();
   
   // generate texture
   GLuint newTexture;
   glEnable(GL_TEXTURE_2D);
   glGenTextures(1, &newTexture);
   glBindTexture(GL_TEXTURE_2D, newTexture);
   glTexImage2D(
      GL_TEXTURE_2D, 0, bpp, width, height, 0, format,
      GL_UNSIGNED_BYTE, data
   );
   
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   boost::shared_ptr<Texture> texture(new Texture(newTexture));
   cachedTextures.insert(std::make_pair(name, texture));
   
   return texture;
}

