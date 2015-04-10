
#pragma once

#include "../Scraper/Scraper.h"

namespace autotagger
{
	class Tagger
	{
	public:
		static bool tagFile(const ScrapedData&scrapedData, unsigned int trackIndex, const std::string& filePath);
		static size_t tagFiles(const ScrapedData&scrapedData, const std::vector<std::pair<unsigned int, std::string> >& files);
	};
}
