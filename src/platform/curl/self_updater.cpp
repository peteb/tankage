#include "self_updater.h"
#include <curl/curl.h>
#include <utils/log.h>

#include <sys/stat.h>
#include <cstdlib>
#include <cstdio>
#include <stdexcept>
#include <cerrno>

#ifdef _WIN32
#include <io.h>
#include <process.h>
#include <fstream>
#else 
#include <unistd.h>
#endif

namespace {
int out_file;
size_t bytes_rx;

#ifdef _WIN32
const char* out_filename;
#endif

size_t WriteData(void *buffer, size_t size, size_t nmemb, void *userp) {
#ifdef _WIN32
  size_t written = nmemb * size;
  std::ofstream file(out_filename, std::ios::app | std::ios::binary);
  if (file.is_open()) {
    file.write((char*)buffer, written);
    file.close();
  } else {
    Log(DEBUG) << "failed to write the file";
    written = 0;
  }
#else 
  size_t written = write(out_file, buffer, nmemb * size);
#endif
  bytes_rx += written;
  return written;
}
  
long FileLastModified(const std::string &filename) {
  struct stat file_stat;
  struct tm *file_time;

  if (stat(filename.c_str(), &file_stat) == -1) {
    throw std::runtime_error("failed to get last modification time for '" + filename + "'");
  }
  
  file_time = localtime(&file_stat.st_mtime);
  return mktime(file_time);
}
  
void install(const std::string &filename,
             const std::string &dest) {

#ifdef _WIN32
  if (rename(dest.c_str(), (std::string("_") +  dest).c_str() ) == -1) {
     throw std::runtime_error("failed to move file '" + 
                              dest + "' to '" + dest + "_" + "':" 
                              + std::string(strerror(errno)));
  }
#else 
  if (chmod(filename.c_str(), S_IRWXU) == -1) {
    throw std::runtime_error("failed to set executable bit for '" + 
                             filename + "':" + std::string(strerror(errno)));
  }
#endif
  if (rename(filename.c_str(), dest.c_str()) == -1) {
    throw std::runtime_error("failed to move file '" + 
                             filename + "' to '" + dest + "':" 
                             + std::string(strerror(errno)));
  }
  
  
}
  
void restart(const std::string &file) {
#ifdef _WIN32
  putenv("SKIP_UPDATE=true");
#else
  setenv("SKIP_UPDATE", "true", 1);
#endif
  
  // FIXME: do we have to shutdown all the services?
  
  Log(DEBUG) << "restarting binary...";
  if (execl(file.c_str(), file.c_str(), (char *)0) == -1) {
    throw std::runtime_error("failed to restart binary using new version!");
  }
}
}

void Curl::SelfUpdater::requestUpdate(const std::string &file, 
                                      const std::string &url) {
  
  long file_modified = FileLastModified(file);
  Log(DEBUG) << "'" << file << "' last modification: " << file_modified;
  
  CURL *handle = curl_easy_init();
  if (!handle) {
    Log(DEBUG) << "curl: failed to create handle";
    return;
  }
  
  // create a temp file for download
  char tmp_filename[32];

#ifdef _WIN32
  const char* fn_template = "temp.XXXXXX";
  strcpy(tmp_filename, fn_template);
  if (_mktemp_s(tmp_filename, strlen(fn_template) + 1)) {
    throw std::runtime_error("failed to create temporary filename..");
  }
  out_filename = tmp_filename;
#else 
  strcpy(tmp_filename, "/tmp/temp.XXXXX");
  out_file = mkstemp(tmp_filename);
#endif

  bytes_rx = 0;
  Log(DEBUG) << "created temp file '" << tmp_filename << "'";
  
  // download it
  curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, WriteData);
  curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
  curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1);
  curl_easy_setopt(handle, CURLOPT_FILETIME, 1);
  curl_easy_setopt(handle, CURLOPT_TIMECONDITION, CURL_TIMECOND_IFMODSINCE);
  curl_easy_setopt(handle, CURLOPT_TIMEVALUE, file_modified);
  
  CURLcode ret = curl_easy_perform(handle);
  close(out_file);
  
  if (ret != 0) {
    Log(DEBUG) << "curl: failed to perform (code: " << ret << ")";
    return;
  }
  
  long response = 0;
  curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &response);
  Log(DEBUG) << "curl: server responded with " << response;
  
  if (response != 304 && response != 200) {
    Log(DEBUG) << "curl: failed to get latest";
    curl_easy_cleanup(handle);
    return;
  }

  if (bytes_rx != 0 && response != 304) {
    // we got a new version
    long remote_time = 0;
    curl_easy_getinfo(handle, CURLINFO_FILETIME, &remote_time);          
    double time_diff = difftime(remote_time, file_modified);
    curl_easy_cleanup(handle);
    Log(INFO) << "local version is " << time_diff/60.0 << " minutes old";
    

    install(tmp_filename, file);
    Log(DEBUG) << "installed new binary";
    
    restart(file); // FIXME: forward all args from initial invocation
  }
  else {
    curl_easy_cleanup(handle);
    remove(tmp_filename);
  }
}