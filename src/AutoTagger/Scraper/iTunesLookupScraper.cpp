
#include "iTunesLookupScraper.h"
#include "../Request/WebRequest.h"
#include <rapidjson/document.h>

namespace autotagger
{
	ScrapedData iTunesLookupScraper::scrape(const std::string&pagedata)
	{
		ScrapedData scrapedData;
		
		rapidjson::Document doc;
		doc.Parse<0>(pagedata.c_str());
		
		const rapidjson::Value&results = doc["results"];
		size_t results_size = results.Size();
		for(size_t i=0; i<results_size; i++)
		{
			const rapidjson::Value& item = results[i];
			std::string wrapperType = item["wrapperType"].GetString();
			if(wrapperType == "track")
			{
				TrackProperties track;
				track.title = item["trackName"].GetString();
				track.artist = item["artistName"].GetString();
				track.disc = item["discNumber"].GetUint();
				track.index = item["trackNumber"].GetUint();
				scrapedData.track_properties.push_back(track);
				scrapedData.album_properties.discs = item["discCount"].GetUint();
			}
			else if(wrapperType == "collection")
			{
				scrapedData.album_properties.title = item["collectionName"].GetString();
				scrapedData.album_properties.artist = item["artistName"].GetString();
				scrapedData.album_properties.tracks = item["trackCount"].GetUint();
				scrapedData.album_properties.genre = item["primaryGenreName"].GetString();
				scrapedData.album_properties.year = (unsigned int)std::atoi(std::string(item["releaseDate"].GetString()).substr(0,4).c_str());
				std::string artworkUrl = item["artworkUrl100"].GetString();
				std::string resStr = ".100x100-75";
				size_t resIndex = artworkUrl.rfind(resStr);
				size_t fileExtIndex = artworkUrl.find_last_of('.');
				std::string fileExt = artworkUrl.substr(fileExtIndex + 1, artworkUrl.length());
				std::string fullResArtworkUrl = artworkUrl.substr(0, resIndex) + ".1200x1200-75." + fileExt;
				if(fileExt == "jpg")
				{
					fileExt = "jpeg";
				}
				scrapedData.album_properties.artwork_mimetype = "image/" + fileExt;
				scrapedData.album_properties.artwork = WebRequest::getContentsOfURL(fullResArtworkUrl);
			}
		}
		
		return scrapedData;
	}
}
