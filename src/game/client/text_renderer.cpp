#include "text_renderer.h"
#include <game/common/texture_loader.h>
#include <engine/graphics.h>
#include <engine/portal.h>
#include <engine/texture.h>
#include <utils/vec.h>

TextRenderer::TextRenderer(class Portal &services, class TextureLoader &texloader)
  : _texloader(texloader)
{
  _gfx = services.requestInterface<Graphics>();
  _font = texloader.texture("smallfont.tga");
 // _font->setFiltering(false);
}

void TextRenderer::renderText(const std::string &text, const vec2 &pos) {
  // prepare buffers
  _coordbuf.reserve(text.size() * 4 * 2);
  _tcbuf.reserve(text.size() * 4 * 2);

  
  _gfx->enableTextures();
  _gfx->setBlend(Graphics::BLEND_ALPHA);
  _gfx->setTexture(_font);

  // collect data
  _coordbuf.clear();
  _tcbuf.clear();
  fillVerts(text, pos + vec2(1.0f, 1.0f), vec2(1.5f, 1.5f), _coordbuf, _tcbuf);
  
  _gfx->setColor(color4(0.0f, 0.0f, 0.0f, 0.5f));
  _gfx->drawQuads(2, _coordbuf.size() / 2, &_coordbuf[0], &_tcbuf[0]);

  // collect data
  _coordbuf.clear();
  _tcbuf.clear();
  fillVerts(text, pos, vec2(1.5f, 1.5f), _coordbuf, _tcbuf);
  
  _gfx->setColor(color4(1.0f, 1.0f, 1.0f, 1.0f));
  _gfx->drawQuads(2, _coordbuf.size() / 2, &_coordbuf[0], &_tcbuf[0]);
}

void TextRenderer::fillVerts(const std::string &text,
                             const vec2 &origin,
                             const vec2 &scale,
                             std::vector<float> &coords, 
                             std::vector<float> &tcs) 
{
  const int ROWS = 16;
  const int COLS = 16;

  const int WIDTH = 256/COLS * scale.x;
  const int HEIGHT = 256/ROWS * scale.y;

  vec2 offset = origin;
  offset.y -= HEIGHT / 2;
  offset.x -= (WIDTH * text.size()) / 4;
  
  for (size_t i = 0; i < text.size(); ++i) {
    char ascii = text[i];
    int map_x = ascii % COLS;
    int map_y = ascii / ROWS;

    vec2 c_start = offset;
    vec2 c_end = offset + vec2(WIDTH, HEIGHT);
    vec2 t_start(static_cast<float>(map_x) / COLS,
                 static_cast<float>(map_y) / ROWS);
    vec2 t_end(static_cast<float>(map_x + 1) / COLS,
               static_cast<float>(map_y + 1) / ROWS);
            
    coords.push_back(c_start.x);
    coords.push_back(c_start.y);
    coords.push_back(c_end.x);
    coords.push_back(c_start.y);
    coords.push_back(c_end.x);
    coords.push_back(c_end.y);
    coords.push_back(c_start.x);
    coords.push_back(c_end.y);
    
    tcs.push_back(t_start.x);
    tcs.push_back(t_start.y);
    tcs.push_back(t_end.x);
    tcs.push_back(t_start.y);
    tcs.push_back(t_end.x);
    tcs.push_back(t_end.y);
    tcs.push_back(t_start.x);
    tcs.push_back(t_end.y);
    
    offset.x += static_cast<float>(WIDTH) / 2.0f;
  }
}