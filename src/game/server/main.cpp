
#include <engine/portal.h>
#include <engine/network.h>
#include <cstdlib>


int app_main(Portal &interfaces) {
  Network *net = interfaces.requestInterface<Network>();
  Address *adr = net->address("0.0.0.0:1234");
  
  return EXIT_SUCCESS;
}

