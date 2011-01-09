#include <game/snails.h>
#include <engine/graphics.h>
#include <engine/texture.h>
#include <engine/image_loader.h>
#include <engine/portal.h>
#include <engine/image.h>
#include <utils/rect.h>

Snails::Snails(const class Portal &interfaces, SystemContext *ctx)
  : System(ctx)
{
  graphics = interfaces.requestInterface<Graphics>();
  ImageLoader *imgLoader = interfaces.requestInterface<ImageLoader>();

  std::auto_ptr<Image> img(imgLoader->loadImage("../data/snail_l.png"));
  Snail *snail = new Snail(vec2(50.0f, 300.0f));
  snail->setTexture(graphics->createTexture(img.get()));

  snails.push_back(snail);
}

void Snails::render() {
  SnailVector::iterator i = snails.begin(), e = snails.end();
  for (; i != e; ++i) {
    (*i)->render(graphics);
  }
}

Snail *Snails::snail(int id) const {
  return snails.at(id);
}

Snail::Snail(const vec2 &initialPos)
  : position(initialPos)
{
}

void Snail::startState(SnailState state) {
  
}

void Snail::setTexture(Texture *texture) {
  this->texture = texture;
}

void Snail::render(Graphics *graphics) {
  graphics->setBlend(Graphics::BLEND_ALPHA);
  graphics->enableTextures();
  texture->bind();

  graphics->drawQuad(rect(position, 64, 64));
}
