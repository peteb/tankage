
#include <game/background.h>
#include <engine/portal.h>
#include <engine/graphics.h>
#include <engine/image_loader.h>
#include <engine/texture.h>
#include <utils/color.h>
#include <utils/vec.h>
#include <utils/rect.h>

Background::Background(class Portal &interfaces) {
  graphics = interfaces.requestInterface<Graphics>();
  ImageLoader *imgLoader = interfaces.requestInterface<ImageLoader>();
  Image *img = imgLoader->loadImage("../data/smoke.png");
  tex = graphics->createTexture(img);
}
  
void Background::render() {
  graphics->clear(color4::White());

  graphics->enableTextures();
  tex->bind();
  graphics->drawQuad(rect(vec2(400.0f, 300.0f), 128, 128));
}

