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

#include <platform/glfw/wm.h>
#include <platform/glfw/input.h>
#include <platform/portal.h>
#include <platform/opengl/graphics.h>
#include <platform/devil/image_loader.h>
#include <platform/enet/network.h>
#include <platform/config.h>

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

int main(int argc, char **argv) {
  // create and register default log consumer
  Log::registerConsumer(Log::DefaultLogConsumer());

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
