#ifndef GAME_BACKGROUND_H
#define GAME_BACKGROUND_H

class Background {
public:
  Background(class Portal &interfaces);
  
  void render();

private:
  class Graphics *graphics;
};

#endif // !GAME_BACKGROUND_H
