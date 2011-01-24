#include <game/texture_loader.h>
#include <engine/graphics.h>
#include <engine/image_loader.h>
#include <engine/image.h>
#include <engine/portal.h>
#include <memory>

void TextureLoader::init(const class Portal &interfaces) {
  gfx = interfaces.requestInterface<Graphics>();
  imgLoader = interfaces.requestInterface<ImageLoader>();
}

class Texture *TextureLoader::texture(const std::string &filename) {
  std::auto_ptr<Image> img(imgLoader->loadImage("../data/" + filename));
  return gfx->createTexture(img.release());
}

