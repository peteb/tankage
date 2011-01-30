#ifndef ENGINE_WINDOW_MANAGER_H
#define ENGINE_WINDOW_MANAGER_H

#include <engine/interface.h>
#include <utils/rect.h>

class WindowManager : public Interface {
public:
  enum WindowState {
    OPENED = 0,
    ACTIVE,
    ICONIFIED
  };
  
  static std::string id() {
    return "window_manager";
  }

  virtual void createWindow(int width, int height) =0;
  virtual rect size() const =0;
  virtual double timeSeconds() =0;
  virtual void swapBuffers() =0;
  virtual bool windowState(WindowState state) =0;
  virtual void displayError(const char *title, const char *description) =0;
  
};

#endif // !ENGINE_WINDOW_MANAGER_H
