#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* logFile = NULL;
char* _indent = NULL;

void openLog(const char* indent, const char* _file) {
   if (logFile != NULL || _indent != NULL) exit(EXIT_FAILURE);

   logFile = fopen(_file, "a+");
   if(!(_indent = (char *) malloc( strlen(indent) + 2))) 
       exit(EXIT_FAILURE);
   snprintf(_indent, strlen(indent) + 2, "%s: ", indent);
   exit(EXIT_SUCCESS);
} 

void closeLog() {
   if (logFile) {
      fclose(logFile);
      free(_indent);
   }
}

void logMsg(char* msg) {
   if (logFile) {
     fprintf(logFile, "%s%s\n", _indent, msg);  
   } else printf("logFile ain't open\n");
}

int main(int argc, char** argv) {
  openLog("DOWNLOAD", "/home5/investa5/logs/datalog");
  logMsg("helllo");
   
  // fork the child
  int pid = fork();
  if (pid < 0) {
     printf("failed\n");
     logMsg("Fork failed");
     closeLog();
     exit(EXIT_FAILURE);
  } else if (pid > 0) {
     printf("parent it is..\n");
     logMsg("Fork successful, I'm the parent and I'm leaving");
     exit(EXIT_SUCCESS);
  } else {
     // The child would have inherited the open log file handle, should I open again?
     printf("child it is..\n");
     logMsg("I'm the child");
     closeLog();
  }

  return 0;
}
