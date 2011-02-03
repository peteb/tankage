
#include <engine/portal.h>
#include <engine/network.h>
#include <cstdlib>


int app_main(Portal &interfaces) {
  Network *net = interfaces.requestInterface<Network>();
  Host *host = net->startHost("0.0.0.0:12345");
  
  return EXIT_SUCCESS;
}

