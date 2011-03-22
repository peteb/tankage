#ifndef ENGINE_CFG_CFG_H
#define ENGINE_CFG_CFG_H

#include <game/common/config.h>
#include <game/common/system.h>

#include <engine/config.h>
#include <utils/singleton.h>
#include <ptrcfg/propertynode.h>

#include <string>
#include <map>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <iomanip>

class VariableBase {
public:
  virtual ~VariableBase() {}
  virtual void assign(const std::string &value) =0;
  virtual std::string value() const =0;
};

template<typename VarT>
class Variable : public VariableBase {
public:
  Variable(const VarT &default_value) 
    : _value(default_value)
  {
  }
  
  const VarT &operator *() const {return _value; }
  VarT &operator *() {return _value; }
  const VarT *operator ->() const {return &_value; }
  VarT *operator ->() {return &_value; }
  
  void assign(const std::string &value) {
    std::stringstream ss(value);
    if (!(ss >> _value)) {
      throw std::runtime_error("Failed to convert '" + value + "'");
    }
  }
  std::string value() const {
    std::stringstream ss;
    ss << _value;
    return ss.str();
  }
  
private:
  VarT _value;
};

class Config : public System { 
public:
  Config(const std::string &path = "");
  ~Config();

  static SystemContext::SystemId id() {
    return SystemContext::SYSTEM_CONFIG;
  }
  
  void init(const class Portal &modules);
  void start();
  
  void registerVariable(const char *subtree,
                        const char *name,
                        VariableBase *variable);  
  
  void parse(const std::vector<char *> &args);

private:
  std::string *getVariable(const std::string &system, const std::string &name) const;
  
  std::string property(const std::string &system, const std::string &name,
					   const std::string &defaultValue);
  void updateProperty(const std::string &system, const std::string &name,
                      const std::string &value); 
  //void registerConsumer(const std::string &system, ConfigConsumer* consumer);

private:
  typedef std::pair<std::string, std::string> StringPair;
  typedef std::map<StringPair, VariableBase *> ConsumerMap;
  std::string _path;
  PropertyNode _node;
  ConsumerMap _consumers;
  //std::multimap<std::string,ConfigConsumer*> _consumers;  
};

#endif // !CFG_CFG_H

