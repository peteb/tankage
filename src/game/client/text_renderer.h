#ifndef GAME_CLIENT_TEXT_RENDERER_H
#define GAME_CLIENT_TEXT_RENDERER_H

#include <string>
#include <vector>

class TextRenderer {
public:
  TextRenderer(class Portal &services, class TextureLoader &texloader);
  
  void renderText(const std::string &text, const class vec2 &pos, float scale = 1.0f);
  
private:
  void fillVerts(const std::string &text, const class vec2 &origin, 
                 const class vec2 &scale, 
                 std::vector<float> &coords, std::vector<float> &tcs);
  
  class TextureLoader &_texloader;
  class Graphics *_gfx;
  class Texture *_font;
  
  std::vector<float> _coordbuf;
  std::vector<float> _tcbuf;
};

#endif // !GAME_CLIENT_TEXT_RENDERER_H
