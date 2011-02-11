#include <engine/logging/logging.h>

#include <cstdio>
#include <cstdarg>


Log::Logging::Logging() {
	// Snail-Wail is registered in Twitter as an application. They provide
	// consumer key and secret. These will probably never change
    _twitter.getOAuth().setConsumerKey("dL0G2z0Uc3tz88mU8Kww");
    _twitter.getOAuth().setConsumerSecret("B7iNYddua7DWxEg2kBKlomUYfKMM88rSIvr0c7Cc");
	// Tokens are generated semi-manually using user provided pin code
	_twitter.getOAuth().setOAuthTokenKey("95492131-kGGgFMi14Cra8uhDmLI3kkKJ8aI1GUTauP660fSM");
    _twitter.getOAuth().setOAuthTokenSecret("lDeMu1aW2QVAQR9XskbpRNQnt215X87PcKfGVS9I");
} // Logging

void Log::Logging::write(Logging::LogType type, const char *format, ...) {

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
  vprintf(format, args);
  printf("\n");

  if (type == Logging::TWEET) {
    char buffer[1024];
    vsprintf(buffer, format, args);
    std::string message = buffer;
    if (!_twitter.statusUpdate(message)) {
      std::string reply;
      _twitter.getLastCurlError(reply);
      write(Logging::ERROR, reply.c_str());
    } 
  }

  va_end (args);
} // write

