#ifndef GAME_CLIENT_TEXT_RENDERER_H
#define GAME_CLIENT_TEXT_RENDERER_H

class TextRenderer {
public:
  TextRenderer(class Portal &services, class TextureLoader &texloader);
  
private:
  class TextureLoader &_texloader;
};

#endif // !GAME_CLIENT_TEXT_RENDERER_H
