
#include "Scraper.h"

namespace autotagger
{
	class AppleScraper : public Scraper
	{
	public:
		virtual ScrapedData scrape(const std::string&pagedata) override;
	};
}
