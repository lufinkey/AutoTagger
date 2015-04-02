
#pragma once

#include <tinyxml2.h>

#include <string>
#include <list>
#include <map>

namespace autotagger
{
	typedef std::map<std::string, std::string> Track;
	
	typedef struct
	{
		std::list<Track> track_properties;
		std::map<std::string, std::string> album_properties;
	} ScrapedData;
	
	class Scraper
	{
	private:
		virtual ~Scraper(){}
		virtual ScrapedData scrape(const std::string&pagedata) = 0;
	};
}
