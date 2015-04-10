
#pragma once

#include "../libxml_plus/libxml_plus.h"

#include <string>
#include <list>
#include <map>
#include <vector>

namespace autotagger
{
	typedef struct
	{
		unsigned int index;
		std::string title;
		std::string artist;
		std::string composer;
		unsigned int disc;
	} TrackProperties;
	
	typedef struct
	{
		std::string title;
		std::string artist;
		std::string genre;
		unsigned int tracks;
		unsigned int discs;
		unsigned int year;
		std::string artwork_mimetype;
		std::vector<char> artwork;
	} AlbumProperties;
	
	typedef struct
	{
		std::vector<TrackProperties> track_properties;
		AlbumProperties album_properties;
	} ScrapedData;
	
	class Scraper
	{
	public:
		virtual ~Scraper(){}
		virtual ScrapedData scrape(const std::string&pagedata) = 0;
	};
}
