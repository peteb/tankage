#include "self_updater.h"
#include <curl/curl.h>
#include <utils/log.h>

#include <sys/stat.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <stdexcept>
#include <cerrno>

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
int out_file;
size_t bytes_rx;
  
size_t WriteData(void *buffer, size_t size, size_t nmemb, void *userp) {
  size_t written = write(out_file, buffer, nmemb * size);
  bytes_rx += written;
  return written;
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
  
void install(const std::string &filename,
             const std::string &dest) {
  if (chmod(filename.c_str(), S_IRWXU) == -1) {
    throw std::runtime_error("failed to set executable bit for '" + 
                             filename + "':" + std::string(strerror(errno)));
  }
  
  if (rename(filename.c_str(), dest.c_str()) == -1) {
    throw std::runtime_error("failed to move file '" + 
                             filename + "' to '" + dest + "':" 
                             + std::string(strerror(errno)));
  }
  
  
}
  
void restart() {
  setenv("SKIP_UPDATE", "true", 1);
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
  out_file = mkstemp(tmp_filename);
  bytes_rx = 0;
  Log(DEBUG) << "created temp file '" << tmp_filename << "'";
  
  // download it
  curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, WriteData);
  curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
  curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 0);
  curl_easy_setopt(handle, CURLOPT_FILETIME, 1);
  //curl_easy_setopt(handle, CURLOPT_TIMECONDITION, CURL_TIMECOND_IFMODSINCE);
  curl_easy_setopt(handle, CURLOPT_TIMEVALUE, file_modified);
  
  CURLcode ret = curl_easy_perform(handle);
  close(out_file);
  
  if (ret != 0) {
    Log(DEBUG) << "curl: failed to perform (code: " << ret << ")";
    return 0;
  }
  
  long response = 0;
  curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &response);
  Log(DEBUG) << "curl: server responded with " << response;
  
  if (bytes_rx != 0 && response != 304) {
    // we got a new version
    long remote_time = 0;
    curl_easy_getinfo(handle, CURLINFO_FILETIME, &remote_time);          
    double time_diff = difftime(remote_time, file_modified);
    Log(INFO) << "local version is " << time_diff/60.0 << " minutes old";
    
    install(tmp_filename, file);
    Log(DEBUG) << "installed new binary";
    
    restart();
  }
  
  return new Curl::UpdateResult;
}