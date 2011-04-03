#include <engine/portal.h>
#include <engine/window_manager.h>
#include <engine/graphics.h>

#include <game/server/gameserver.h>
#include <game/common/config.h>
#include <utils/log.h>

#include <unistd.h>


int app_main(Portal &interfaces, const std::vector<char *> &args) {
  //WindowManager *wm = interfaces.requestInterface<WindowManager>();
  //Graphics *gfx = interfaces.requestInterface<Graphics>();
  
  //wm->createWindow(800, 600);
  
  Config config(interfaces);
  server_RegisterVariables(config);
  config.parse(args); // read and set variables from cmdline
  
  GameServer server(interfaces);

  /*
    TODO:
      * new config should be saved
      * unittest packer
      * think about how removing objects will be handled (code and net)
        * hint the client that the object is removed (send ACTORS_REMOVED packet or similar)
        * client can remove object if not updated within a set time
        * if a hint was wrong (started to receive updates), recreate the object
      * start using packer instead of structs
      * start using new log
      * 
   
   */
  //const rect wndSize = wm->size();
  //gfx->setViewport(wndSize);
  //gfx->setOrtho(wndSize);
  //wm->swapBuffers(); 

  // deamonize server
  pid_t pid = fork();
  if (pid > 0) {
    // code executed only by parent
    Log(INFO) << "Server successfully started on pid: " << pid;
  } else if (pid < 0) {
    throw std::runtime_error("failed to fork");
  } else {
    // code executed only by child
    server.run();
  }

  // code executed by both parent and child
  return EXIT_SUCCESS;
}

