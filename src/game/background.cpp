
#include <game/background.h>
#include <engine/portal.h>
#include <engine/graphics.h>
#include <engine/image_loader.h>
#include <engine/texture.h>
#include <utils/color.h>
#include <utils/vec.h>
#include <utils/rect.h>

Background::Background(class Portal &interfaces, SystemContext *ctx)
  : System(ctx)
{
  graphics = interfaces.requestInterface<Graphics>();
  ImageLoader *imgLoader = interfaces.requestInterface<ImageLoader>();
  Image *img = imgLoader->loadImage("../data/smoke.png");
  tex = graphics->createTexture(img);
}
  
void Background::render() {
  const color4 desertColor(0.957f, 0.917f, 0.682f, 1.0f);
  graphics->clear(desertColor);

  graphics->setBlend(Graphics::BLEND_ALPHA);
  graphics->enableTextures();
  tex->bind();

  graphics->drawQuad(rect(vec2(400.0f, 300.0f), 128, 128));
}

