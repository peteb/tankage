#ifndef ENGINE_IMAGE_LOADER_H
#define ENGINE_IMAGE_LOADER_H

#include <engine/image_loader.h>

class ImageLoader : public Interface {
public:
  static std::string id() {
    return "image_loader";
  }

  virtual class Image *loadImage(const std::string &filename) =0;
};

#endif // !ENGINE_IMAGE_LOADER_H
