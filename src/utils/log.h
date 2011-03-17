
#ifndef UTILS_LOG_H
#define UTILS_LOG_H

#include <sstream>
#include <vector>
#include <string>


/**
 * The logging interface. Logs can be printed using overloaded << operator. 
 * If no consumer registered, all the logs will be streamed to std::cout.
 * Log class collects the messages during it's existence and calls all the
 * consumers in destructor, meaning, actual logging moment can be adjusted
 * by managing log class life-cycle. See usage examples below!
 * @code
 * // Implement and register log consumer 
 * struct my_log_consumer : public log::log_consumer {
 *   void write(log::log_level level, const std::string &line) {
 *     std::cout << line << std::endl;
 *   }
 * };
 * my_log_consumer consumer;
 * log::register_consumer(&consumer); 
 * // Do logging and feel free to try weird combinations I haven't tested yet
 * log(log::error) << "Very bad error occurred, exiting with code: " << -1;
 * @endcode
 */
struct log {
  enum log_level {
    error,
	warning,
    info,
	debug
  };
  struct log_consumer {
    virtual void write(log_level level, const std::string &line) =0;
    virtual ~log_consumer() {};
  };
  explicit log(log_level level);
  ~log();

  template<typename T> 
  log& operator<< (T data) {
    _stream << data;
	return *this;
  }
  static void register_consumer(log_consumer* consumer);

private:
  static std::vector<log::log_consumer*> _consumers;
  log_level _level;
  std::stringstream _stream;
};

#endif // !UTILS_LOG_H

