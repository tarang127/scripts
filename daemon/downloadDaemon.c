#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <curl/curl.h>

FILE* logFile = NULL;
char* _indent = NULL;

void openLog(const char* indent, const char* _file) {
   if (logFile != NULL || _indent != NULL) exit(EXIT_FAILURE);

   logFile = fopen(_file, "a+");
   if(!(_indent = (char *) malloc( strlen(indent) + 2))) 
       exit(EXIT_FAILURE);
   snprintf(_indent, strlen(indent) + 2, "%s: ", indent);
} 

void closeLog() {
   if (logFile) {
      fclose(logFile);
      free(_indent);
   }
}

void logMsg(const char* msg) {
   if (logFile) {
     fprintf(logFile, "%s%s\n", _indent, msg);  
   } else printf("logFile ain't open\n");
}

size_t write_data(void *buffer, size_t len, size_t nmemb, void *userp) {
  char* data = (char *) malloc (nmemb + 1);
  const char* _file  = "/home5/investa5/scripts/downloadedData";
  FILE* _dataFile = fopen(_file, "a+");
  if (!_dataFile) {
      logMsg("Failed to open file to write to it");
      exit(EXIT_FAILURE);
  }

  int num = fwrite((char *) buffer, len, nmemb, _dataFile);
  fclose(_dataFile);
  return num;
}

int main(int argc, char** argv) {
  openLog("DOWNLOAD", "/home5/investa5/logs/datalog");
   
  // fork the child
  int pid = fork();
  if (pid < 0) {
     logMsg("Fork failed");
     closeLog();
     exit(EXIT_FAILURE);
  } else if (pid > 0) {
     exit(EXIT_SUCCESS);
  } else {
     umask(077);   
 
     // Run this in a new session 
     pid_t sid = setsid();
     if (sid < 0) {
        logMsg("Failed to open a new session");
          exit(EXIT_FAILURE);   
     }

     if ((chdir("/home/investa5/")) < 0) {
        logMsg("Failed to change dir");
        exit(EXIT_FAILURE);
     }

     /* Close out the standard file descriptors */
     close(STDIN_FILENO);
     close(STDOUT_FILENO);
     close(STDERR_FILENO);

     CURL* handle = curl_easy_init();
     if (handle) {
        curl_easy_setopt(handle, CURLOPT_URL, "http://investassure.co.in");
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data); 
        CURLcode code = curl_easy_perform(handle);
        if (code != CURLE_OK) { 
	    logMsg("Curl command failed, received the following code");
            logMsg(curl_easy_strerror(code));
        }
        curl_easy_cleanup(handle);
     } else {
        logMsg("Curl init returned invalid handle");
     }
     closeLog();
  }

  return 0;
}
