#include <platform/logging/logging.h>

#include <cstdio>
#include <cstdarg>
#include <cstdlib>


Log::Logging::Logging(std::ostream &output) : _level(3), _output(output) {
  // check logging level  
  const char* level = getenv("TANKAGE_LOGGING_LEVEL");
  if (level) {
    _level = atoi(level);
  }   
  // Snail-Wail is registered in Twitter as an application. They provide
  // consumer key and secret. These will probably never change
  _twitter.getOAuth().setConsumerKey("dL0G2z0Uc3tz88mU8Kww");
  _twitter.getOAuth().setConsumerSecret("B7iNYddua7DWxEg2kBKlomUYfKMM88rSIvr0c7Cc");
  // Tokens are generated semi-manually using user provided pin code
  _twitter.getOAuth().setOAuthTokenKey("95492131-kGGgFMi14Cra8uhDmLI3kkKJ8aI1GUTauP660fSM");
  _twitter.getOAuth().setOAuthTokenSecret("lDeMu1aW2QVAQR9XskbpRNQnt215X87PcKfGVS9I");
} // Logging

void Log::Logging::write(Logging::LogType type, const char *format, ...) {
  if (_level < static_cast<size_t>(type)) {
    return;
  }
  switch (type) {
    case Logging::ERROR:
      printf("ER ");
      break;
    case Logging::WARNING:
      printf("WA ");
      break;
    case Logging::DEBUG:
      printf("DE ");
      break;
	case Logging::TWEET:
      printf("TW ");
      break;
    default:
      break;
  }

  va_list args;
  va_start (args, format);
  const size_t max = 1024;
  char buffer[max];
  vsnprintf(buffer, max, format, args);

  // write into the stream
  _output << buffer << std::endl;

  if (type == Logging::TWEET) {
    // send tweet, skip if empty
	std::string message = buffer[0] ? buffer : "";
    if (!message.empty() && !_twitter.statusUpdate(message)) {
      std::string reply;
      _twitter.getLastCurlError(reply);
      write(Logging::ERROR, reply.c_str());
    } 
  }

  va_end (args);
} // write

