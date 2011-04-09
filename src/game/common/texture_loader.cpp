#include <game/common/texture_loader.h>
#include <platform/graphics.h>
#include <platform/image_loader.h>
#include <platform/image.h>
#include <platform/portal.h>
#include <platform/config.h>

#include <memory>

TextureLoader::TextureLoader(const class Portal &interfaces) {
  gfx = interfaces.requestInterface<Graphics>();
  imgLoader = interfaces.requestInterface<ImageLoader>();
}

class Texture *TextureLoader::texture(const std::string &filename) {
  std::auto_ptr<Image> img(imgLoader->loadImage(std::string(RESOURCE_PATH) + "/" + filename));
  return gfx->createTexture(img.get());
}

