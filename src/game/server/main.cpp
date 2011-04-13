#include <platform/portal.h>
#include <platform/window_manager.h>
#include <platform/graphics.h>

#include <game/server/gameserver.h>
#include <game/common/config.h>
#include <utils/log.h>

#include <unistd.h>


int app_main(Portal &interfaces, const std::vector<char *> &args) {
  // set-up file logger, keep default if not daemon
  char* home = getenv("HOME");
  if (home && args.size() > 1 && args[1] && std::string(args[1]) == "-d") {
    // remove default consumer and continue to log to file
    Log::clearConsumers();
    std::string path = std::string(home) + "/.tankage-server.log";
    Log::registerConsumer(Log::ToFileConsumer(path));
  }

  Config config(interfaces);
  server_RegisterVariables(config);
  config.parse(args); // read and set variables from cmdline
  
  GameServer server(interfaces);

  if (args.size() > 1 && args[1] && std::string(args[1]) == "-d") {
    pid_t pid = fork();
    if (pid > 0) {
      Log(INFO) << "Server successfully started on pid: " << pid;
      return EXIT_SUCCESS;
    } else if (pid < 0) {
      throw std::runtime_error("failed to fork");
    }
  }

  // child continues
  server.run();

  return EXIT_SUCCESS;
}

