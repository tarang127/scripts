#ifndef FILES_H
#define FILES_H

#include <iostream>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlreader.h>

#include "logger.h"
#include "downloadDaemon.h"

using namespace std;

namespace {
   const char* PATH = "/home5/investa5/scripts/daemon/files";
   const int MAX_NUM_ARTICLES_WITH_IMGS = 8;
   const int MAX_NUM_ARTICLES_NO_IMGS = 10;
   LOGGER logger("RSS", "/home5/investa5/error_log");
}

class FileURL {
   string URL;
   FileType type; 
   string OUTPUT_FILE;
   string HTML_FILE;

   void downloadFile();
   void parseXMLtoHTML();
   void parseTickerData();
   string gatherImageData(const xmlAttr* attr);
   void generateHTML(const xmlNode * a_node, string& output);
   static size_t writeURLData(void *ptr, size_t size, size_t nmemb, void* userdata);

   public:
	FileURL(const char* url, const char* output, const FileType type) : URL(url), type(type) {
	   OUTPUT_FILE.append(PATH).append("/").append(output);
	   HTML_FILE.append(OUTPUT_FILE).append(".html");
	}
	void processFile(); 
};
#endif
