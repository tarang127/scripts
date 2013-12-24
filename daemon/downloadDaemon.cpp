#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <curl/curl.h>
#include "logger.h"

LOGGER logger("RSS", "/home5/investa5/error_log"); 

size_t write_data(void *buffer, size_t len, size_t nmemb, void *userp) {
  const char* _file  = "/home5/investa5/scripts/downloadedData.xml";
  FILE* _dataFile = fopen(_file, "a+");
  if (!_dataFile) {
      logger.log("Failed to open file to write to it");
      exit(EXIT_FAILURE);
  }

  int num = fwrite((char *) buffer, len, nmemb, _dataFile);
  fclose(_dataFile);
  return num;
}

int main(int argc, char** argv) {
  // fork the child
  int pid = fork();
  if (pid < 0) {
     logger.log("Fork failed");
     exit(EXIT_FAILURE);
  } else if (pid > 0) {
     exit(EXIT_SUCCESS);
  } else  {
    umask(077);   
 
    // Run this in a new session 
    pid_t sid = setsid();
    if (sid < 0) {
       logger.log("Failed to open a new session");
       exit(EXIT_FAILURE);   
    }

    if ((chdir("/home/investa5/")) < 0) {
       logger.log("Failed to change dir");
       exit(EXIT_FAILURE);
    }

    /* Close out the standard file descriptors */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    CURL* handle = curl_easy_init();
    if (handle) {
       curl_easy_setopt(handle, CURLOPT_URL, "http://feeds.marketwatch.com/marketwatch/topstories?format=xml");
       curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data); 
       CURLcode code = curl_easy_perform(handle);
       if (code != CURLE_OK) { 
	  logger.log("Curl command failed, received the following code");
          logger.log(curl_easy_strerror(code));
       }
       curl_easy_cleanup(handle);
    } else {
       logger.log("Curl init returned invalid handle");
    }
 }
 return 0;

}
