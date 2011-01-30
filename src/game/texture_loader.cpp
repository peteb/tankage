#include <game/texture_loader.h>
#include <engine/graphics.h>
#include <engine/image_loader.h>
#include <engine/image.h>
#include <engine/portal.h>

#include <config.h>

#include <memory>

void TextureLoader::init(const class Portal &interfaces) {
  gfx = interfaces.requestInterface<Graphics>();
  imgLoader = interfaces.requestInterface<ImageLoader>();
}

class Texture *TextureLoader::texture(const std::string &filename) {
  std::auto_ptr<Image> img(imgLoader->loadImage(std::string(RESOURCE_PATH) + "/" + filename));
  return gfx->createTexture(img.get());
}

