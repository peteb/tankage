#ifndef ENGINE_SERVER_H
#define ENGINE_SERVER_H

#include <engine/interface.h>


/*
  FIXME: add use cases
*/

class Server : public Interface {
public:
  static std::string id() {
    return "server";
  }


};


#endif // !ENGINE_SERVER_H
