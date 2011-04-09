#ifndef PLATFORM_GLFW_WM_H
#define PLATFORM_GLFW_WM_H

#include <platform/window_manager.h>
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
  void displayError(const char *title, const char *description);
  
  rect _size;
};

}

#endif // !PLATFORM_GLFW_WM_H
