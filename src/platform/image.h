#ifndef PLATFORM_IMAGE_H
#define PLATFORM_IMAGE_H

#include <utils/rect.h>

class Image {
public:
  enum ImageFormat {
    IMAGE_RGB = 1,
    IMAGE_RGBA,
    IMAGE_BGR,
    IMAGE_BGRA,
    IMAGE_LUMINANCE
  };

  enum ComponentType {
    IMAGE_UNSIGNED_BYTE = 1
  };
    
  virtual ~Image() {}

  virtual const void *data() const =0;
  virtual size_t bytesPerPixel() const =0;
  virtual ComponentType dataType() const =0;
  virtual ImageFormat format() const =0;
  virtual rect size() const =0;
};

#endif // !PLATFORM_IMAGE_H
