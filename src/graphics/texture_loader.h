/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef TEXTURE_LOADER_H_7GYX3QY2
#define TEXTURE_LOADER_H_7GYX3QY2

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <map>

class Texture;

// this is really both a cache and a loader
class TextureLoader {
public:
   TextureLoader();
   ~TextureLoader();
   
   boost::shared_ptr<Texture> loadTexture(const std::string & name);
   
private:
   typedef std::map<std::string, boost::weak_ptr<Texture> > TextureCache;
   TextureCache cachedTextures; 
};

#endif /* end of include guard: TEXTURE_LOADER_H_7GYX3QY2 */
