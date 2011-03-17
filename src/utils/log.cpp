
#include "log.h"
#include <iostream>

std::vector<log::log_consumer*> log::_consumers;

log::log(log_level level) : _level(level) {
  switch (level) {
    case error: 
      _stream << "ER ";
      break;
    case warning:
      _stream << "WA ";
      break;
    case info:
      _stream << "IN ";
      break;
	case debug:
      _stream << "DE ";
      break;
    default:
      _stream << "?? ";
  }
} // log

log::~log() {
    if (_consumers.empty()) {
      // print to standard out if no consumers
      std::cout << _stream.str() << std::endl; 
    } else { 
      for (std::vector<log_consumer*>::iterator it = _consumers.begin(); 
        it != _consumers.end(); ++it) {
        (*it)->write(_level, _stream.str());
      }
    }
} // ~log

void log::register_consumer(log_consumer* consumer) {
  _consumers.push_back(consumer); 
} // register_consumer 

