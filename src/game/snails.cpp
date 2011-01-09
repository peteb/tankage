#include <game/snails.h>
#include <engine/graphics.h>
#include <engine/texture.h>
#include <engine/image_loader.h>
#include <engine/portal.h>
#include <engine/image.h>
#include <utils/rect.h>
#include <algorithm>

Snails::Snails(const class Portal &interfaces, SystemContext *ctx)
  : System(ctx)
{
  graphics = interfaces.requestInterface<Graphics>();
  ImageLoader *imgLoader = interfaces.requestInterface<ImageLoader>();

  // First snail
  {
    std::auto_ptr<Image> img(imgLoader->loadImage("../data/snail_l.png"));
    Snail *snail = new Snail(vec2(50.0f, 300.0f));
    snail->setTexture(graphics->createTexture(img.get()));
  
    snails.push_back(snail);
  }

  // Second snail
  {
    std::auto_ptr<Image> img(imgLoader->loadImage("../data/snail_r.png"));
    Snail *snail = new Snail(vec2(800-50.0f, 300.0f));
    snail->setTexture(graphics->createTexture(img.get()));
  
    snails.push_back(snail);
  }
  
  
}

void Snails::render() {
  SnailVector::iterator i = snails.begin(), e = snails.end();
  for (; i != e; ++i) {
    (*i)->update();
    (*i)->render(graphics);
  }
}

Snail *Snails::snail(int id) const {
  return snails.at(id);
}


Snail::Snail(const vec2 &initialPos)
  : position(initialPos)
{
  std::fill(&_state[0], &_state[STATE_MAX], 0);
}

void Snail::startState(SnailState state) {
  _state[state] = true;
}

void Snail::stopState(SnailState state) {
  _state[state] = false;
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

void Snail::update() {
  if (_state[STATE_MOVE_UP])
    position += vec2(0.0f, -1.0f);
  if (_state[STATE_MOVE_DOWN])
    position += vec2(0.0f, 1.0f);
}
