#ifndef ENGINE_GLFW_INPUT_H
#define ENGINE_GLFW_INPUT_H

#include <engine/input.h>
#include <utils/singleton.h>

namespace Glfw {
class Input : public ::Input, public Singleton<Input> {
public:
  bool keyPressed(int key);
  int keycode(const std::string &key);
};

}

#endif // !ENGINE_GLFW_INPUT_H
