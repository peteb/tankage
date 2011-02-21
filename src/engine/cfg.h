#ifndef ENGINE_CFG_H 
#define ENGINE_CFG_H

#include <engine/interface.h>
#include <string>

class CfgConsumer {
public:
  virtual void updateCfg(const std::string &name, const std::string &value) =0;
};

class Cfg : public Interface {
public:
  static std::string id() {
    return "cfg";
  }
  virtual std::string property(const std::string &system, 
							   const std::string &name) =0;
  virtual void updateProperty(const std::string &system,
						      const std::string &name,
                              const std::string &value) =0; 
  /// Overrides configuration file with input arguments
  virtual void updateProperties(int argc, char **argv) =0;
  virtual void registerConsumer(const std::string &system,
                                CfgConsumer* consumer) =0;
};

#endif // !ENGINE_CFG_H

