
#include "AppleScraper.h"

namespace autotagger
{
	ScrapedData AppleScraper::scrape(const std::string&pagedata)
	{
		xmlpp::Document*doc = new xmlpp::Document(htmlReadDoc((xmlChar*)pagedata.c_str(), NULL, NULL, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING));
		
		
		
		delete doc;
	}
}
