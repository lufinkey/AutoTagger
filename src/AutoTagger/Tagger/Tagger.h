
#pragma once

#include "../Scraper/Scraper.h"

namespace autotagger
{
	class TagOptions
	{
	public:
		bool clearUnusedFields;
		
		TagOptions() : clearUnusedFields(false)
		{
			//
		}
	};
	
	class Tagger
	{
	public:
		static bool tagFile(const ScrapedData&scrapedData, unsigned int trackIndex, const std::string& filePath, const TagOptions&options=TagOptions());
		static size_t tagFiles(const ScrapedData&scrapedData, const std::vector<std::pair<unsigned int, std::string> >& files, const TagOptions&options=TagOptions());
	};
}
