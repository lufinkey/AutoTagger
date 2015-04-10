
#pragma once

#include "Scraper.h"

namespace autotagger
{
	class iTunesLookupScraper : public Scraper
	{
	public:
		virtual ScrapedData scrape(const std::string&pagedata) override;
	};
}
