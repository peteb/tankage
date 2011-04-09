#ifndef PLATFORM_DEVIL_IMAGE_LOADER_H
#define PLATFORM_DEVIL_IMAGE_LOADER_H

#include <platform/image_loader.h>
#include <utils/singleton.h>

namespace DevIl {

class ImageLoader : public ::ImageLoader, public Singleton<ImageLoader> {
public:
  ImageLoader();
  ~ImageLoader();
  
  class Image *loadImage(const std::string &filename);
  
};

}

#endif // !PLATFORM_DEVIL_IMAGE_LOADER_H
