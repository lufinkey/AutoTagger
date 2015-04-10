
#pragma once

#include <string>
#include <vector>

namespace autotagger
{
	class WebRequest
	{
	public:
		typedef void(*LoadFinishCallback)(const std::string&);
		
		static void separateURL(const std::string&url, std::string*host, std::string*subpage);
		static std::vector<char> getContentsOfURL(const std::string&url);
	};
}
