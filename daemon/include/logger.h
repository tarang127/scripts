#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

#ifndef LOGGER_H
#define LOGGER_H

using namespace std;

class LOGGER {
  private:
	FILE* logFile;
	string _indent;
  public:
	LOGGER(const char* indent, const char* _file) : logFile(NULL)  {
	  if (_file) {
   	    logFile = fopen(_file, "a+");
   		
    	    if (indent) {
	      std::ostringstream stringStream;
	      stringStream << "["<< indent <<"]: ";
	      _indent = stringStream.str();
	    }
	  }
	}

	~LOGGER() {
	   if (logFile) {
	      fclose(logFile);
           }
	}

	void log(const char* msg) {
	  if (logFile) {
            fprintf(logFile, "%s%s\n", _indent.c_str(), msg);
   	  } else printf("logFile ain't open\n");
	}
};

#endif
