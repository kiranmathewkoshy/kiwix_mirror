/*
 * Copyright 2011 Emmanuel Engelhart <kelson@kiwix.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU  General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#include "reader.h"



std::string fakeUID(std::string input)
{
    std::string tmp="";
    for(int i=0;i<input.size();i++)
    {
        if(input[i]!='-')
            tmp+=input[i];
    }
    std::string output="";
    for(int i=0;i<8;i++)
    {
        output+=tmp[i];
    }
    output+="-";
    for(int i=8;i<12;i++)
    {
        output+=tmp[i];
    }
    output+="-";
    for(int i=12;i<16;i++)
    {
        output+=tmp[i];
    }
    output+="-";
    for(int i=12;i<16;i++)
    {
        output+=tmp[i];
    }
    output+="-";
    for(int i=16;i<tmp.size();i++)
    {
        output+=tmp[i];
    }
    return output;
}

static char charFromHex(std::string a) {
  std::istringstream Blat (a);
  int Z;
  Blat >> std::hex >> Z;
  return char (Z);
}


void unescapeUrl(string &url) {
  std::string::size_type pos = 0;
  while ((pos = url.find('%', pos + 1)) != std::string::npos &&
	 pos + 3 <= url.length()) {
    url.replace(pos, 3, 1, charFromHex(url.substr(pos + 1, 2)));
  }
  return;
}

namespace kiwix {

  /* Constructor */
  Reader::Reader(const string zimFilePath)
    : zimFileHandler(NULL) {
    string tmpZimFilePath = zimFilePath;

    /* Remove potential trailing zimaa */
    size_t found = tmpZimFilePath.rfind("zimaa");
    if (found != string::npos &&
	tmpZimFilePath.size() > 5 &&
	found == tmpZimFilePath.size() - 5) {
      tmpZimFilePath.resize(tmpZimFilePath.size() - 2);
    }

    this->zimFileHandler = new zim::File(tmpZimFilePath);

    if (this->zimFileHandler != NULL) {
      this->firstArticleOffset = this->zimFileHandler->getNamespaceBeginOffset('A');
      this->lastArticleOffset = this->zimFileHandler->getNamespaceEndOffset('A');
      this->currentArticleOffset = this->firstArticleOffset;
      this->nsACount = this->zimFileHandler->getNamespaceCount('A');
      this->nsICount = this->zimFileHandler->getNamespaceCount('I');
    }

    /* initialize random seed: */
    srand ( time(NULL) );
  }

  /* Destructor */
  Reader::~Reader() {
    if (this->zimFileHandler != NULL) {
      delete this->zimFileHandler;
    }
  }

  zim::File* Reader::getZimFileHandler() {
    return this->zimFileHandler;
  }

  /* Reset the cursor for GetNextArticle() */
  void Reader::reset() {
    this->currentArticleOffset = this->firstArticleOffset;
  }

  std::map<std::string, unsigned int> Reader::parseCounterMetadata() {
    std::map<std::string, unsigned int> counters;
    string content, mimeType, item, counterString;
    unsigned int contentLength, counter;
    string counterUrl = "/M/Counter";

    this->getContentByUrl(counterUrl, content, contentLength, mimeType);
    stringstream ssContent(content);

    while(getline(ssContent, item,  ';')) {
      stringstream ssItem(item);
      getline(ssItem, mimeType, '=');
      getline(ssItem, counterString, '=');
      if (!counterString.empty() && !mimeType.empty()) {
	sscanf(counterString.c_str(), "%u", &counter);
	counters.insert(pair<string, int>(mimeType, counter));
      }
    }

    return counters;
  }

  /* Get the count of articles which can be indexed/displayed */
  unsigned int Reader::getArticleCount() {
    std::map<std::string, unsigned int> counterMap = this->parseCounterMetadata();
    unsigned int counter = 0;

    if (counterMap.empty()) {
      counter = this->nsACount;
    } else {
      std::map<std::string, unsigned int>::const_iterator it = counterMap.find("text/html");
      if (it != counterMap.end())
	counter = it->second;
    }

    return counter;
  }

  /* Get the count of medias content in the ZIM file */
  unsigned int Reader::getMediaCount() {
    std::map<std::string, unsigned int> counterMap = this->parseCounterMetadata();
    unsigned int counter = 0;

    if (counterMap.empty())
      counter = this->nsICount;
    else {
      std::map<std::string, unsigned int>::const_iterator it;

      it = counterMap.find("image/jpeg");
      if (it != counterMap.end())
	counter += it->second;

      it = counterMap.find("image/gif");
      if (it != counterMap.end())
	counter += it->second;

      it = counterMap.find("image/png");
      if (it != counterMap.end())
	counter += it->second;
    }

    return counter;
  }

  /* Get the total of all items of a ZIM file, redirects included */
  unsigned int Reader::getGlobalCount() {
    return this->zimFileHandler->getCountArticles();
  }

  /* Return the UID of the ZIM file */
  string Reader::getId() {
    std::ostringstream s;
    s << this->zimFileHandler->getFileheader().getUuid();
    return  s.str();
  }

  /* Return a page url from a title */
  bool Reader::getPageUrlFromTitle(const string &title, string &url) {
    /* Extract the content from the zim file */
    std::pair<bool, zim::File::const_iterator> resultPair = zimFileHandler->findxByTitle('A', title);

    /* Test if the article was found */
    if (resultPair.first == true) {

      /* Get the article */
      zim::Article article = *resultPair.second;

      /* If redirect */
      unsigned int loopCounter = 0;
      while (article.isRedirect() && loopCounter++<42) {
	article = article.getRedirectArticle();
      }

      url = article.getLongUrl();
      return true;
    }

    return false;
  }

  /* Return an URL from a title*/
  string Reader::getRandomPageUrl() {
    zim::size_type idx = this->firstArticleOffset +
      (zim::size_type)((double)rand() / ((double)RAND_MAX + 1) * this->nsACount);
    zim::Article article = zimFileHandler->getArticle(idx);

    return article.getLongUrl().c_str();
  }

  /* Return the welcome page URL */
  string Reader::getMainPageUrl() {
    string url = "";

    if (this->zimFileHandler->getFileheader().hasMainPage()) {
      zim::Article article = zimFileHandler->getArticle(this->zimFileHandler->getFileheader().getMainPage());
      url = article.getLongUrl();

      if (url.empty()) {
	url = getFirstPageUrl();
      }
    } else {
	url = getFirstPageUrl();
    }

    return url;
  }

  bool Reader::getFavicon(string &content, string &mimeType) {
    unsigned int contentLength = 0;

    this->getContentByUrl( "/-/favicon.png", content,
			   contentLength, mimeType);

    if (content.empty()) {
      this->getContentByUrl( "/I/favicon.png", content,
			     contentLength, mimeType);


      if (content.empty()) {
	this->getContentByUrl( "/I/favicon", content,
			       contentLength, mimeType);

	if (content.empty()) {
	  this->getContentByUrl( "/-/favicon", content,
				 contentLength, mimeType);
	}
      }
    }

    return content.empty() ? false : true;
  }

  /* Return a metatag value */
  bool Reader::getMetatag(const string &name, string &value) {
    unsigned int contentLength = 0;
    string contentType = "";

    return this->getContentByUrl( "/M/" + name, value,
				  contentLength, contentType);
  }

  string Reader::getTitle() {
    string value;
    this->getMetatag("Title", value);
    if (value.empty()) {
      value = getLastPathElement(zimFileHandler->getFilename());
      std::replace(value.begin(), value.end(), '_', ' ');
      size_t pos = value.find(".zim");
      value = value.substr(0, pos);
    }
    return value;
  }

  string Reader::getDescription() {
    string value;
    this->getMetatag("Description", value);

    /* Mediawiki Collection tends to use the "Subtitle" name */
    if (value.empty()) {
      this->getMetatag("Subtitle", value);
    }

    return value;
  }

  string Reader::getLanguage() {
    string value;
    this->getMetatag("Language", value);
    return value;
  }

  string Reader::getDate() {
    string value;
    this->getMetatag("Date", value);
    return value;
  }

  string Reader::getCreator() {
    string value;
    this->getMetatag("Creator", value);
    return value;
  }

  string Reader::getPublisher() {
    string value;
    this->getMetatag("Publisher", value);
    return value;
  }

  string Reader::getOrigID() {
    string value;
    this->getMetatag("startfileuid", value);
        return fakeUID(value);
  }

  /* Return the first page URL */
  string Reader::getFirstPageUrl() {
    string url;

    zim::size_type firstPageOffset = zimFileHandler->getNamespaceBeginOffset('A');
    zim::Article article = zimFileHandler->getArticle(firstPageOffset);
    url = article.getLongUrl();

    return url;
  }

  bool Reader::parseUrl(const string &url, char *ns, string &title) {
    /* Offset to visit the url */
    unsigned int urlLength = url.size();
    unsigned int offset = 0;

    /* Ignore the '/' */
    while ((offset < urlLength) && (url[offset] == '/')) offset++;

    /* Get namespace */
    while ((offset < urlLength) && (url[offset] != '/')) {
      *ns= url[offset];
      offset++;
    }

    /* Ignore the '/' */
    while ((offset < urlLength) && (url[offset] == '/')) offset++;

    /* Get content title */
    unsigned int titleOffset = offset;
    while (offset < urlLength) {
      offset++;
    }

    /* unescape title */
    title = url.substr(titleOffset, offset - titleOffset);
    unescapeUrl(title);

    return true;
  }

  /* Get a content from a zim file */
  bool Reader::getContentByUrl(const string &urlStr, string &content, unsigned int &contentLength, string &contentType) {
    bool retVal = false;
    content="";
    contentType="";
    contentLength = 0;

    if (this->zimFileHandler != NULL) {

      /* Parse the url */
      char ns = 0;
      string titleStr;
      this->parseUrl(urlStr, &ns, titleStr);

      /* Main page */
      if (titleStr.empty() && ns == 0) {
	this->parseUrl(this->getMainPageUrl(), &ns, titleStr);
      }

      /* Extract the content from the zim file */
      std::pair<bool, zim::File::const_iterator> resultPair = zimFileHandler->findx(ns, titleStr);

      /* Test if the article was found */
      if (resultPair.first == true) {

	/* Get the article */
	zim::Article article = zimFileHandler->getArticle(resultPair.second.getIndex());

	/* If redirect */
	unsigned int loopCounter = 0;
	while (article.isRedirect() && loopCounter++<42) {
	  article = article.getRedirectArticle();
	}

	/* Get the content mime-type */
	contentType = string(article.getMimeType().data(), article.getMimeType().size());

	/* Get the data */
	content = string(article.getData().data(), article.getArticleSize());

	/* Try to set a stub HTML header/footer if necesssary */
	if (contentType == "text/html" && std::string::npos == content.find("<body>")) {
	  content = "<html><head><title>" + article.getTitle() + "</title><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /></head><body>" + content + "</body></html>";
	}

	/* Get the data length */
	contentLength = article.getArticleSize();

	/* Set return value */
	retVal = true;
      }
    }

    return retVal;
  }

  /* Search titles by prefix */
  bool Reader::searchSuggestions(const string &prefix, unsigned int suggestionsCount, const bool reset) {
    bool retVal = false;
    zim::File::const_iterator articleItr;
    std::vector<std::string>::iterator suggestionItr;
    int result;

    /* Reset the suggestions */
    if (reset) {
      this->suggestions.clear();
    }

    if (prefix.size()) {
      for (articleItr = zimFileHandler->findByTitle('A', prefix);
	   articleItr != zimFileHandler->end() &&
	     articleItr->getTitle().compare(0, prefix.size(), prefix) == 0 &&
	     this->suggestions.size() < suggestionsCount ;
	   ++articleItr) {

	  if (this->suggestions.size() == 0) {
	    this->suggestions.push_back(articleItr->getTitle());
	  } else {
	    for (suggestionItr = this->suggestions.begin() ;
		 suggestionItr != this->suggestions.end();
		 ++suggestionItr) {

	      result = articleItr->getTitle().compare(*suggestionItr);
	      if (result < 0) {
		this->suggestions.insert(suggestionItr, articleItr->getTitle());
		break;
	      } else if (result == 0) {
		break;
	      }
	    }

	    if (suggestionItr == this->suggestions.end()) {
	      this->suggestions.push_back(articleItr->getTitle());
	    }
	  }

	  /* Suggestions where found */
	  retVal = true;
      }
    }

    /* Set the cursor to the begining */
    this->suggestionsOffset = this->suggestions.begin();

    return retVal;
  }

  /* Try also a few variations of the prefix to have better results */
  bool Reader::searchSuggestionsSmart(const string &prefix, unsigned int suggestionsCount) {
    std::string myPrefix = prefix;

    /* Normal suggestion request */
    bool retVal = this->searchSuggestions(prefix, suggestionsCount, true);

    /* Try with first letter uppercase */
    myPrefix = kiwix::ucFirst(myPrefix);
    this->searchSuggestions(myPrefix, suggestionsCount, false);

    /* Try with first letter lowercase */
    myPrefix = kiwix::lcFirst(myPrefix);
    this->searchSuggestions(myPrefix, suggestionsCount, false);

    return retVal;
  }

  /* Get next suggestion */
  bool Reader::getNextSuggestion(string &title) {
    if (this->suggestionsOffset != this->suggestions.end()) {
      /* title */
      title = *(this->suggestionsOffset);

      /* increment the cursor for the next call */
      this->suggestionsOffset++;

      return true;
    }

    return false;
  }

  /* Check if the file has as checksum */
  bool Reader::canCheckIntegrity() {
    return this->zimFileHandler->getChecksum() != "";
  }

  /* Return true if corrupted, false otherwise */
  bool Reader::isCorrupted() {
    try {
      if (this->zimFileHandler->verify() == true)
	return false;
    } catch (exception &e) {
      cerr << e.what() << endl;
      return true;
    }

    return true;
  }

  /* Return the file size, works also for splitted files */
  unsigned int Reader::getFileSize() {
    zim::File *file = this->getZimFileHandler();
    zim::offset_type size = 0;

    if (file != NULL) {
      size = file->getFilesize();
    }

    return (size / 1024);
  }
}
