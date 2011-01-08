#ifndef ENGINE_DEVIL_IMAGE_LOADER_H
#define ENGINE_DEVIL_IMAGE_LOADER_H

#include <engine/image_loader.h>
#include <utils/singleton.h>

namespace DevIl {

class ImageLoader : public ::ImageLoader, public Singleton<ImageLoader> {
public:
  class Image *loadImage(const std::string &filename);
  
};

}

#endif // !ENGINE_DEVIL_IMAGE_LOADER_H
