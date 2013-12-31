#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "files.h"
#include "logger.h"
#include "downloadDaemon.h"

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

    // Close out the standard file descriptors 
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    FileURL fileURLs[] = {
       FileURL("", "/home5/investa5/scripts/daemon/files/stocksFeed.html", TICKER), 
       FileURL("http://feeds.marketwatch.com/marketwatch/topstories?format=xml", "mwTopStories.xml", FEED), 
       FileURL("http://www.moneycontrol.com/rss/MCtopnews.xml", "mcTopNews.xml", FEED)
    };

    for (int i = 0; i < 2; ++i) {
	fileURLs[i].processFile();
    }
 }
 return 0;


}
