#include <platform/devil/image_loader.h>
#include <platform/image.h>
#include <IL/il.h>
#include <sstream>
#include <stdexcept>

namespace {
class DevilImage : public Image {
public:
  DevilImage(ILuint id) : _id(id) {}
  
  const void *data() const {
    ilBindImage(_id);
    return static_cast<const void *>(ilGetData());
  }
  
  size_t bytesPerPixel() const {
    ilBindImage(_id);
    return ilGetInteger(IL_IMAGE_BPP);
  }
    
  ComponentType dataType() const {
    ilBindImage(_id);
    ILuint type = ilGetInteger(IL_IMAGE_TYPE);
    Image::ComponentType ret;

    switch (type) {
    case IL_UNSIGNED_BYTE:
      ret = Image::IMAGE_UNSIGNED_BYTE;
      break;
      
    default:
      throw std::runtime_error("devil: using unsupported data type");
    }

    return ret;
  }
  
  ImageFormat format() const {
    ImageFormat ret;
    ilBindImage(_id);

    switch (ilGetInteger(IL_IMAGE_FORMAT)) {
    case IL_RGB:
      ret = Image::IMAGE_RGB;
      break;

    case IL_RGBA:
      ret = Image::IMAGE_RGBA;
      break;

    case IL_BGR:
      ret = Image::IMAGE_BGR;
      break;

    case IL_BGRA:
      ret = Image::IMAGE_BGRA;
      break;

    default:
      throw std::runtime_error("devil: using unsupported image format");
    }

    return ret;
  }

  rect size() const {
    ilBindImage(_id);
    ILuint width = ilGetInteger(IL_IMAGE_WIDTH);
    ILuint height = ilGetInteger(IL_IMAGE_HEIGHT);
    
    return rect(width, height);
  }

private:
  ILuint _id;
};

}

DevIl::ImageLoader::ImageLoader() {
  ilInit();
}

DevIl::ImageLoader::~ImageLoader() {

}


class Image *DevIl::ImageLoader::loadImage(const std::string &filename) {
  ILuint imageId;
  ilGenImages(1, &imageId);
  ilBindImage(imageId);

  if (ilLoadImage(filename.c_str()) == IL_FALSE) {
    std::stringstream ss;
    
    ILenum errorCode;
    while ((errorCode = ilGetError()) != IL_NO_ERROR) {
      ss << errorCode << ", ";
    }
    
    throw std::runtime_error("devil: failed to load image '" + filename + "': " + ss.str());
  }

  return new DevilImage(imageId);
}
