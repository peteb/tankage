
#include "log.h"

std::list<log::log_consumer*> log::_consumers;

log::log(log_level level) : _level(level) {
} // log

log::~log() {
    // dumps all the log at dying, that means it's reached the end
    //std::cout << "level: " << _level << ", text: " << _stream.str() << std::endl;
    if (_consumers.empty()) {
      std::cout << _stream.str() << std::endl; 
    } else { 
      for (std::list<log_consumer*>::iterator it = _consumers.begin(); 
        it != _consumers.end(); ++it) {
        (*it)->write(_stream.str());
      }
    }
} // ~log

void log::register_consumer(log_consumer* consumer) {
  _consumers.push_back(consumer); 
} // register_consumer 

