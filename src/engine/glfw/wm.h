#ifndef ENGINE_GLFW_WM_H
#define ENGINE_GLFW_WM_H

#include <engine/window_manager.h>
#include <utils/singleton.h>

namespace Glfw {

class WindowManager : public ::WindowManager, public Singleton<WindowManager> {
public:
  WindowManager();

  rect size() const;
  void createWindow(int width, int height);
  double timeSeconds();
  void swapBuffers();
  bool windowState(WindowState state);

  rect _size;
};

}

#endif // !ENGINE_GLFW_WM_H
