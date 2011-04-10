#include "self_updater.h"
#include <curl/curl.h>
#include <utils/log.h>

#include <sys/stat.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <stdexcept>

namespace Curl {
class UpdateResult : public ::UpdateResult {
public:
  bool gotUpdate() const {
    return false;
  }
};
}

Curl::SelfUpdater::SelfUpdater() {
    
}

Curl::SelfUpdater::~SelfUpdater() {
  
}

namespace {
FILE *out_file;
size_t bytes_rx;
  
size_t WriteData(void *buffer, size_t size, size_t nmemb, void *userp) {
  size_t written = fwrite(buffer, nmemb, size, out_file);
  bytes_rx += written * nmemb;
  return written * nmemb;
}
  
long FileLastModified(const std::string &filename) {
  struct stat file_stat;
  struct tm *file_time;

  if (stat(filename.c_str(), &file_stat) == -1) {
    throw std::runtime_error("failed to get last modification time for '" + filename + "'");
  }
  
  file_time = gmtime(&(file_stat.st_mtime));
  return mktime(file_time);
}
}

::UpdateResult *Curl::SelfUpdater::requestUpdate(const std::string &file, 
                                                 const std::string &url) {
  
  long file_modified = FileLastModified(file);
  Log(DEBUG) << "'" << file << "' last modification: " << file_modified;
  
  CURL *handle = curl_easy_init();
  if (!handle) {
    Log(DEBUG) << "curl: failed to create handle";
    return 0;
  }
  
  // create a temp file for download
  char tmp_filename[32];
  strcpy(tmp_filename, "/tmp/temp.XXXXX");
  out_file = (FILE *)mkstemp(tmp_filename); // bad..
  Log(DEBUG) << "created temp file '" << tmp_filename << "'";
  
  curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, WriteData);
  curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
  curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 0);
  curl_easy_setopt(handle, CURLOPT_FILETIME, 1);
  curl_easy_setopt(handle, CURLOPT_TIMECONDITION, CURL_TIMECOND_IFMODSINCE);
  curl_easy_setopt(handle, CURLOPT_TIMEVALUE, file_modified);
  
  CURLcode ret = curl_easy_perform(handle);
  if (ret != 0) {
    Log(DEBUG) << "curl: failed to perform (code: " << ret << ")";
    return 0;
  }
  
  
  
  return new Curl::UpdateResult;
}