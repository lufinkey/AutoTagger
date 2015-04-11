
#include "AutoTagger/AutoTagger.h"
#include <dirent.h>
#include <iostream>

using namespace autotagger;

std::vector<std::string> getFilesFromDirectory(const std::string&path)
{
	std::vector<std::string> files;
	DIR*dir = opendir(path.c_str());
	if(dir==NULL)
	{
		return files;
	}
	struct dirent *entry = readdir(dir);
	while (entry != NULL)
	{
		std::string name = entry->d_name;
		if(name!="." && name!="..")
		{
			if(entry->d_type == DT_REG)
			{
				files.push_back(name);
			}
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return files;
}

int main(int argc, char* argv[])
{
	std::cout << "Enter a path to load files from: ";
	std::string path;
	std::getline(std::cin, path);
	
	std::vector<std::string> files = getFilesFromDirectory(path);
	
	std::cout << "Enter a url to scrape from: ";
	std::string url;
	std::getline(std::cin, url);
	
	std::string host;
	std::string subpage;
	WebRequest::separateURL(url, &host, &subpage);
	
	Scraper* scraper = nullptr;
	if(host == "itunes.apple.com")
	{
		scraper = new iTunesLookupScraper();
	}
	
	if(scraper != nullptr)
	{
		std::vector<char> data = WebRequest::getContentsOfURL(url);
		std::string data_str(data.data(), data.data()+data.size());
		ScrapedData scrapedData = scraper->scrape(data_str);
		size_t files_size = files.size();
		for(size_t i=0; i<files_size; i++)
		{
			std::string file = files.at(i);
			std::cout << std::endl << "File name: " << file << std::endl;
			std::cout << "Enter track number: ";
			std::string trackNo_str = 0;
			std::getline(std::cin, trackNo_str);
			unsigned int trackNo = std::atoi(trackNo_str.c_str());
			bool success = Tagger::tagFile(scrapedData, trackNo, path + "/" + file);
			if(!success)
			{
				std::cout << "Error tagging file" << std::endl;
			}
		}
	}
	else
	{
		std::cout << "Unsupported host: " << host << " Sorry!" << std::endl;
	}
	
	system("PAUSE");
	return 0;
}

