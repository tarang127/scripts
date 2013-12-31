#include "files.h"
#include <errno.h>
#include <curl/curl.h>
#include <iostream>
#include <fstream>

void FileURL::processFile() {
   downloadFile();   

   switch(type) {
     case FEED:
        logger.log("Got FEED data");
	parseXMLtoHTML();
        break;
     case TICKER:
        logger.log("Got TICKET data");
        parseTickerData();
        break;
   }
}

void FileURL::parseTickerData() {
   // Does nothing for now.
}

size_t FileURL::writeURLData(void *ptr, size_t size, size_t nmemb, void* userdata) {
   FILE* stream = (FILE*) userdata;
   if (!stream) {
      logger.log("Failed to open file stream");
      return 0;
   }

   size_t written = fwrite((FILE*) ptr, size, nmemb, stream);
   return written;
}

void FileURL::downloadFile() {
   FILE* _dataFile = fopen(this->OUTPUT_FILE.c_str(), "w+");
   if (!_dataFile) {
      logger.log("Failed to open file to write to it");
      exit(EXIT_FAILURE);
   }
 
   CURL* handle = curl_easy_init();
   if (handle) {
      curl_easy_setopt(handle, CURLOPT_URL, this->URL.c_str());
      curl_easy_setopt(handle, CURLOPT_WRITEDATA, _dataFile);
      curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, writeURLData);
      curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1);
 
      CURLcode code = curl_easy_perform(handle);

      if (code) {
         logger.log("Curl command failed, received the following code");
         logger.log(curl_easy_strerror(code));
      }

      curl_easy_cleanup(handle);
      fclose(_dataFile);

   } else {
      logger.log("Curl init returned invalid handle");
   }
}

string FileURL::gatherImageData(const xmlAttr* attr) {
    bool isValidImg = true;
    string emptyImg = "<img  />", output = "<img ", nodata, feedburner = "feeds.feedburner.com";
    while (attr) {
       string content = (const char *) attr->children->content;
       string key((const char *)attr->name), value;

       if (content.find(feedburner)!=std::string::npos) {
	 isValidImg = false;
	 break;
       }
       if (!key.compare("height") || !key.compare("width")) {
	 content = "100";
       }
       output.append((const char *)attr->name).append("=\"").append(content).append("\" ");
       attr = attr->next;
    }
    output.append(" />");
    isValidImg = emptyImg.compare(output);

    return (isValidImg ? output : nodata);
}

void FileURL::generateHTML(const xmlNode * a_node, string& output) {
   const xmlNode *cur_node = NULL;
   string title, link, pubDate, img, description;
   int maxNumArticles = MAX_NUM_ARTICLES_NO_IMGS, numArticles = 0;

   for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE && !(xmlStrcmp (cur_node->name, (const xmlChar *) "item"))) {
            for (xmlNode* child = cur_node->children; child; child = child->next) {
	       const char* contentRead = (const char *) xmlNodeGetContent(child);
	       if(!(xmlStrcmp (child->name, (const xmlChar *) "title"))) {
		  title.assign(contentRead);
	       } else if (!(xmlStrcmp (child->name, (const xmlChar *) "link"))) {
		  link.assign(contentRead);
	       } else if (!(xmlStrcmp (child->name, (const xmlChar *) "pubDate"))) {
		  pubDate.assign(contentRead);
	       } else if (!(xmlStrcmp (child->name, (const xmlChar *) "description"))) {	
		  for (xmlNode* sub_child = child->children; sub_child; sub_child = sub_child->next) {
		      if (!xmlStrcmp (sub_child->name, (const xmlChar *) "img")) {
			  xmlAttr *attr = sub_child->properties;
			  img = gatherImageData(attr);
		      }
		   }
		   description.assign(contentRead);
	       }
            }
	    output.append("<div class=\"news-feed\">");
	    output.append("<div class=\"news-title\">");
	    output.append("<a href=\"").append(link).append("\">");
	    output.append(title);
	    output.append("</a>");
	    output.append("<a href=\"");
	    output.append(link);
	    output.append("\" /><img class=\"news-link\" src=\"/sites/all/themes/analytic/images/ext.gif\" /></a></div>");

	    output.append("<div class=\"submitted-by\">");
	    output.append("By Research Analyst | ");
	    output.append(pubDate);
	    output.append("</div>");
	    output.append("<div class=\"news-hr\"></div>");
	    if (!img.empty()) {
               maxNumArticles = MAX_NUM_ARTICLES_WITH_IMGS;
	       output.append("<div class=\"news-img\">");
	       output.append(img);
	       output.append("</div>");
	    }
            output.append("<div class=\"news-text\">");
	    output.append(description);
	    output.append("</div>");
	    output.append("</div>");
           
            if (++numArticles >= maxNumArticles) {
	       return;
            }
        } else {
            generateHTML(cur_node->children, output);
        }
   } 
}

void FileURL::parseXMLtoHTML() {
  string htmlOutput = "<div class=\"news-section\">";
  string executeScript = "/home5/investa5/scripts/daemon/src/replaceSpChars.sh " + this->OUTPUT_FILE; 
  system(executeScript.c_str());

  LIBXML_TEST_VERSION
  xmlDoc *doc = xmlReadFile(this->OUTPUT_FILE.c_str(), NULL, 0);
  if (doc == NULL) {
    logger.log("Couldn't parse XML file, Failed operation");
  }

  xmlNode *rootElement = xmlDocGetRootElement(doc);;
  generateHTML(rootElement, htmlOutput);
  htmlOutput.append("</div>");

  ofstream htmlFile;
  htmlFile.open (this->HTML_FILE.c_str(), ios::trunc);
  htmlFile << htmlOutput;
  htmlFile.close();

  xmlFreeDoc(doc);
  xmlCleanupParser();
}
