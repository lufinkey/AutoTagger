
#pragma once

#include "../libxml_plus/libxml_plus.h"

#include <string>
#include <list>
#include <map>

namespace autotagger
{
	typedef std::map<std::string, std::string> Track;
	
	typedef struct
	{
		std::vector<Track> track_properties;
		std::map<std::string, std::string> album_properties;
	} ScrapedData;
	
	class Scraper
	{
	private:
		virtual ~Scraper(){}
		virtual ScrapedData scrape(const std::string&pagedata) = 0;
	};
}
