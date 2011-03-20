/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include <iostream>
#include <fstream>
#include <map>
#include <cstdlib>
#include "GL/glfw.h"

#include <engine/glfw/wm.h>
#include <engine/glfw/input.h>
#include <engine/portal.h>
#include <engine/opengl/graphics.h>
#include <engine/devil/image_loader.h>
#include <engine/enet/network.h>
#include <engine/config.h>

#include <utils/log.h>

#include <ctime>

// The GLFW module is a "starter" module, meaning, it implements 'main' and runs
// app_main

extern int app_main(Portal &portal, const std::vector<char *> &args);

int catch_app_main(Portal &portal, const std::vector<char *> &args) {
  try {
    return app_main(portal, args);
  }
  catch (const std::exception &e) {
    std::string description = std::string("An error has occured:\n") + e.what();
    // FIXME: if DEV, don't display this; make it easy to bt in gdb
    portal.requestInterface<Glfw::WindowManager>()->displayError(
      "Error in application; can't continue", description.c_str());
  }

  return EXIT_FAILURE;
}

class MainLogConsumer { 
public:
  MainLogConsumer(const std::string &file = "tankage.log") : _file(file) {
    if (char* home = getenv("HOME")) {
      _file = std::string(home).append("/.").append(file);
    }
  }
  void operator()(Log::Severity severity, const std::string &line) {
    std::fstream file(_file.c_str(), std::ios::out | std::ios::app);
    if (file.is_open()) {
      file << line << std::endl;
      file.close();
    } 
  }
private:
  std::string _file;
};

int main(int argc, char **argv) {
  // create and register main log consumer
  Log::registerConsumer(MainLogConsumer());

  std::stringstream ss;
  ss << argv[0];
  for (int i(1); i != argc; ++i) {
    ss << " " << argv[i];
  }
  Log(INFO) << "Starting-up: " << ss.str();

  Portal interfaces;

  if (!glfwInit()) {
    std::cerr << "glfw: failed to initialize" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "glfw: registering interfaces..." << std::endl;
  interfaces.registerInterface<Glfw::WindowManager>();
  interfaces.registerInterface<Glfw::Input>();
  interfaces.registerInterface<OpenGl::Graphics>();
  interfaces.registerInterface<DevIl::ImageLoader>();
  interfaces.registerInterface<Enet::Network>();

  std::cout << "glfw: initialized" << std::endl;

  std::vector<char *> args(argv, argv + argc);
  
  int exitCode;
  #ifndef DEV
  // Catch any exceptions thrown from main, then show it to the user
  exitCode = catch_app_main(interfaces, args);
  #else
  // Don't catch any exceptions at this level during debug; makes it easier to
  // see where the error is using gdb/other debugging tool
  exitCode = app_main(interfaces, args);
  #endif
  
  glfwTerminate();
}
